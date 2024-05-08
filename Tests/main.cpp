#include <Tree.h>
#include <Mem.h>

#include <limits>

struct TestData
{
    int i = 10;
    short c[2] = { 7, 9 };
};

int main()
{
    Mem mem{ std::numeric_limits<size_t>::max() };
    Tree tree{ mem };



    return 0;
}
