//
// Created by BinyBrion on 2019-09-13.
//

#ifndef TEXTUREATLASCREATOR_TESTSUITE_H
#define TEXTUREATLASCREATOR_TESTSUITE_H

#include <QtCore/QObject>

namespace Tests
{
    /*
     *  Creates a test suite to simplify the execution of tests. Any time a class derived
     *  from this class is created, it will be added to the (static) vector of this class.
     *  Once all the appropriate test classes are created (which are derived from this class),
     *  loop through the (static) vector of this class and execute each test.
     *
     *  Idea is from: https://alexhuszagh.github.io/2016/using-qttest-effectively/
     *
     *  Example Usage:
     *
     *  class TestSomeComponent : public TestSuite { *Some Tests to execute* };
     *
     *  auto tests = TestSuite::suite();
     *
     *  for(const auto i : tests)
     *  {
     *      QTest::qExec(i, argc, argv);
     *  }
     */

    struct TestSuite : public QObject
    {
        TestSuite();

        // the vector containing pointers to all of the tests to be executed
        static std::vector<QObject*>& suite();
    };
}

#endif //TEXTUREATLASCREATOR_TESTSUITE_H
