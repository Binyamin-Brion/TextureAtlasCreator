//
// Created by binybrion on 3/28/20.
//

#ifndef TEXTUREATLASCREATOR_TESTTEXTUREATLAS_H
#define TEXTUREATLASCREATOR_TESTTEXTUREATLAS_H

#include "../Tests/TestSuite.h"

namespace Tests
{
    namespace TextureAtlas
    {
        class TestTextureAtlas : public TestSuite
        {
            Q_OBJECT

            private slots:

                /*
                 * Test adding a new texture to the texture atlas by mimicking the actions taken when pressing a
                 * texture button.
                 */
                void testAddTexture();

                /*
                 * Test adding the same texture twice to an atlas.
                 */
                void testAddDuplicateTexture();

                /*
                 * Test in which a texture already exists in the atlas, and a new texture is attempted to be placed
                 * in a position causing an intersection with the existing texture.
                 */
                void testAddIntersectingTextures();

                /*
                 * Test adding a texture with a known position, skipping the actions a user would normally take when
                 * adding a texture to the atlas.
                 */
                void testAddTextureWithPosition();

                /*
                 * Test to remove a selected texture; ie after the user takes some action, there is no longer a selected texture.
                 */
                void testRemoveNewSelectedTexture();

                /*
                 * Test in which a texture is moved around, checking that the texture is placed in its new position correctly.
                 */
                void testMoveExistingTexture();

                /*
                 * Try moving an existing texture to a position that would cause an intersection with a different existing texture.
                 */
                void testMoveExistingTextureIntersection();

                /*
                 * Test to attempt to delete a texture from an atlas.
                 */
                void testDeleteExistingTexture();

                /*
                 * Test undo(ing) a texture movement, placing the texture back to its previous drawing location.
                 */
                void testTextureMovementUndo();

                /*
                 * Test zooming in and out of the atlas to ensure texture drawing positions are updated correctly. This
                 * includes both (existing) selected and unselected textures.
                 */
                void testZoom();

                /*
                 *  Check that the texture can be resized, and only resized if the new size does not cut out any existing textures.
                 */
                void testResizeAtlas();

                /*
                 *  Test removing an existing texture by calling a function the texture atlas provides rather than by
                 *  selecting an existing texture and pressing the "Delete" key.
                 */
                void testRemoveTexture();

                /*
                 * Test that the atlas exports itself correctly.
                 */
                void testExportAtlas();

                /*
                 * Test that the atlas is saved to a file correctly.
                 */
                void testSaveAtlas();

                /*
                 * Test that changing the intersection border width of a border is valid.
                 */
                void testNewIntersectionBorderWidthValid();

                /*
                 * Test that the reported percentage of the atlas that is used is correct.
                 */
                void testGetPercentageAtlasUsed();

            private:
                QString getTestAssetFolder() const;
        };
    }
}

#endif //TEXTUREATLASCREATOR_TESTTEXTUREATLAS_H
