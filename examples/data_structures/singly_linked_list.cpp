/**
 * @file	examples/data_structures/singly_linked_list.cpp
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


#include <memory>
#include <iostream>
#include <stdexcept>

template<typename T>
class SinglyLinkedList
{
    struct Node 
    {
        T value;
        std::unique_ptr<Node> next;
        Node(T const& v, std::unique_ptr<Node> nx = nullptr) 
            : value{v}, next{std::move(nx)} 
            {}
    };

public:
    void push_front(T const& v)
    {
        head_ = std::make_unique<Node>(v, std::move(head_));
        ++sz_;
    }

    void push_back(T const& v)
    {
        auto new_node = std::make_unique<Node>(v);
        if (!head_)
        {
            head_ = std::move(new_node);
        }
        else
        {
            Node* cur = head_.get();
            while (cur->next)
                cur = cur->next.get();
            cur->next = std::move(new_node);
        }
        ++sz_;
    }

    void pop_front()
    {
        if (!head_)
            throw std::out_of_range("list is empty");
        head_ = std::move(head_->next);
        --sz_;
    }

    T const& front() const { return head_->value; }
    Node const* front_p() const { return head_.get(); }

    size_t size() const { return sz_; }
    bool empty() const { return !head_; }

private:
    std::unique_ptr<Node> head_;
    size_t sz_ = 0;
};

template<typename T>
void print(const SinglyLinkedList<T>& list)
{
    auto* cur = list.front_p();
    std::cout << "[" << cur->value << ", ";
    while (cur->next)
    {
        cur = cur->next.get();
        std::cout << cur->value << ", ";
    }
    std::cout << "]" << std::endl;
}

int main()
{
    auto sll = SinglyLinkedList<int>();
    sll.push_front(5);
    print(sll);
    sll.push_front(7);
    print(sll);
    sll.push_back(10);
    print(sll);
    sll.push_back(12);
    print(sll);
    sll.pop_front();
    print(sll);
    std::cout << sll.size() << std::endl;
    std::cout << sll.front() << std::endl;
    std::cout << sll.empty() << std::endl;
    return 0;
}