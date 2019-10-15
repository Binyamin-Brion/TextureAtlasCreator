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
    /*
     *  Represents a texture that is selected. It is used in conjunction with the TextureAtlas class.
     *  Whenever a user clicks on a texture in the atlas, the reference to that texture is passed to an instance
     *  of this class from the TextureAtlas class. The reference in the TextureAtlas class is removed until the user
     *  moves to the selected texture to the desired place and indicates the texture is to be placed there. Then,
     *  this class becomes "empty" (doesn't hold any reference), and the TextureAtlas class once again holds the texture
     *  reference.
     */

    class SelectedTexture
    {
        using SurroundingBorder = std::array<TextureBorder::SurroundingBorder, ::TextureLogic::NumberZoomElements()>;

        public:

            QPointF getDrawingCoordinates() const;
            int getTextureIndex() const;
            const TextureLogic::Texture& getImage();
            const TextureLogic::Texture& getImageForDrawing() const;
            const SurroundingBorder& getSurroundingBorderForDrawing() const;
            SurroundingBorder getSurroundingBorder() const;
            const QString& getTextureLocation() const;
            bool isOpen() const;
            void move(int mouseX, int mouseY, QSize boundaries);
            void setDrawingCoordinates(QPoint drawingCoords);
            void setDrawSelectedSurroundingBorder(bool value);
            void setTexture(const TextureLogic::Texture &selectedTexture, int index = -1);
            void setTextureReference(const TextureLogic::Texture &selectedTexture);
            void setZoom(TextureLogic::Zoom zoom, float zoomFactor);

        private:
            const TextureLogic::Texture* selectedTexture = nullptr;

            // The texture index refers to the index of the selected texture inside the appropriate format texture
            // in the TextureBank class. See fn setTextureReference of this class for details.

            int textureIndex = -1;

            TextureLogic::Zoom currentZoom = TextureLogic::Zoom::Normal;
            QPointF drawingCoordinates;
            bool _isOpen = false;
            SurroundingBorder surroundingBorder;
    };

}

#endif //TEXTUREATLASCREATOR_SELECTEDTEXTURE_H
