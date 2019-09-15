#include "TextureLogic/TextureBank.h"
#include <iostream>
#include <QtTest/qtestcase.h>
#include <Tests/TestSuite.h>

int main(int argc, char *argv[]) {

    auto suite = Tests::TestSuite::suite();

    for(const auto i : suite)
    {
        QTest::qExec(i, argc, argv);
    }



    return 0;
}