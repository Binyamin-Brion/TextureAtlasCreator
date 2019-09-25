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
    class SelectedTexture
    {
        using SurroundingBorder = std::array<TextureBorder::SurroundingBorder, ::TextureLogic::NumberZoomElements()>;

        public:

            QPoint getDrawingCoordinates() const;
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
            void setZoom(TextureLogic::Zoom zoom);
            void translateSurroundingBorder(int x, int y);

        private:
            const TextureLogic::Texture* selectedTexture;
            int textureIndex = -1;
            TextureLogic::Zoom currentZoom = TextureLogic::Zoom::Normal;
            QPoint drawingCoordinates;
            bool _isOpen = false;
            bool firstMouse = true;
            int previousMouseX = 0;
            int previousMouseY = 0;
            SurroundingBorder surroundingBorder;
    };

}

#endif //TEXTUREATLASCREATOR_SELECTEDTEXTURE_H
