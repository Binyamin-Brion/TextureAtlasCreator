//
// Created by binybrion on 3/28/20.
//

#ifndef TEXTUREATLASCREATOR_TESTSELECTEDTEXTURE_H
#define TEXTUREATLASCREATOR_TESTSELECTEDTEXTURE_H

#include "../TestSuite.h"

namespace Tests
{
    namespace TextureLogic
    {
        class TestSelectedTexture : public TestSuite
        {
            Q_OBJECT

            private slots:

                /*
                 * Tests that the logic of ensuring a selected texture's drawing coordinates are valid
                 * even if the position it is told to move to would normally result in invalid drawing coordinates.
                 */
                void testMove();

            private:
                QString getTestAssetFolder() const;
        };
    }
}

#endif //TEXTUREATLASCREATOR_TESTSELECTEDTEXTURE_H
