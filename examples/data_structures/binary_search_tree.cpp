#include <ranges>
#include <memory>
#include <utility>
#include <functional>
#include <iostream>


template<typename Key, typename Compare = std::less<>>
class BinarySearchTree
{
    struct Node
    {
        Key key;
        std::unique_ptr<Node> left, right;
        explicit Node(Key const& k) : key{k} { }
        explicit Node(Key&& k) : key{std::move(k)} { }
    };

public:
    bool contains(Key const& key) const { return find_node(key) != nullptr; }
    void insert(Key const& key) { insert_impl(root_, key); }
    void insert(Key&& key) { insert_impl(root_, std::move(key)); }
    bool erase(Key const& key) { return erase_impl(root_, key); }

    void inorder(std::ostream& os = std::cout) { inorder_impl(root_.get(), os); os << '\n'; }

private:

    Node const* find_node(Key const& key) const {
        Node* cur { root_.get() };
        while (cur)
        {
            if (comp_(key, cur->key)) cur = cur->left.get();
            else if (comp_(cur->key, key)) cur = cur->right.get();
            else return cur;
        }
        return nullptr;
    }

    template<typename K>
    void insert_impl(std::unique_ptr<Node>& node, K&& k) {
        if (!node) {
            node = std::make_unique<Node>(std::forward<K>(k));
            return;
        }
        if (comp_(k, node->key))
            insert_impl(node->left, std::forward<K>(k));
        else if (comp_(node->key, k))
            insert_impl(node->right, std::forward<K>(k));
        else {
            // ignore equal elements
        }
    }

    void inorder_impl(Node const* node, std::ostream& os) {
        if (!node) return;
        inorder_impl(node->left.get(), os);
        os << node->key << ' ';
        inorder_impl(node->right.get(), os);
    }

    bool erase_impl(std::unique_ptr<Node>& node, Key const& key) {
        if (!node) return false;

        if (comp_(key, node->key)) 
            return erase_impl(node->left, key);
        else if (comp_(node->key, key)) 
            return erase_impl(node->right, key);
        else {
            if (!node->left) 
                node = std::move(node->right);
            else if (!node->right) 
                node = std::move(node->left);
            else {
                Node* min_parent = node.get();
                auto* min_ptr = &node->right;
                while((*min_ptr)->left) {
                    min_parent = min_ptr->get();
                    min_ptr = &(*min_ptr)->left;
                }
                node->key = std::move((*min_ptr)->key);
                // remove min in right subtree
                if (min_parent == node.get()) // no left subtree
                    node->right = std::move((*min_ptr)->right);
                else
                    min_parent->left = std::move((*min_ptr)->right);
            }
            return true;
        }
        
    }

    std::unique_ptr<Node> root_;
    Compare comp_;
};

int main() {
    BinarySearchTree<int> bst;
    for (int x : {5, 2, 8, 1, 4, 7, 9}) bst.insert(x);

    std::cout << "inorder: ";
    bst.inorder();                     // 1 2 4 5 7 8 9

    bst.erase(5);
    std::cout << "after erase(5): ";
    bst.inorder();                     // 1 2 4 7 8 9

    std::cout << "contains 4? " << std::boolalpha << bst.contains(4) << '\n';
    std::cout << "contains 10? " << std::boolalpha << bst.contains(10) << '\n';
}