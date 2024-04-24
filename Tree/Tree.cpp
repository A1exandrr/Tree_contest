#include <Tree.h>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <queue> //необходим для поиска. Реализация позже

using std::cout;
using std::endl;

static int count = 0;

void element() {} // для передачи в insert.Временная заглушка

int Tree::insert(Iterator *iter, int child_index, void *elem, size_t size)
{
    // реализовать вставку
    this->iter = iter;
    child_index = 0;
    this->size = size;
}

bool Tree::remove(Iterator *iter, int leaf_only)
{
    this->iter = iter;
    this->leaf = leaf_only;
}

int Tree::size()
{
    // реализовать подсчет элементов в контейнере
    return count;
}

size_t Tree::max_bytes()
{
    // sizeof(count) / sizeof(*count)
    return count;
}

Tree::Iterator *Tree::find(void *elem, size_t size)
{
    // создать динамеческий локальный итератор указывающий на первый найденный элемент.
    // Удаление пользователем delete
    return 0;
}

Tree::Iterator *Tree::newIterator()
{
    // связана с функцией высше. Указывает на первый элемент. Аналогичная процедура удаления
}

void Tree::remove(Iterator *iter)
{
    // удаление элемента. Next указывает на следующий элемент
}

void Tree::clear() {}

bool Tree::empty() {}

// Реализация методов вложенного класса

bool Tree::TreeIterator::goToParent() {}
bool Tree::TreeIterator::goToChild(int child_index) {}
void (Tree::TreeIterator::*getElement)(size_t &size) {}
bool Tree::TreeIterator::hasNext() {}
void Tree::TreeIterator::goToNext() {}
bool Tree::TreeIterator::equal() {}

// переделать
Tree::TreeDictionary::treeNode *CreateNode(int data)
{

    Tree::TreeDictionary::treeNode *parent_node = new Tree::TreeDictionary::treeNode;
    if (parent_node)
        parent_node->value = data;
    else
        cout << "Error" << endl;
    return parent_node;
}

Tree::TreeDictionary::treeNode *InsertNode(Tree::TreeDictionary::treeNode *parent, Tree::TreeDictionary::treeNode *ChildNode, int data)
{
    if (parent == NULL)
        ChildNode = CreateNode(data);
    else
    {
        Tree::TreeDictionary::treeNode *childNode = CreateNode(data);
        parent->children.push_back(childNode);
        return childNode;
    }
    return ChildNode;
}

Tree::TreeDictionary::treeNode *SearchElement(Tree::TreeDictionary::treeNode *NextNode, int *data)
{
    // Реализовать или поиск(глубину или ширину)
    return NextNode;
}