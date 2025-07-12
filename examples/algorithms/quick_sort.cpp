/**
 * @file	examples/algorithms/quick_sort.cpp
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
#include <iostream>

namespace detail {

template<std::ranges::random_access_range R, class Compare>
void insertion_sort(R& rng, Compare comp, 
               std::ranges::iterator_t<R> first, 
               std::ranges::iterator_t<R> last) {
    if (first == last) return;

    auto it = first + 1;

    while (it != last) {
        auto key = std::move(*it);
        auto hole = it;

        while (hole > first && comp(key, *(hole - 1))) {
            *hole = std::move(*(hole - 1));
            --hole;
        }
        *hole = std::move(key);
        ++it;
    }
}

template<std::ranges::random_access_range R, 
         class Compare>
auto partition(R& rng, Compare comp, 
               std::ranges::iterator_t<R> first, 
               std::ranges::iterator_t<R> last)
{
    auto pivot = *(last - 1);
    auto store = first;

    for (auto it = first; it != last - 1; ++it) {
        if (comp(*it, pivot)) {
            std::ranges::iter_swap(it, store);
            ++store;
        }
    }
    std::ranges::iter_swap(store, last - 1);
    return store;
}

template<std::ranges::random_access_range R, class Compare>
void quicksort(R& rng, Compare comp, 
                         std::ranges::iterator_t<R> first, 
                         std::ranges::iterator_t<R> last) 
{
    while(last - first > 16) {
        auto pivot_pos = partition(rng, comp, first, last);

        if (pivot_pos - first < last - pivot_pos - 1) {
            quicksort(rng, comp, first, pivot_pos);
            first = pivot_pos + 1;
        }
        else {
            quicksort(rng, comp, pivot_pos + 1, last);
            last = pivot_pos;
        }
    }
    insertion_sort(rng, comp, first, last);
}

} // namespace detail

template<std::ranges::random_access_range R,
         class Compare = std::less<>>
void quicksort(R& rng, Compare comp = {}) 
{
    if (std::ranges::begin(rng) == std::ranges::end(rng)) return;
    detail::quicksort(rng, comp, std::ranges::begin(rng), std::ranges::end(rng));
}
         
int main()
{
    std::vector<int> v{ 5, 1, 4, 2, 8, 0, 2, 10, 17, 22, 9, 2, 1, 90, 33, 23, 11, 67, 99, 43, 25, 71, -10, -21, -1, 13 };

    quicksort(v);
    for (int x : v) std::cout << x << ' ';
    std::cout << std::endl;

    quicksort(v, std::greater<>{});
    for (int x : v) std::cout << x << ' ';
}
