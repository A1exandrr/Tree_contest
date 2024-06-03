#pragma once

#include "TestData.h"

struct TestAsContainer: public Test
{
    void run() override;

    bool result() override;

private:
    void fillContainer();

    bool size();
    bool max_bytes();
    bool find();
    bool newIterator();
    bool remove();
    // bool clear(); // пропущено, так как оттестировано и активно используется
    // bool empty(); // в контейнере происходит подсчёт напрямую

    bool _testResult = true;
};
