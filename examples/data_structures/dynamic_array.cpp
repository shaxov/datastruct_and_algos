#include <iostream>
#include <memory>
#include <iterator>
#include <algorithm>


template<typename T>
class DynamicArray
{
public:
    // empty ctor (explicit definition since other ctors are defined)
    DynamicArray() noexcept = default;

    // explicit to avoid implicit conversion from int
    explicit DynamicArray(size_t n, const T& value = {}) 
        : sz_{n}, 
          cap_{n},
          data_{std::make_unique<T[]>(n)}
    {
        std::fill_n(data_.get(), n, value);
    }

    // copy ctor
    DynamicArray(const DynamicArray& other)
        : DynamicArray(other.begin(), other.end())
    { }

    // move ctor (explicit definition since copy ctor is defined)
    // bit-to-bit move (or copy if not movable like sz_ and cap_)
    DynamicArray(DynamicArray&&) noexcept = default;

    // copy assign
    DynamicArray& operator=(const DynamicArray& other) 
    {
        sz_ = other.sz_;
        cap_ = other.cap_;
        data_ = std::make_unique<T[]>(sz_);
        std::copy(other.begin(), other.end(), data_.get());
        return *this;
    }

    // move assign
    DynamicArray& operator=(DynamicArray&&) noexcept = default;

    // range-based ctor
    template<std::input_iterator It>
    DynamicArray(It first, It last)
    {
        sz_ = cap_ = static_cast<size_t>(std::distance(first, last));
        data_ = std::make_unique<T[]>(sz_);
        std::copy(first, last, data_.get());
    }

    T& operator[](size_t i) { return data_[i]; }
    const T& operator[](size_t i) const { return data_[i]; }

    void push_back(const T& value)
    {
        ensure_capacity(sz_ + 1);
        data_[sz_++] = value; // copy
    }

    void push_back(T&& value)
    {
        ensure_capacity(sz_ + 1);
        data_[sz_++] = std::move(value); // move
    }

    T* begin() { return data_.get(); }
    const T* begin() const { return data_.get(); }
    T* end() { return data_.get() + sz_; }
    const T* end() const { return data_.get() + sz_; }

    size_t size() const noexcept { return sz_; }
    bool empty() const noexcept { return sz_ == 0; }

private:
    void ensure_capacity(size_t new_cap)
    {
        if (new_cap < cap_) return;
        cap_ = std::max(new_cap, cap_ * 2);
        auto tmp = std::make_unique<T[]>(cap_);
        std::move(begin(), end(), tmp.get());
        data_.swap(tmp);
    }

    size_t sz_;
    size_t cap_;
    std::unique_ptr<T[]> data_;
};

template<typename T>
void print(DynamicArray<T>& array)
{   
    std::cout << "[";
    for (auto a : array)
        std::cout << a << ", ";
    std::cout << "]\n";
}

int main()
{   
    auto array = DynamicArray<int>();
    array.push_back(5);
    array.push_back(4);
    array.push_back(7);
    print(array);

    auto array2 = DynamicArray<int>(5, 42);
    print(array2);

    auto array3 = DynamicArray<int>(array);
    print(array3);

    auto array4 = std::move(array2);
    print(array4);
    std::cout << array2.size() << " " << (array2.begin() == nullptr) << std::endl;

    auto array5 = DynamicArray<int>(array4.begin(), array4.end());
    print(array5);

    array = array5;
    print(array);
    array = std::move(array5);
    print(array);

    return 0;
}
