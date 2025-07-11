#include <memory>
#include <stdexcept>


template<typename T>
class DoublyLinkedList
{
    struct Node
    {
        T value;
        std::unique_ptr<Node> next;
        Node* prev;
        Node(const T& v)v: value{v} {}
    };
public:
    DoublyLinkedList() noexcept = default;
    ~DoublyLinkedList() = default;

    void push_front(const T& v)
    {
    }

private:
    std::unique_ptr<Node> head_;
    size_t sz = 0;
};