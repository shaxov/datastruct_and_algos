/**
 * @file	tests/leetcode_705.cpp
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
#include <list>

class MyHashSet {
public:
    MyHashSet() : buckets_(bucket_count_) {}

    void add(int key) {
        int idx = hash(key);
        for (int x : buckets_[idx])
            if (x == key) return;
        buckets_[idx].push_back(key);
    }

    void remove(int key) {
        int idx = hash(key);
        auto &lst = buckets_[idx];
        for (auto it = lst.begin(); it != lst.end(); ++it) {
            if (*it == key) {
                lst.erase(it);
                return;
            }
        }
    }

    bool contains(int key) {
        int idx = hash(key);
        for (int x : buckets_[idx])
            if (x == key) return true;
        return false;
    }
private:
    static constexpr int bucket_count_ = 769;
    std::vector<std::list<int>> buckets_;

    inline int hash(int key) const { return key % bucket_count_; }
};

int main()
{
    MyHashSet* obj = new MyHashSet();
    obj->add(5);
    obj->remove(5);
    bool param_3 = obj->contains(5);
    return 0;
}