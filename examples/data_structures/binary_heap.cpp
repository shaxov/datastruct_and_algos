/**
 * @file	examples/data_structures/binary_heap.cpp
 * @date	Jul 2025
 * @author	Maksym Shpakovych
 */

/*
 Copyright (c) 2025 Maksym Shpakovych

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 */

#include <ranges>
#include <vector>
#include <utility>
#include <stdexcept>
#include <functional>
#include <iostream>
#include <initializer_list>


template<typename T, typename Compare = std::less<>>
class BinaryHeap
{
public:
    explicit BinaryHeap(Compare comp = {}) : comp_{comp} { }
    BinaryHeap(std::initializer_list<T> const& list) {
        for (auto l : list)
            push_back(std::move(l));
    }

    void push_back(T const& value) { data_.push_back(value); sift_up(last_index()); }
    void push_back(T&& value) { data_.push_back(std::move(value)); sift_up(last_index()); }

    void pop() {
        require_not_empty();
        data_.front() = std::move(data_.back());
        data_.pop_back();
        if (!data_.empty()) sift_down(0);
    }

    size_t size() const noexcept { return data_.size(); }
    bool empty() const noexcept { return data_.empty(); }
    T const& top() const { require_not_empty(); return data_.front(); }

private:
    size_t last_index() const noexcept { return data_.size() - 1; }
    static constexpr size_t parent(size_t i) noexcept { return (i - 1) / 2; }
    static constexpr size_t left(size_t i) noexcept { return 2 * i + 1; }
    static constexpr size_t right(size_t i) noexcept { return 2 * i + 2; }

    void require_not_empty() const {
        if (data_.empty())
            throw std::out_of_range("heap is empty");
    }

    bool better(size_t i, size_t j) const {
        return comp_(data_[i], data_[j]);
    }

    void sift_up(size_t i) {
        if (!i) return;

        while (i > 0 && better(i, parent(i))) {
            std::swap(data_[i], data_[parent(i)]);
            i = parent(i);
        }
    }

    void sift_down(size_t i) {
        while (left(i) < data_.size()) {
            size_t best = left(i);
            size_t r = right(i);

            if (right(i) < data_.size() && better(r, best))
                best = r;

            if (better(i, best)) break;
            std::swap(data_[i], data_[best]);
            i = best;
        }
    }

    std::vector<T> data_;
    Compare comp_;
    size_t sz_ = 0;
};


int main()
{
    auto heap = BinaryHeap<int>{10, 7, 1, 5, -9, 4};
    while (!heap.empty()) {
        std::cout << heap.top() << " ";
        heap.pop();
    }
    std::cout << std::endl;

    auto heap2 = BinaryHeap<int, std::greater<int>>{10, 7, 1, 5, -9, 4};
    while (!heap2.empty()) {
        std::cout << heap2.top() << " ";
        heap2.pop();
    }
    std::cout << std::endl;
    return 0;
}