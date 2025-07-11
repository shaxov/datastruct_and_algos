#include <memory>
#include <utility>
#include <stdexcept>


template<typename T>
class DoublyLinkedList
{
    struct Node
    {
        T value;
        std::unique_ptr<Node> next;
        Node* prev = nullptr;
        explicit Node(const T& v): value{v} {}
        explicit Node(T&& v): value{std::move(v)} {}
    };
public:
    DoublyLinkedList() noexcept = default;
    ~DoublyLinkedList() = default;


    void push_front(const T& v) { emplace_front(v); }
    void push_front(T&& v) { emplace_front(std::move(v)); }

private:

    template<typename U>
    void emplace_front(U&& v)
    {
        auto node = std::make_unique<Node>(std::forward<U>(v));
        node->next = std::move(head_);
        if (node->next)
            node->next->prev = node.get();
        else
            tail_ = node.get(); // first element
        head_ = std::move(node);
    }

    std::unique_ptr<Node> head_;
    Node* tail_ = nullptr;
    size_t sz = 0;
};

int main()
{
    auto dll = DoublyLinkedList<int>();
    dll.push_front(10);
    dll.push_front(15);
    dll.push_front(3);
    return 0;
}