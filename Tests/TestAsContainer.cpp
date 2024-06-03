#include "TestAsContainer.h"

#include <cstring>
#include <iostream>

void TestAsContainer::run()
{
    const char *currentFunction = "TestAsContainer::run()";

    std::cout << currentFunction << ": begin" << std::endl;

    if (false == size())
    {
        std::cout << currentFunction << ": TestAsContainer::size() failed" << std::endl;
        _testResult = false;
        return;
    }
    std::cout << currentFunction << ": TestAsContainer::size() success" << std::endl;

    if (false == max_bytes())
    {
        std::cout << currentFunction << ": TestAsContainer::max_bytes() failed" << std::endl;
        _testResult = false;
        return;
    }
    std::cout << currentFunction << ": TestAsContainer::max_bytes() success" << std::endl;

    if (false == find())
    {
        std::cout << currentFunction << ": TestAsContainer::find() failed" << std::endl;
        _testResult = false;
        return;
    }
    std::cout << currentFunction << ": TestAsContainer::find() success" << std::endl;

    if (false == newIterator())
    {
        std::cout << currentFunction << ": TestAsContainer::newIterator() failed" << std::endl;
        _testResult = false;
        return;
    }
    std::cout << currentFunction << ": TestAsContainer::newIterator() success" << std::endl;

    if (false == remove())
    {
        std::cout << currentFunction << ": TestAsContainer::remove() failed" << std::endl;
        _testResult = false;
        return;
    }
    std::cout << currentFunction << ": TestAsContainer::remove() success" << std::endl;

    std::cout << currentFunction << ": end" << std::endl;
}

bool TestAsContainer::result()
{
    return _testResult;
}

// private

void TestAsContainer::fillContainer()
{
    // Для дочерних вершин первого слоя -- пять элементов
    TestData testDataFirstLevel[5];
    for (size_t i = 0; i < 5; ++i)
    {
        testDataFirstLevel[i].i = 100 * (i + 1);
        testDataFirstLevel[i].c[0] = 10 * (i + 1);
        testDataFirstLevel[i].c[1] = 5 * (i + 1);
    }

    // Для дочерних вершин второго слоя -- по три листа у каждого корня первого слоя
    TestData testDataSecondLevel[15];
    for (size_t i = 0; i < 15; ++i)
    {
        testDataSecondLevel[i].i = 1000 * (i + 1);
        testDataSecondLevel[i].c[0] = 100 * (i + 1);
        testDataSecondLevel[i].c[1] = 10 * (i + 1);
    }

    // Вставка

    AbstractTree &tree = abstractTree();
    AbstractTree::Iterator *iterator =
            dynamic_cast<AbstractTree::Iterator *>(tree.newIterator());

    for (size_t i = 0; i < 5; ++i)
        tree.insert(iterator, i, &testDataFirstLevel[i], sizeof(TestData));

    for (size_t i = 0; i < 5; ++i)
    {
        iterator->goToChild(i);
        for (size_t j = 0; j < 3; ++j)
        {
            size_t index = (i * 3) + j;
            tree.insert(iterator, j, &testDataSecondLevel[index], sizeof(TestData));
        }
        iterator->goToParent();
    }

    delete iterator;
}

bool TestAsContainer::size()
{
    const char *currentFunction = "    TestAsContainer::size()";

    Container &container = this->container();
    if (0 != container.size())
    {
        std::cout << currentFunction
                << ": container.size() with empty container got non-zero value"
                << std::endl;
        return false;
    }
    fillContainer();
    if (20 != container.size())
    {
        std::cout << currentFunction
                << ": container.size() with non-empty container got invalid value"
                << std::endl;
        return false;
    }

    container.clear();
    return true;
}

bool TestAsContainer::max_bytes()
{
    const char *currentFunction = "    TestAsContainer::max_bytes()";
    if (std::numeric_limits<size_t>::max() != container().max_bytes())
    {
        std::cout << currentFunction
                << ": container.max_bytes() got invalid value"
                << std::endl;
        return false;
    }
    return true;
}

bool TestAsContainer::find()
{
    const char *currentFunction = "    TestAsContainer::find()";

    Container &container = this->container();

    // Существующее
    TestData existData;
    existData.i = 2000;
    existData.c[0] = 200;
    existData.c[1] = 20;

    // Несущетвующее
    TestData notExistData;
    notExistData.i = 2000;
    notExistData.c[0] = 200;
    notExistData.c[1] = 200;

    // Поиск в пустом контейнере
    Container::Iterator *iterator = container.find(&existData, sizeof(TestData));
    if (nullptr != iterator)
    {
        std::cout << currentFunction
                << ": Empty container after find() didn't return nullptr" << std::endl;
        return false;
    }

    fillContainer(); // Заполняем контейнер

    // Поиск существующего в заполненном контейнере
    iterator = container.find(&existData, sizeof(TestData));
    if (nullptr == iterator)
    {
        std::cout << currentFunction
                << ": Filled container after find(existData) didn't return iterator" << std::endl;
        return false;
    }
    size_t dataSize = 0;
    void *dataValue = iterator->getElement(dataSize);
    if ((sizeof(TestData) != dataSize) || (dataValue == nullptr) ||
            (0 != ::memcmp(&existData, dataValue, sizeof(TestData))))
    {
        delete iterator;
        std::cout << currentFunction
                << ": Iterator returned invalid data" << std::endl;
        return false;
    }
    delete iterator;

    // Поиск несуществующего в заполненном контейнере
    iterator = container.find(&notExistData, sizeof(TestData));
    if (nullptr != iterator)
    {
        std::cout << currentFunction
                << ": Filled container after find(notExistData) didn't return nullptr"
                << std::endl;
        return false;
    }

    container.clear();
    return true;
}

bool TestAsContainer::newIterator()
{
    // Получить итератор пустого контейнера

    // Получить итератор непустого контейнера и проверить, что значение там то

    return true;
}

bool TestAsContainer::remove()
{
    // Удалить узел из пустого контейнера

    // Удалить первый

    // Удалить средний

    // Удалить последний

    return true;
}
