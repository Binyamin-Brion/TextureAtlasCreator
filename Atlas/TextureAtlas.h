//
// Created by BinyBrion on 2019-09-14.
//

#ifndef TEXTUREATLASCREATOR_TEXTUREATLAS_H
#define TEXTUREATLASCREATOR_TEXTUREATLAS_H

#include "TextureLogic/Texture.h"
#include "Atlas/TextureDrawingPosition.h"
#include "AccessRestriction/PassKey.h"

class QPainter;

namespace TextureLogic
{
    class TextureBank;
}

namespace GUI
{
    namespace Atlas
    {
        class AtlasWidget;
    }
}

namespace Atlas
{
    class SelectedTexture;

    /**
     *  Logically holds textures inside one bigger texture to create a texture atlas. Internally, holds references
     *  to textures held inside the TextureAtlas class. This class implements the logic of adding textures together
     *  to create a texture atlas, such as for example making sure that two textures do not overlap.
     */

    class TextureAtlas
    {
        public:

            /**
             * Initializes the texture atlas to the format passed in.
             *
             * @param atlasFormat the internal format the atlas should have, and will export as
             */
            explicit TextureAtlas(QImage::Format atlasFormat);

            /**
             *
             * @return
             */
            bool checkIntersection();

            /**
              * Draws the entire atlas (and thus all of the textures within the atlas), onto the widget that the passed in painter came from.
              *
              * @param painter the painter object to be used for painting the textures
              */
            void draw(QPainter &painter);

            /**
             * Get the internal format that the atlas uses.
             *
             * @return interal format of the atlas, which is the format all of the interal textures use
             */
            QImage::Format getAtlasFormat() const;

            /**
             * Get the size of the atlas in the x and y dimensions.
             *
             * @return rectangular dimensions of the atlas
             */
            QSize getAtlasSize() const;

            /**
             * Get the number of textures currently in the atlas.
             * Note: this includes all of the unselected textures plus the selected texture if there is a selected texture
             *
             * @return the total number of textures in the atlas.
             */
            unsigned int getNumberTextures();

            /**
             * Returns the percentage of the atlas is covered by textures.
             *
             * @return percentage of the total area of the atlas that is taken by textures
             */
            unsigned int getPercentageAtlasUsed() const;

            /**
             * Gets the size of the selected texture, if any.
             * Note: this function will return a negative size if there is neither a selected new texture or a selected
             *       existing texture
             *
             * @return the size of the selected texture. Negative dimensions if no texture is selected
             */
            QSize getSelectedTextureSize() const;

            /**
             * Exports the texture atlas to the given location using the atlas' internal format and size.
             *
             * @param exportLocation the location the texture atlas should be saved to. This must include the full path,
             *         ie, the path to the destination folder + the name of the exported file + image extension name
             * @return true if the atlas was successfully saved
             */
            bool exportImage(const QString &exportLocation) const;

            /**
             * React to certain key presses and take the appropriate action. For example, pressing delete on a selected
             * existing texture causes it to be removed from the atlas
             *
             * @param keyID the numeric representation of the key pressed held by a Qt key event
             */
            void keyPressed(int keyID);

            /**
             * React to mouse click and take the appropriate action. This typically includes selecting and deselecting textures.
             *
             * @param mouseX the x-position of the cursor relative to the widget holding the texture atlas
             * @param mouseY the y-position of the cursor relative to the widget holding the texture atlas
             * @param mouseButton the mouse button that was clicked
             */
            void mouseClicked(int mouseX, int mouseY, int mouseButton);

            /**
             *  React to mouse being moved and take appropriate action. Typically includes moving a selected texture around.
             *
             * @param mouseX the x-position of the cursor relative to the widget holding the texture atlas
             * @param mouseY the y-position of the cursor relative to the widget holding the texture atlas
             */
            void mouseMoved(int mouseX, int mouseY);

            /**
             * React to mouse click being released and take appropriate action. Typically includes letting the atlas know
             * to ignore future mouse movements until another mouse click is performed.
             *
             * @param mouseX the x-position of the cursor relative to the widget holding the texture atlas
             * @param mouseY the y-position of the cursor relative to the widget holding the texture atlas
             * @param mouseButton the mouse button that was released
             */
            void mouseReleased(int mouseX, int mouseY, int mouseButton);

            /**
             * Resizes the intersection border width of the passed in texture. This merely checks that the new intersection width,
             * changed by the caller of this function, is valid.
             *
             * @param texture
             * @return true if the new intersection width causes an intersection; the new intersection width is not valid
             */
            bool newIntersectionBorderWidthValid(TextureLogic::Texture *texture);

            /**
             * Removes the passed in texture from the atlas. This includes selected textures as well. If the selected
             * texture is not an already placed texture, it is unbound to the cursor; there will no longer be a texture
             * following the cursor.
             *
             * @param texture the texture to remove from the atlas
             */
            void removeTexture(const TextureLogic::Texture *texture);

            /**
             * Sets the size of the atlas. It should match the widget holding the atlas.
             *
             * @param size the dimensions the atlas should have
             */
            void setAtlasSize(QSize size);

            /**
             * Pass in a reference
             *
             * @param atlasWidget
             */
            void setAtlasWidgetReference(GUI::Atlas::AtlasWidget *atlasWidget);

            /**
             * Sets the selected texture for the atlas. The texture is not placed in the atlas, but the cursor will now
             * be bound to the texture passed in.
             * Note: This applies to newly selected texture, not a texture already placed in the atlas. Any existing
             *       selected texture, new or already placed in atlas, is unselected.
             *
             * @param texture the texture to mark as selected and bound to the cursor
             */
            void setSelectedTexture(const TextureLogic::Texture &texture);

            /**
             * Sets the reference to the vector containing all of the textures that were loaded. This is used in the
             * textureLoaded() function.
             *
             * @param textureBank reference to the vector holding all of the textures
             */
            void setTextureBankReference(TextureLogic::TextureBank *textureBank);

            /**
             * Function to update the the pointers referencing the contents of the texture bank. This has to be called each time
             * a texture is loaded as the vector contents may have moved in memory, invalidating pointers to the contents.
             */
            void textureLoaded();

            /**
             * Checks if a texture is selected, either a texture already in the atlas or a new texture to place in the atlas.
             *
             * @return true if texture is selected, regardless of it is in the atlas already
             */
            bool textureSelected() const;

            /**
             * Update the drawing positions of the textures in the atlas to account for the change in zoom.
             * Note: zoom is in done in steps, ie from 100% to 200%, not from 100% to 400%. This negates the need to
             *       pass in the new zoom value in to this function.
             */
            void zoomIn();

            /**
             * Update the drawing positions of the textures in the atlas to account for the change in zoom.
             * Note: zoom is in done in steps, ie from 100% to 200%, not from 100% to 400%. This negates the need to
             *       pass in the new zoom value in to this function.
             */
            void zoomOut();

        private:

            /**
             * Adds the current selected texture to the atlas. This applies to the existing textures in the atlas as well,
             * as when then were selected they are in a state in which which they are a part of the atlas, but considered
             * separate from the other textures not selected.
             *
             * @param selectedTexture the texture to add to the atlas
             */
            void addTexture(SelectedTexture *selectedTexture);

            /**
             * Let the selected texture, if any, know of changes to zoom so that it can change its drawing position
             *
             * @param newZoom the new zoom that the atlas is currently being viewed at
             * @param zoomFactor the ratio between the new zoom and the old one
             */
            void updateSelectedTexturesZoom(TextureLogic::Zoom newZoom, float zoomFactor);

            /**
             * Update the unselected textures in the atlas as a result of the zoom changing.
             *
             * @param factor the ratio between the new zoom and the old one
             */
            void updateTextureDrawingPositions(float factor);

            // This class needs to talk with other classes if a certain action has been performed, hence the reason for these
            // pointers to those classes.
            GUI::Atlas::AtlasWidget *atlasWidget = nullptr;
            TextureLogic::TextureBank *textureBank = nullptr;

            const std::vector<std::pair<std::vector<TextureLogic::Texture>, std::vector<unsigned int>>> *textures = nullptr;
            std::vector<TextureDrawingPosition> textureDrawingPositions;

            // The selectedTexture variable is for when a new texture is being added to the atlas; otherwise, a texture already
            // in the atlas is selected and the selectedExistingTexture variable is used.
            SelectedTexture *selectedTexture = nullptr;
            SelectedTexture *selectedExistingTexture = nullptr;
            QPointF previousDrawingCoords;

            // Variables to keep track of the zoom the atlas is in so that any changes done to zoom can be done from a
            // known zoom reference point
            TextureLogic::Zoom currentZoom;
            unsigned int currentZoomIndex;

            // Variables are used when working with the logic surrounding cursor input
            bool intersectionOccured = false;
            bool ignoreMouseRelease = false;
            bool leftMouseButtonDown = false;
            QPoint newMousePosition;

            // Variables used when exporting the atlas and determining if a new texture can be placed in the atlas
            QSize atlasSize;
            QImage::Format atlasFormat;
    };
}

#endif //TEXTUREATLASCREATOR_TEXTUREATLAS_H
