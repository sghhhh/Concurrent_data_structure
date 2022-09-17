//
// Created by s on 2022/9/15.
//

#ifndef LIB_LOCKSAFE_QUEUE_HPP
#define LIB_LOCKSAFE_QUEUE_HPP

#include <condition_variable>
#include <memory>
#include <mutex>
#include <new>

namespace pds {
    template<typename T>
    class safeQueue {
    private:
        struct node {
            std::shared_ptr<T> data;
            std::shared_ptr<node> next;
        };
        std::mutex mutable head_mutex;
        std::mutex mutable tail_mutex;
        std::shared_ptr<node> head;
        std::condition_variable cond;
        std::shared_ptr<node> tail;

    protected:
        std::weak_ptr<node> get_tail() const {
            std::lock_guard<std::mutex> lock{tail_mutex};
            return tail;
        }

        auto pop_head() -> std::shared_ptr<node> {
            std::lock_guard<std::mutex> lock(head_mutex);
            if (head == get_tail().lock()) {
                return nullptr;
            }
            auto old = head;
            head = head->next;
            return old;
        }

    public:
        safeQueue() : head(new node), tail(head) {}
        safeQueue(safeQueue const &) = delete;
        safeQueue &operator=(safeQueue const &) = delete;

        std::shared_ptr<T> try_pop() {
            auto old = pop_head();
            return old ? std::move(old->data) : nullptr;
        }

        void push(T new_value) {
            std::shared_ptr<T> new_node{
                    std::make_shared<T>(std::move(new_value))};
            std::shared_ptr<node> next{new node};
            std::lock_guard<std::mutex> lock(tail_mutex);
            tail->data = new_node;
            tail->next = next;
            tail = next;
        }
        template<typename Iterator>
        safeQueue(Iterator first, Iterator last) : safeQueue() {
            while (first != last) {
                push(*first);
                ++first;
            }
        }
        ~safeQueue() {
            while (head != tail) {
                head = head->next;
            }
            head = nullptr;
            tail = nullptr;
        }
    };
}// namespace pds
#endif//LIB_LOCKSAFE_QUEUE_HPP
