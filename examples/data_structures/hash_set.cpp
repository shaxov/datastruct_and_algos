/**
 * @file	examples/data_structures/hash_set.cpp
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

#include <list>
#include <vector>
#include <string>
#include <iostream>
#include <stdexcept>
#include <functional>
#include <type_traits>

template<
    typename Key,
    typename Hash = std::hash<Key>,
    typename KeyEqual = std::equal_to<Key>>
class HashSet {
public:
    explicit HashSet(
        size_t bucket_count = bucket_count_,
        const Hash& hasher = Hash(),
        const KeyEqual& equal = KeyEqual())
         : buckets_(bucket_count),
           hasher_(hasher),
           equal_(equal)
    { }

    HashSet(const HashSet& other) noexcept = default;
    HashSet(HashSet&&) noexcept = default;

    HashSet& operator=(const HashSet& other) noexcept = default;
    HashSet& operator=(HashSet&&) noexcept = default;
    
    void add(const Key& key)
    {
        auto& lst = buckets_[hash(key)]; 
        for (auto key_ : lst)
            if (equal_(key, key_))
                return;
        lst.push_back(key);
    }
    
    void remove(const Key& key) 
    {
        auto& lst = buckets_[hash(key)]; 
        for (auto it = lst.begin(); it != lst.end(); ++it)
        {
            if (equal_(*it, key))
            {
                lst.erase(it);
                return;
            }
        }
    }
    
    bool contains(const Key& key) const 
    {
        auto& lst = buckets_[hash(key)];
         for (auto key_ : lst)
            if (equal_(key, key_))
                return true;
        return false;
    }

private:
    static constexpr size_t bucket_count_ = 1069;
    std::vector<std::list<Key>> buckets_;
    Hash hasher_;
    KeyEqual equal_;

    inline size_t hash(const Key& key) const
    {
        return hasher_(key) % buckets_.size(); 
    }
};


int main()
{  
    auto hashset = HashSet<int>();
    hashset.add(2);
    hashset.add(3);
    hashset.add(2);
    hashset.add(7);

    std::cout << hashset.contains(2) << std::endl;
    hashset.remove(2);
    std::cout << hashset.contains(2) << std::endl;
    std::cout << hashset.contains(3) << std::endl;
    std::cout << hashset.contains(4) << std::endl;
    std::cout << hashset.contains(7) << std::endl;

    auto hashset2 = HashSet<std::string>();
    hashset2.add("Hello");
    hashset2.add("wolrd");
    hashset2.add("!");

    std::cout << hashset2.contains("Hello") << std::endl;
    hashset2.remove("Hello");
    std::cout << hashset2.contains("Hello") << std::endl;
    std::cout << hashset2.contains("wolrd") << std::endl;
    std::cout << hashset2.contains("?") << std::endl;

    auto hashset3 = HashSet<int>(hashset);
    std::cout << hashset3.contains(3) << std::endl;

    auto hashset4 = HashSet<int>(std::move(hashset3));

    hashset = hashset4;
    std::cout << hashset4.contains(3) << std::endl;

    hashset = std::move(hashset4);
    // std::cout << hashset4.contains(3) << std::endl; // segmentation fault
    return 0;
}