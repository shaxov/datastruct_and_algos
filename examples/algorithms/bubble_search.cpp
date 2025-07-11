#include <ranges>
#include <utility>
#include <iostream>

template<std::ranges::random_access_range R,
         class Compare = std::less<>>
constexpr void bubble_sort(R&& rng, Compare comp = {})
{
    auto last = std::ranges::end(rng);
    bool swapped = true;

    while (swapped) {
        swapped = false;

        for (auto it = std::ranges::begin(rng); it < last - 1; ++it) {
            if (comp(*it, *(it + 1))) {
                std::ranges::iter_swap(it, it + 1);
                swapped = true;
            }
        }
        --last;
    }
}


int main()
{
    std::vector<int> v{ 5, 1, 4, 2, 8, 0, 2 };

    bubble_sort(v);
    for (int x : v) std::cout << x << ' ';
    std::cout << std::endl;

    bubble_sort(v, std::greater<>{});
    for (int x : v) std::cout << x << ' ';
}