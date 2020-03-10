//
// Created by BinyBrion on 2020-03-09.
//

#ifndef TEXTUREATLASCREATOR_TESTPROJECTLOADER_H
#define TEXTUREATLASCREATOR_TESTPROJECTLOADER_H

#include "Tests/TestSuite.h"

namespace Tests
{
    namespace ProjectLoader
    {
        class TestProjectLoader : public TestSuite
        {
            Q_OBJECT

            private slots:

                void testParseProjectFile();
                void testParseProjectFile2();

            private:
                QString getTestAssetDirectory();
        };
    }
}

#endif //TEXTUREATLASCREATOR_TESTPROJECTLOADER_H
