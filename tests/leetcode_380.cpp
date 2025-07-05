/**
 * @file	tests/leetcode_380.cpp
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
#include <random>
#include <unordered_map>

class RandomizedSet {
public:
    RandomizedSet() noexcept = default;

    bool insert(int val) {
        if (index_.count(val))
            return false;
        index_[val] = data_.size();
        data_.push_back(val);
        return true;
    }

    bool remove(int val) {
        auto it = index_.find(val);
        if (it == index_.end())
            return false;

        auto pos = it->second;
        int last = data_.back();

        data_[pos] = last;
        index_[last] = pos;

        data_.pop_back();
        index_.erase(it);
        return true;
    }

    int getRandom() {
        std::uniform_int_distribution<size_t> dist(0, data_.size() - 1);
        return data_[dist(rng_)];
    }

private:
    std::vector<int> data_;
    std::unordered_map<int, size_t> index_; // val to pos
    std::mt19937 rng_{std::random_device{}()};
};

#include <iostream>

int main()
{
    auto rs = RandomizedSet();
    rs.insert(5);
    rs.insert(10);
    rs.insert(13);
    std::cout << rs.getRandom() << std::endl;
    std::cout << rs.remove(1) << std::endl;
    std::cout << rs.remove(5) << std::endl;
    std::cout << rs.getRandom() << std::endl;
    return 0;
}