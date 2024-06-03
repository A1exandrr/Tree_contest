#pragma once

#include <TreeAbstract.h>
#include <MemoryManager.h>

#include <vector>

class Tree : public AbstractTree
{
    struct Node
    {
        std::vector<Node *> children = {1, nullptr};
        Node *parent = nullptr;

        void *dataValue = nullptr;
        size_t dataSize = 0;

        // Удалить все нижележащие узлы
        size_t removeChildren(MemoryManager &manager);

        // Найти узел по данным
        Node *find(void *data, size_t size);

        // Является ли узел листом
        bool isLeaf();

        // Получить индекс следущего узла по указатетелю текущего узла
        // Если не получается, возвращается std::numeric_limits<size_t>::max()
        size_t indexOfNextChild(Node *node);

        // Получить самый крайний ("левый") узел
        Node *getFirst();
    };

public:
    class Iterator;

    // Exceptions classes
    class NotAvailable;
    class InvalidIterator;
    class InvalidParameters;

public:
    Tree(MemoryManager &manager);
    ~Tree();

    // -- From Container

    // Функция возвращает значение, равное количеству элементов в контейнере.
    int size() override;

    // Функция возвращает значение, равное максимальной вместимости контейнера в байтах.
    size_t max_bytes() override;

    // Функция создает в динамической памяти итератор, указывающий на первый найденный
    // в контейнере элемент. Если элемент не найден, возвращается пустой указатель.
    // Удаление этого итератора должно делаться пользователем с помощью оператора delete.
    Container::Iterator *find(void *elem, size_t size) override;

    // Функция создает в динамической памяти итератор, указывающий на первый элемент
    // контейнера. Если контейнер пустой, возвращается нулевой указатель.
    // Удаление этого итератора должно делаться пользователем с помощью оператора delete.
    Container::Iterator *newIterator() override;

    // Удаление элемента из позиции, на которую указывает итератор iter.
    // После удаления итератор указывает на следующий за удаленным элемент.
    void remove(Container::Iterator *iter) override;

    // Удаление всех элементов из контейнера.
    void clear() override;

    // Если контейнер пуст возвращает true, иначе false
    bool empty() override;

    // -- From AbstractTree

    // Добавление элемента как дочернюю вершину № child_index вершины, на которую
    // указывает итератор. В случае успешного добавления функция возвращает значение 0, в случае неудачи 1.
    int insert(AbstractTree::Iterator *iter, int child_index, void *elem, size_t size) override;

    // Удаление вершины, на которую указывает итератор. Если leaf_only==1 и вершина не является листом, возвращает false
    // !!! Примечание: метод remove удаляет вершину вместе со всеми ее потомками
    bool remove(AbstractTree::Iterator *iter, int leaf_only) override;

private:
    // Преобразовать указатель на итератор абстрактного контейнера в указатель на реализацию итератора дерева
    // Если указатель nullptr, выбросится исключение InvalidParameters
    // Если этого не удалось сделать, выбросится исключение InvalidIterator
    Iterator *convert(Container::Iterator *iter);

    // Создать итератор с указателем на конкретную вершину
    Iterator *createIterator(Node *node);

    Node _root; // родительская вершина
    int _size; // количество элементов в дереве
};

// Node

struct Node
{
    std::vector<Node *> children = {1, nullptr};
    Node *parent = nullptr;

    void *value = nullptr;
    std::size_t amount = 0;

    void removeChildren(MemoryManager &manager);

    Node *find(void *data, size_t size);
};

// Iterator

class Tree::Iterator : public AbstractTree::Iterator
{
public:
    Iterator(const Node *root, Node *node);
    ~Iterator() = default;

    // -- From Container::Iterator

    // Возврашает явно указатель на элемент, на который указывает итератор в данный момент.
    // Неявно возвращает размер данных.
    // Если итератор показывает за пределы контейнера (например, удален последний элемент), возвращает NULL.
    void *getElement(size_t &size);

    // Возвращает true, если есть следующий элемент, иначе false.
    bool hasNext();

    // Переход к следующему элементу.
    void goToNext();

    // проверка на равенство итераторов
    bool equals(Container::Iterator *right);

    // -- From AbstractTree::Iterator

    // Переход к родительской вершине. Возвращает false если текущая вершина - корень или end().
    bool goToParent();

    // Переход к дочерней вершине № child_index. Возвращает false если дочерней вершины с таким номером нет.
    bool goToChild(int child_index);

    // -- Members

    // Получение узла дерева, на который указывает итератор на данный момент
    Node *getCurrentNode() { return _node; }

private:
    const Node *_root;
    Node *_node;
};

// Exceptions

class Tree::NotAvailable : public Container::Error
{
public:
    NotAvailable() : Container::Error{"Element is not available"} {}
};

class Tree::InvalidIterator : public Container::Error
{
public:
    InvalidIterator() : Container::Error{"Iterator is not valid"} {}
};

class Tree::InvalidParameters : public Container::Error
{
public:
    InvalidParameters() : Container::Error{"Paramter is not valid"} {}
};
