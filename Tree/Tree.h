#pragma once

#include <TreeAbstract.h>
#include <MemoryManager.h>

#include <vector>

class Tree : public AbstractTree
{
    struct Node
    {
        std::vector<Node *> children = {};
        Node *parent = nullptr;

        void *value = nullptr;
        std::size_t amount = 0;
    };

public:
    class Iterator : public AbstractTree::Iterator
    {
    public:
        Iterator(Node *node);
        ~Iterator() = default;

        // From Container::Iterator
        void *getElement(size_t &size);
        bool hasNext();
        void goToNext();
        bool equals(Container::Iterator *right);

        // From AbstractTree::Iterator
        bool goToParent();
        bool goToChild(int child_index);
        Node *getCurrentNode(){
            return _node;
        }

    private:
        Node *_node;
    };
    // Exceptions classes
    class NotAvailable : public Container::Error
    {
    public:
        NotAvailable() : Container::Error{"Element is not available"} {}
    };

    class InvalidIterator : public Container::Error
    {
    public:
        InvalidIterator() : Container::Error{"Iterator is not valid"} {}
    };



public:
    Tree(MemoryManager &manager);
    ~Tree();

    // From Container
    int size();
    size_t max_bytes();
    Iterator *find(void *elem, size_t size);
    Iterator *newIterator();
    void remove(Container::Iterator *iter);
    void clear();
    bool empty();

    // From AbstractTree
    int insert(AbstractTree::Iterator *iter, int child_index, void *elem, size_t size);
    bool remove(AbstractTree::Iterator *iter, int leaf_only);
    
private:
    // количество детей заведомо неизвественно
    Node *_root;
    int _size;
};
