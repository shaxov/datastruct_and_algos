/**
 * @file	examples/algorithms/heap_sort.cpp
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

namespace detail
{

template<typename T, typename Compare = std::less<>>
class BinaryHeap
{
public:
    BinaryHeap(std::vector<T>&& data, Compare comp = {}) // heapify
        : data_{std::move(data)}, comp_{comp}, sz_{data_.size()}
    {
        if (!empty()) {
            for (size_t i = parent(last_index()); ; --i) {
                sift_down(i);
                if (i == 0) break;
            }
        }
    }

    void push_back(T const& value) { data_.push_back(value); ++sz_; sift_up(last_index()); }
    void push_back(T&& value) { data_.push_back(std::move(value)); ++sz_; sift_up(last_index()); }

    void pop(bool keep_tail = false) {
        require_not_empty();
        std::swap(data_[0], data_[last_index()]);
        if (!keep_tail)
            data_.pop_back();
        --sz_;
        if (!empty()) sift_down(0);
    }

    size_t size() const noexcept { return sz_; }
    bool empty() const noexcept { return sz_ == 0; }
    T const& top() const { require_not_empty(); return data_[0]; }

    std::vector<T>& get() { return data_; }

private:
    size_t last_index() const noexcept { return sz_ - 1; }
    static constexpr size_t parent(size_t i) noexcept { return (i - 1) / 2; }
    static constexpr size_t left(size_t i) noexcept { return 2 * i + 1; }
    static constexpr size_t right(size_t i) noexcept { return 2 * i + 2; }

    void require_not_empty() const {
        if (empty())
            throw std::out_of_range("heap is empty");
    }

    bool better(size_t i, size_t j) const {
        return !comp_(data_[i], data_[j]);
    }

    void sift_up(size_t i) {
        if (!i) return;

        while (i > 0 && better(i, parent(i))) {
            std::swap(data_[i], data_[parent(i)]);
            i = parent(i);
        }
    }

    void sift_down(size_t i) {
        while (left(i) < size()) {
            size_t best = left(i);
            size_t r = right(i);

            if (right(i) < size() && better(r, best))
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

} // namespace detail


template<std::ranges::random_access_range R, typename Compare = std::less<>>
constexpr void heapsort(R&& rng, Compare comp = {})
{
    auto heap = detail::BinaryHeap(std::move(rng), comp);
    while (!heap.empty()) heap.pop(true);
    rng = std::move(heap.get());
}


int main()
{
    std::vector<int> v{ 5, 1, 4, 2, 8, 0, 2 };

    heapsort(v);
    for (int x : v) std::cout << x << ' ';
    std::cout << std::endl;

    heapsort(v, std::greater<>{});
    for (int x : v) std::cout << x << ' ';
    return 0;
}