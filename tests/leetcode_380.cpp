#include <vector>
#include <unordered_map>
#include <random>

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