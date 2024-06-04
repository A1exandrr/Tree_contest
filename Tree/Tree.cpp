#include <Tree.h>

#include <iostream>
#include <limits>

// -- Tree::Node

size_t Tree::Node::removeChildren(MemoryManager &manager)
{
    size_t amountOfRemoved = 0;
    for (std::vector<Node *>::iterator it = children.begin(); it != children.end(); ++it)
    {
        Node *child = *it;
        if (child == nullptr)
            continue;

        amountOfRemoved += child->removeChildren(manager);
        if (child->dataValue)
        {
            manager.freeMem(child->dataValue);
            child->dataValue = nullptr;
            child->dataSize = 0;
        }
        child->~Node();
        manager.freeMem(child);
        ++amountOfRemoved;

        *it = nullptr;
    }
    children.clear();
    return amountOfRemoved;
}

Tree::Node *Tree::Node::find(void *data, size_t size)
{
    // сравнение размеров
    // memcmp() возвращает 0 при условии, что данные заданного размера совпадают
    if ((dataSize == size) && (memcmp(dataValue, data, dataSize) == 0))
        return this; // возврат самого себя, если всё совпало

    Node *node = nullptr;
    for (std::vector<Node *>::iterator it = children.begin(); it != children.end(); ++it)
    {
        Node *child = *it;
        if (child == nullptr)
            continue;

        node = child->find(data, size);
        if (nullptr != node)
            return node;
    }
    return nullptr;
}

bool Tree::Node::isLeaf()
{
    if (children.empty())
        return true;

    for (std::vector<Node *>::iterator it = children.begin(); it != children.end(); ++it)
    {
        Node *child = *it;
        if (nullptr != child)
            return false;
    }
    return true;
}

size_t Tree::Node::indexOfNextChild(Node *node)
{
    size_t index = 0;

    std::vector<Node *>::iterator it = children.begin();
    // Ищем текущий узел
    for (; it != children.end() && *it != node; ++it, ++index)
    {}
    ++it, ++index;
    // Ищем следующий
    for (; it != children.end(); ++it, ++index)
    {
        if (*it != nullptr)
            return index;
    }
    // Мы есть последний узел
    if (it == children.end())
        return std::numeric_limits<size_t>::max();
    return index;
}

Tree::Node *Tree::Node::getFirst()
{
    if (children.empty())
        return this;

    for (std::vector<Node *>::iterator it = children.begin();
            it != children.end(); ++it)
    {
        if (nullptr != *it)
            return (*it)->getFirst();
    }
    return this;
}

// Tree::Iterator

Tree::Iterator::Iterator(const Node *root, Node *node) : _root{root}, _node{node}
{}

// -- From Container::Iterator

//
//       -------- root ---------
//      /      |        |       \
//     /       |        |        \
//    4        8       11        15
//  / | \    / | \    /  \     /  | \
// 1  2  3  5  6  7  9   10   /   |  \
//                           12  13  14
//

void *Tree::Iterator::getElement(size_t &amount)
{
    if (nullptr == _node)
        return nullptr;

    amount = _node->dataSize;
    return _node->dataValue;
}

bool Tree::Iterator::hasNext()
{
    if ((nullptr == _node) || (_root == _node))
        return false;

    size_t index = _node->parent->indexOfNextChild(_node);
    if ((_root == _node->parent) && (std::numeric_limits<size_t>::max() == index))
        return false;
    // if (_node->parent->children[index] == _node)
    //     return false;

    return true;
}

void Tree::Iterator::goToNext()
{
    if (false == hasNext())
        return;

    size_t index = _node->parent->indexOfNextChild(_node);
    if (std::numeric_limits<size_t>::max() == index)
    {
        _node = _node->parent;
        return;
    }
    _node = _node->parent->children[index]->getFirst();
}

bool Tree::Iterator::equals(Container::Iterator *iterator)
{
    if (nullptr == iterator)
        throw InvalidParameters{};

    if (this == iterator)
        return true;

    Tree::Iterator *inputIterator = dynamic_cast<Tree::Iterator *>(iterator);
    if (!inputIterator)
        throw InvalidParameters{};

    return _node == inputIterator->_node;
}

// -- From AbstractTree::Iterator

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
{}

Tree::~Tree()
{
    _root.removeChildren(this->_memory);
}

// -- From Container

int Tree::size()
{
    return _size;
}

size_t Tree::max_bytes()
{
    return std::numeric_limits<std::size_t>::max();
}

Container::Iterator *Tree::find(void *elem, size_t size)
{
    if (0 == _size) // если контейнер пуст
        return nullptr;

    if ((nullptr == elem) || (0 == size))
        throw Tree::InvalidParameters{};

    Node *node = _root.find(elem, size);
    if (nullptr == node)
        return nullptr;

    return createIterator(node);
}

Container::Iterator *Tree::newIterator()
{
    // Противоречие требований:
    // -- Контейнер пустой - возвращается nullptr
    // -- Вставка в контейнер по итератору и индексу дочерней вершины
    // if (true == empty())
    //     return nullptr;

    return createIterator(_root.getFirst());
}

void Tree::remove(Container::Iterator *iter)
{
    Iterator *inputIterator = nullptr;
    try
    {
        inputIterator = convert(iter);
    }
    catch (const Container::Error &error)
    {
        std::cerr << "Tree::remove(iter): cannot convert iterator: error = '"
                << error.msg << "'" << std::endl;
    }
    remove(inputIterator, 0);
}

void Tree::clear()
{
    if (true == empty())
        return;

    _size -= _root.removeChildren(this->_memory);
    _root.children.clear();
}

bool Tree::empty()
{
    return size() == 0;
}

// -- From AbstactTree

int Tree::insert(AbstractTree::Iterator *iter, int child_index, void *elem, size_t size)
{
    const int SUCCESS_RESULT = 0;
    const int FAILED_RESULT = 1;

    Iterator *inputIterator = nullptr;
    try
    {
        inputIterator = convert(iter);
    }
    catch (const Container::Error &error)
    {
        std::cerr << "Tree::insert(): cannot convert iterator: error = '"
                << error.msg << "'" << std::endl;
        return FAILED_RESULT;
    }

    Node *node = inputIterator->getCurrentNode();
    if (nullptr == node)
        throw Tree::InvalidIterator{};
    if (node->children.size() < child_index + 1)
    {
        node->children.resize(child_index + 1, nullptr);
    }

    Node *newNode = static_cast<Node *>(this->_memory.allocMem(sizeof(Node)));
    new(newNode) Node{};

    newNode->parent = node;
    newNode->dataValue = this->_memory.allocMem(size);
    ::memcpy(newNode->dataValue, elem, size);
    newNode->dataSize = size;

    auto children_iterator = node->children.begin() + child_index;
    if (nullptr == *children_iterator)
        *children_iterator = newNode;
    else
        node->children.insert(children_iterator, newNode);

    ++_size;

    return SUCCESS_RESULT;
}

bool Tree::remove(AbstractTree::Iterator *iter, int leaf_only)
{
    if (nullptr == iter)
        throw InvalidIterator{};

    Iterator *inputIterator = nullptr;
    try
    {
        inputIterator = convert(iter);
    }
    catch (const Container::Error &error)
    {
        std::cerr << "Tree::remove(): cannot convert iterator: error = '"
                << error.msg << "'" << std::endl;
        return false;
    }

    if (empty() || (&_root == inputIterator->getCurrentNode()))
        return false;

    Node *node = inputIterator->getCurrentNode();
    if (nullptr == node)
        throw InvalidIterator{};

    bool isLeaf = node->isLeaf();
    if ((1 == leaf_only) && (false == isLeaf))
        return false;

    // Даже если удалится узел с потомками, итератор перескочит на соседний
    bool hasNext = inputIterator->hasNext();
    if (hasNext)
        inputIterator->goToNext();

    if (!isLeaf)
        _size -= node->removeChildren(_memory);

    // Ищем этого потомка в узле выше
    std::vector<Node *> &parentChildren = node->parent->children;
    std::vector<Node *>::const_iterator itChild = parentChildren.cbegin();
    for (; itChild != parentChildren.end() && *itChild != node; ++itChild)
    {}

    // Исключительная ситуация
    if (itChild == parentChildren.end())
        throw InvalidIterator{};

    parentChildren.erase(itChild);
    if (node->dataValue)
    {
        _memory.freeMem(node->dataValue);
        node->dataValue = nullptr;
        node->dataSize = 0;
    }
    node->~Node();
    _memory.freeMem(node);
    --_size;

    // Если удалили последний элемент, ищем другой последний элемент
    if (!hasNext)
    {
        // reverse_iterator -- итератор, идущий от последнего элемента к первому
        // rbegin()/rend() возвращают, соответственно, реверсивный начальный и конечный итераторы
        for (std::vector<Node *>::reverse_iterator it = _root.children.rbegin();
                it != _root.children.rend(); ++it)
        {
            if (*it != nullptr)
            {
                new (inputIterator) Iterator{&_root, *it}; // пересоздаём итератор по адресу старого
                break;
            }
        }
    }

    return true;
}

// private

Tree::Iterator *Tree::convert(Container::Iterator *iter)
{
    if (nullptr == iter)
        throw InvalidParameters{};
    Iterator *inputIter = dynamic_cast<Iterator*>(iter);
    if(nullptr == inputIter)
        throw InvalidIterator{};
    return inputIter;
}

Tree::Iterator *Tree::createIterator(Tree::Node *node)
{
    return new Iterator{&_root, node};
}
