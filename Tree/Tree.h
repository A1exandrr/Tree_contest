#pragma once

#include <TreeAbstract.h>
#include <MemoryManager.h>

#include <vector>

class Tree : public AbstractTree
{
    struct Node
    {
        Node(std::size_t amountOfChildren);

        std::vector<Node *> children = {};
        Node *parent = nullptr;

        void *value = nullptr;
        std::size_t size = 0;
    };

public:
    class Iterator: public AbstractTree::Iterator
    {
    public:
        Iterator(Node *node);
        ~Iterator() = default;

        // From Container::Iterator
        void *getElement(size_t &size);
        bool hasNext();
        void goToNext();

        // From AbstractTree::Iterator
        bool goToParent();
        bool goToChild(int child_index);

    private:
        Node *_node;
    };

public:
    Tree(MemoryManager &manager, size_t amountOfChildren);
    ~Tree();

    // From Container

    // From AbstractTree

private:
    const std::size_t _amountOfChildren;
    Node *_root;
};
