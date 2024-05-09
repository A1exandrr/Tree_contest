#include <Tree.h>
#include <Mem.h>

#include <iostream>
#include <limits>

struct TestData
{
    int i = 10;
    short c[2] = {7, 9};
};

int main()
{
    Mem mem{std::numeric_limits<size_t>::max()};
    Tree tree{mem};
    Tree::Iterator *iterator = tree.newIterator();
    TestData data;
    tree.insert(iterator, 0, &data, sizeof(data));
    if (iterator->goToChild(0))
    {
        size_t dataSize = 0;
        TestData *data = reinterpret_cast<TestData *>(iterator->getElement(dataSize));

        std::cout << "data->i = " << data->i
                  << "; data->c = { " << data->c[0] << ", " << data->c[1] << " }" << std::endl;

        iterator->goToParent();
    }
    delete iterator;

    return 0;
}
