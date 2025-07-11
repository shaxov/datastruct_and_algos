/**
 * @file	examples/data_structures/doubly_linked_list.cpp
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
#include <utility>
#include <iostream>
#include <stdexcept>

template<typename T>
class DoublyLinkedList
{
    struct Node
    {
        T value;
        std::unique_ptr<Node> next;
        Node* prev = nullptr;
        explicit Node(T const& v): value{v} {}
        explicit Node(T&& v): value{std::move(v)} {}
    };
public:

    size_t size() const noexcept { return sz_; }
    bool empty() const noexcept { return sz_ == 0; }

    T& front() { requires_not_empty(); return head_->value; }
    T const& front() const { requires_not_empty(); return head_->value; }

    T& back() { requires_not_empty(); return tail_->value; }
    T const& back() const { requires_not_empty(); return tail_->value; }

    void push_front(const T& v) { emplace_front(v); }
    void push_front(T&& v) { emplace_front(std::move(v)); }

    void push_back(const T& v) { emplace_back(v); }
    void push_back(T&& v) { emplace_back(std::move(v)); }

    void pop_front() {
        requires_not_empty();
        head_ = std::move(head_->next);
        if (head_)
            head_->prev = nullptr;
        else
            tail_ = nullptr;
        --sz_;
    }

    void pop_back() {
        requires_not_empty();
        if (tail_->prev) {
            tail_ = tail_->prev;
            tail_->next.release();
        }
        else {
            head_.release();
            tail_ = nullptr;
        }
    }

private:

    void requires_not_empty() {
        if (!tail_)
            throw std::out_of_range("list is empty");
    }

    template<typename U>
    void emplace_front(U&& v) {
        auto node = std::make_unique<Node>(std::forward<U>(v));
        node->next = std::move(head_);
        if (node->next)
            node->next->prev = node.get();
        else
            tail_ = node.get(); // first element
        head_ = std::move(node);
        ++sz_;
    }

    template<typename U>
    void emplace_back(U&& v) {
        auto node = std::make_unique<Node>(std::forward<U>(v));
        if(tail_) {
            node->prev = tail_;
            tail_->next = std::move(node);
            tail_ = tail_->next.get();
        }
        else {
            tail_ = node.get();
            head_ = std::move(node);
        }
        sz_++;
    }

    std::unique_ptr<Node> head_;
    Node* tail_ = nullptr;
    size_t sz_ = 0;
};

int main()
{
    auto dll = DoublyLinkedList<int>();

    // dll.pop_front();

    dll.push_front(10);
    dll.push_front(15);
    dll.push_front(3);

    dll.push_back(4);
    dll.push_back(8);

    dll.pop_front();

    dll.pop_back();

    std::cout << dll.front() << std::endl;
    std::cout << dll.back() << std::endl;
    return 0;
}