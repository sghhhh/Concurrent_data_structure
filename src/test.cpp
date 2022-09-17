//
// Created by s on 2022/9/15.
//
#include <iostream>
#include <lockSafe_queue.hpp>
#include <memory>

using std::cout;
using std::shared_ptr;
using std::unique_ptr;
using std::weak_ptr;
int main() {
    int a[] = {1, 2, 3, 4, 5, 231, 6, 7, 8, 9, 10, 11, 12, 13};
    pds::safeQueue<size_t> q(std::begin(a), std::end(a));
    for (int i = 0; i < sizeof(a) / sizeof(int); ++i)
        cout << *q.try_pop() << " ";
}