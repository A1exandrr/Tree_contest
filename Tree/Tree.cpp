#include <Tree.h>

// Tree::Node

void Tree::Node::removeChildren(MemoryManager &manager)
{
    for (std::vector<Node *>::iterator it = children.begin(); it != children.end(); ++it)
    {
        Node *child = *it;
        if (child == nullptr)
            continue;

        child->removeChildren(manager);
        if (child->value)
        {
            manager.freeMem(child->value);
            child->value = nullptr;
            child->amount = 0;
        }
        child->~Node();
        manager.freeMem(child);

        *it = nullptr;
    }
}

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

bool Tree::Iterator::equals(Container::Iterator *)
{
    return false;
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

// Tree
Tree::Tree(MemoryManager &manager) : AbstractTree{manager}, _root{}, _size{0}
{
}

Tree::~Tree()
{
    // TODO
}

// From Container
int Tree::size()
{
    return _size;
}

size_t Tree::max_bytes() // ???
{
}

Tree::Iterator *Tree::find(void *elem, size_t size)
{
}

Tree::Iterator *Tree::newIterator()
{
    return new Iterator{&_root};
}

void Tree::remove(Container::Iterator *iter)
{
    if (nullptr == iter)
        throw InvalidIterator{};
    Iterator *inputIter = dynamic_cast<Iterator*>(iter);
    if(nullptr == inputIter)
        throw InvalidIterator{};
    Node *newNode = inputIter->getCurrentNode();
    if(nullptr == newNode)
        throw InvalidIterator{};

    newNode->removeChildren(this->_memory);
    // Перейти в parent и найти ячейку в children, которая указывает на этот Node
    Node *parent = newNode->parent;
    if (nullptr != parent)
    {
        for (std::vector<Node *>::iterator it = parent->children.begin(); it != parent->children.end(); ++it)
            if (*it == newNode)
            {
                *it = nullptr;
                break;
            }
    }
    if (&_root != newNode)
    {
        newNode->~Node();
        this->_memory.freeMem(newNode);
    }
}

void Tree::clear()
{
}

bool Tree::empty()
{
    return size() == 0;
}

// From AbstactTree
int Tree::insert(AbstractTree::Iterator *iter, int child_index, void *elem, size_t size)
{
    if (nullptr == iter)
        throw Tree::InvalidIterator{};

    Iterator *my_iterator = dynamic_cast<Iterator *>(iter);
    if (nullptr == my_iterator)
        throw Tree::InvalidIterator{};

    Node *node = my_iterator->getCurrentNode();
    if (nullptr == node)
        throw Tree::InvalidIterator{};
    if (node->children.size() < child_index)
    {
        node->children.resize(child_index, nullptr);
    }

    Node *newNode = static_cast<Node *>(this->_memory.allocMem(sizeof(Node)));
    new(newNode) Node{};

    newNode->parent = node;
    newNode->value = this->_memory.allocMem(size);
    ::memcpy(newNode->value, elem, size);
    newNode->amount = size;

    auto children_iterator = node->children.begin() + child_index;
    if (nullptr == *children_iterator)
    {
        *children_iterator = newNode;
    }
    else
    {
        node->children.insert(children_iterator, newNode);
    }

    ++_size;

    return 0;
}

bool Tree::remove(AbstractTree::Iterator * /*iter*/, int /*leaf_only*/)
{
    return false;
}

/*
Сделать insert и remove
size как дополнение
*/
