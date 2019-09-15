#include "TextureLogic/Zoom.h"
#include <iostream>
#include <QtTest/qtestcase.h>
#include <Tests/TestSuite.h>

int main(int argc, char *argv[]) {

    auto suite = Tests::TestSuite::suite();

    for(const auto i : suite)
    {
        QTest::qExec(i, argc, argv);
    }

//for(auto &a : TextureLogic::All)
//{
//    std::cout << (static_cast< int>(a) >> 28) << std::endl;
//    std::cout << (static_cast<unsigned int>(a) & 0xFFF) << "\n\n\n";
//}


    return 0;
}