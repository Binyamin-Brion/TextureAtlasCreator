//
// Created by BinyBrion on 2019-09-13.
//

#include "TestSuite.h"

namespace Tests
{
    /*
     *  When a class is derived from this class and constructed, this default constructor will run.
     *  When that does, the instance of the class to be constructed will be added to the test suite.
     *
     *  A class derived from this can be created using a static object. For example"
     *
     *  class SomeTest : public TestSuite { *Some Tests* };
     *
     *  In SomeTest.cpp:
     *
     *  static SomeTest someTest;
     */

    TestSuite::TestSuite()
    {
        suite().push_back(this);
    }

    std::vector<QObject*>& TestSuite::suite()
    {
        static std::vector<QObject*> testObjects;

        return testObjects;
    }
}