//
// Created by binybrion on 3/28/20.
//

#ifndef TEXTUREATLASCREATOR_TESTTEXTUREBANK_H
#define TEXTUREATLASCREATOR_TESTTEXTUREBANK_H

#include "../TestSuite.h"

namespace Tests
{
    namespace TextureLogic
    {
        class TestTextureBank : public TestSuite
        {
            Q_OBJECT

            private slots:

                /*
                 * Tests that loading a texture into the texture bank results in the texture being stored correctly
                 * in the right place internally.
                 */
                void testLoadNewTexture();

                /*
                 * Tests that removing a texture that was previously loaded is correctly removed from the texture bank.
                 */
                void testRemoveTexture();

                /*
                 * Tests that after loading a texture, the texture bank is able to find the loaded texture if requested for.
                 */
                void testFindTexture();

            private:
                QString getTestAssetFolder() const;
        };
    }
}

#endif //TEXTUREATLASCREATOR_TESTTEXTUREBANK_H
