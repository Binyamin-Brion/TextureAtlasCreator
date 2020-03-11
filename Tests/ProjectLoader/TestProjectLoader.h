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

                /*
                 * Tests loading a project file containing only one texture button area and one texture atlas with one
                 * texture within that atlas.
                 */
                void testParseProjectFile();

                /*
                 * Tests loading a more complicated project file that consists of two atlases
                 * (one with two textures, and the other one with one)and two texture button areas
                 * (that each have two textures).
                 */
                void testParseProjectFile2();

            private:
                QString getTestAssetDirectory();
        };
    }
}

#endif //TEXTUREATLASCREATOR_TESTPROJECTLOADER_H
