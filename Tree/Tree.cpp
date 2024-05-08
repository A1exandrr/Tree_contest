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

// Tree
Tree::Tree(MemoryManager &manager) : AbstractTree{ manager }, _root{ nullptr }, _size{ 0 }
{}

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
{}

Tree::Iterator *Tree::find(void *elem, size_t size)
{}

Tree::Iterator *Tree::newIterator()
{
    return new Iterator{ _root };
}

void Tree::remove(Container::Iterator *iter)
{
    delete iter;
}

void Tree::clear()
{}

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
    if(nullptr == node)
        throw Tree::InvalidIterator{};
    if(node->children.size() < child_index){
        node->children.resize(child_index, nullptr);
    }

    Node *newNode = static_cast<Node *>(this->_memory.allocMem(sizeof(Node)));
    newNode->parent = node;
    node->value = this->_memory.allocMem(size);
    ::memcpy(node->value, elem, size);
    node->amount = size;

    auto children_iterator = node->children.begin() + child_index;
    if(nullptr == *children_iterator){
        *children_iterator = newNode;
    } else {
        node->children.insert(children_iterator, newNode);
    }

    ++_size;

    return 0;
}

// bool Tree::remove(AbstractTree::Iterator *iter, int leaf_only)
// {
//     if (leaf_only == 1 &&)
// }

/*
Сделать insert и remove
size как дополнение
*/
