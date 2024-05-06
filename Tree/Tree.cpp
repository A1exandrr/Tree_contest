#include <Tree.h>

// Tree::Iterator

Tree::Iterator::Iterator(Node *node) : _node{node}
{
}

// From Container::Iterator
void *Tree::Iterator::getElement(size_t &amount)
{
    if (nullptr == _node)
        throw NotAvailable{};

    amount = _node->amount;
    return _node->value;
}

bool Tree::Iterator::hasNext()
{
    // ? -- не выяснено, как итерируемся
    return false;
}

void Tree::Iterator::goToNext()
{
    // ? -- не выяснено, как итерируемся
}

// From AbstractTree::Iterator
bool Tree::Iterator::goToParent()
{
    if (nullptr == _node)
        throw InvalidIterator{};

    if (nullptr == _node->parent)
        return false;

    Node *parent = _node->parent;
    _node = parent;
    return true;
}

bool Tree::Iterator::goToChild(int child_index)
{
    if (nullptr == _node)
        throw InvalidIterator{};

    if (_node->children.size() < child_index)
        return false;

    Node *child = _node->children[child_index];
    _node = child;
    return true;
}
