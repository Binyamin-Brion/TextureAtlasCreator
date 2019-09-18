//
// Created by BinyBrion on 2019-09-15.
//

#ifndef TEXTUREATLASCREATOR_SELECTEDTEXTURE_H
#define TEXTUREATLASCREATOR_SELECTEDTEXTURE_H

#include <QtCore/QString>
#include <QtGui/QImage>
#include <TextureLogic/Zoom.h>

class QPainter;

namespace TextureLogic
{
    class Texture;
}

namespace Atlas
{
    class SelectedTexture
    {
        public:

            QPoint getDrawingCoordinates() const;
            const TextureLogic::Texture& getImage();
            const TextureLogic::Texture& getImageForDrawing() const;
            const QString& getTextureLocation() const;
            bool isOpen() const;
            void move(int mouseX, int mouseY, QSize boundaries);
            void setTexture(const TextureLogic::Texture &selectedTexture);
            void setZoom(TextureLogic::Zoom zoom);

        private:
            const TextureLogic::Texture* selectedTexture;
            TextureLogic::Zoom currentZoom = TextureLogic::Zoom::Normal;
            QPoint drawingCoordinates;
            bool _isOpen = false;
            bool firstMouse = true;
            int previousMouseX = 0;
            int previousMouseY = 0;
    };

}

#endif //TEXTUREATLASCREATOR_SELECTEDTEXTURE_H
