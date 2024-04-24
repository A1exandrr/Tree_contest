#pragma once
#include <TreeAbstract.h>
#include <MemoryManager.h>

class Tree : public AbstractTree
{
private:
    // MemoryManager &_memory;

public:
    Tree(/* args */);
    ~Tree();

    // Tree::Container(MemoryManager &mem) : _memory(mem);
    // Разобраться потом с этимм. Надо реализовать конструктор для контейнера

    //============================= [ Functions ] =============================
    int insert(Iterator *iter, int child_index, void *elem, size_t size);
    bool remove(Iterator *iter, int leaf_only);
    int size();
    size_t max_bytes();
    Iterator *find(void *elem, size_t size);
    Iterator *newIterator();
    void remove(Iterator *iter);
    void clear();
    bool empty();
    //=========================================================================

public:
    class TreeIterator : public AbstractTree::Iterator
    {
    public:
        // constructors
        TreeIterator();
        ~TreeIterator();

        //============================= [ Functions ] =============================
        bool goToParent();
        bool goToChild(int child_index);
        void *getElement(size_t &size);
        bool hasNext();
        void goToNext();
        bool equal();
        //=========================================================================
    };

public:
    class TreeDictionary
    {

    private:
        struct TreeNode
        {
            vector<TreeNode *> children;

            int value;
        };
        TreeNode *root;

    public:
        typedef TreeNode treeNode;

    public:
        TreeDictionary(int value)
        {
            root = new treeNode();
            root->value = value;
        }
        ~TreeDictionary() {}

        virtual treeNode *CreateNode(int data) = 0;
        virtual treeNode *InsertNode(treeNode *parent, treeNode *ChildNode, int data) = 0;
        virtual treeNode *SearchElement(treeNode *parent, treeNode *ChildNode, int data) = 0;
    };

    // Добавить узконаправленные классы для улучшения итерации по дереву. Написать тесты
    // реализовтаь свой memorymanager
    // Tree::Tree(/* args */)
    // {
    // }

    // Tree::~Tree()
    // {
    // }

    // Tree::TreeIterator::TreeIterator()
    // {
    // }

    // Tree::TreeIterator::~TreeIterator()
    // {
    // }
