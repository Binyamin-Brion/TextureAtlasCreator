//
// Created by binybrion on 3/29/20.
//

#ifndef TEXTUREATLASCREATOR_TESTTEXTUREBUTTONAREA_H
#define TEXTUREATLASCREATOR_TESTTEXTUREBUTTONAREA_H

#include "../TestSuite.h"

namespace Tests
{
    namespace GUI
    {
        class TestTextureButtonArea : public TestSuite
        {
            Q_OBJECT

            private slots:

                /*
                 *  Tests that a loaded texture information is saved to a file correctly.
                 */
                void testSaveLoadedTextures();

            private:
                QString getTestAssetFolder();
        };
    }
}

#endif //TEXTUREATLASCREATOR_TESTTEXTUREBUTTONAREA_H
