//
// Created by BinyBrion on 2019-09-15.
//

#ifndef TEXTUREATLASCREATOR_SELECTEDTEXTURE_H
#define TEXTUREATLASCREATOR_SELECTEDTEXTURE_H

#include <QtCore/QString>
#include <QtGui/QImage>
#include "TextureLogic/Zoom.h"
#include "Atlas/TextureBorder/SurroundingBorder.h"

class QPainter;

namespace TextureLogic
{
    class Texture;
}

namespace Atlas
{
    /**
     *  Represents a texture that is selected. It is used in conjunction with the TextureAtlas class.
     *  Whenever a user clicks on a texture in the atlas, the reference to that texture is passed to an instance
     *  of this class from the TextureAtlas class. The reference in the TextureAtlas class is removed until the user
     *  moves to the selected texture to the desired place and indicates the texture is to be placed there. Then,
     *  this class becomes "empty" (doesn't hold any reference), and the TextureAtlas class once again holds the texture reference.
     */

    class SelectedTexture
    {
            // There are as many surrounding border as there are zoom values, as the dimensions of the border changes with zoom.
            // To make it easier to read, an alias is defined here.
            using SurroundingBorder = std::array<TextureBorder::SurroundingBorder, ::TextureLogic::NumberZoomElements()>;

        public:

            /**
             * Get the drawing coordinates for this texture.
             *
             * @return point representing the drawing coordinates of this texture, corresponding to the texture top-left corner
             */
            QPointF getDrawingCoordinates() const;

            /**
             * Get the texture index indicating where the texture this object is representing is located in the texture bank.
             *
             * @return index into the texture bank where the texture is stored
             */
            int getTextureIndex() const;

            /**
             * Gets the texture this object is representing. This is a destructive return! After calling this function,
             * this object no longer holds the texture it was set to represent.
             *
             * @return the reference to the texture this object represents
             */
            const TextureLogic::Texture& getImage();

            /**
             * Gets the texture this object is representing. Does not modify this object state. The opposite of member
             * function getImage().
             *
             * @return the reference to the texture this object represents
             */
            const TextureLogic::Texture& getImageForDrawing() const;

            /**
             * Gets a const-reference to the surrounding border.
             * Internal note: see SelectedTexture.cpp
             *
             * @return immutable reference to the surrounding border for the texture this object represents
             */
            const SurroundingBorder& getSurroundingBorderForDrawing() const;

            /**
             * Get a non-const reference to the surrounding border.
             * Internal note: see SelectedTexture.cpp
             *
             * @return mmutable reference to the surrounding border for the texture this object represents
             */
            SurroundingBorder& getSurroundingBorder();

            /**
             * Get the location of the the texture this object represents.
             *
             * @return string representing the location of the texture this object on the file system
             */
            const QString& getTextureLocation() const;

            /**
             * Determine if this object is currently representing any texture
             *
             * @return true if this object is representing a texture at the time of the function call
             */
            bool isOpen() const;

            /**
             * Moves the texture this object is representing to the passed in coordinates.
             * Note that this is NOT a translation operation. The texture is directly moved to the location passed in.
             *
             * @param mouseX the x-dimension of where the texture should be moved to
             * @param mouseY the y-dimension of where the texture should be moved to
             * @param boundaries the boundaries past which the texture cannot be moved. It is assumed this represents the
             *        bottom-right corner of a rectangular bounding area, with the top-left corner being the location (0, 0)
             */
            void move(int mouseX, int mouseY, QSize boundaries);

            /**
             * Sets where the texture this object represents should be drawn the widget holding the texture atlas.
             *
             * @param drawingCoords point where the texture should be drawn
             */
            void setDrawingCoordinates(QPointF drawingCoords);

            /**
             * Set whether or not to draw the selected texture border. This toggles the borders at all zoom values.
             * Internal note: see SelectedTexture.cpp
             *
             * @param value true if the border should be drawn
             */
            void setDrawSelectedSurroundingBorder(bool value);

            /**
             * Sets the texture that this object should represent. Call this when a new existing texture is to be
             * held by this object.
             * Internal note: see SelectedTexture.cpp
             *
             * @param selectedTexture reference to the texture to be held in this object
             * @param index the index value of the passed in texture in texture bank
             */
            void setTexture(const TextureLogic::Texture &selectedTexture, int index = -1);

            /**
             * Updates the reference of the texture this object represents. Call this when a texture is loaded and
             * this object is already representing a texture.
             *
             * @param selectedTexture reference to the texture to be held in this object
             */
            void setTextureReference(const TextureLogic::Texture &selectedTexture);

            /**
             * Adjusts the texture this object represents so that it draws properly given the new zoom.
             *
             * @param zoom the new zoom that the user has selected
             * @param zoomFactor the ratio between the new zoom and the old one
             */
            void setZoom(TextureLogic::Zoom zoom, float zoomFactor);

        private:

            // Reference to the texture in the texture bank that this object will represent
            const TextureLogic::Texture* selectedTexture = nullptr;

            // The texture index refers to the index of the selected texture inside the appropriate format texture
            // in the TextureBank class. See fn setTextureReference of this class for details.
            int textureIndex = -1;

            TextureLogic::Zoom currentZoom = TextureLogic::Zoom::Normal;
            QPointF drawingCoordinates;
            bool _isOpen = false;

            // Holds the surrounding border for the texture at all zoom values; see the alias given at the top of this class declaration
            SurroundingBorder surroundingBorder;
    };

}

#endif //TEXTUREATLASCREATOR_SELECTEDTEXTURE_H
