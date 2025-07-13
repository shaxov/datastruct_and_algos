/**
 * @file	examples/algorithms/insertion_sort.cpp
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

#include <vector>
#include <ranges>
#include <iostream>

template<std::ranges::random_access_range R,
         class Compare = std::less<>>
constexpr void insertion_sort(R&& rng, Compare comp = {}) {
    using std::ranges::begin;
    using std::ranges::end;

    if (begin(rng) == end(rng)) return;

    auto it = begin(rng) + 1;

    while (it != end(rng)) {
        auto key = std::move(*it);
        auto hole = it;

        while (hole > begin(rng) && comp(key, *(hole - 1))) {
            *hole = std::move(*(hole - 1));
            --hole;
        }
        *hole = std::move(key);
        ++it;
    }
}

int main()
{
    std::vector<int> v{ 5, 1, 4, 2, 8, 0, 2 };

    insertion_sort(v);
    for (int x : v) std::cout << x << ' ';
    std::cout << std::endl;

    insertion_sort(v, std::greater<>{});
    for (int x : v) std::cout << x << ' ';
}