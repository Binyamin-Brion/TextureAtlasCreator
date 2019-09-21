//
// Created by BinyBrion on 2019-09-14.
//

#ifndef TEXTUREATLASCREATOR_TEXTUREATLAS_H
#define TEXTUREATLASCREATOR_TEXTUREATLAS_H

#include "TextureLogic/Texture.h"
#include "Atlas/TextureDrawingPosition.h"
#include "AccessRestriction/PassKey.h"

class QPainter;

namespace Atlas
{
    class SelectedTexture;

    class TextureAtlas
    {
        public:
            TextureAtlas();
            bool checkIntersection();
            void draw(QPainter &painter);
            std::pair<bool, QSize> getAtlasSize() const;
            std::pair<bool, QSize> getSelectedTextureSize() const;
            void mouseClicked();
            void mouseMoved(int mouseX, int mouseY);
            std::pair<bool, QPoint> resetCursorPosition() const;
            void resetFirstMouse();
            void setAtlasSize(QSize size);
            void setSelectedTexture(const TextureLogic::Texture &texture);
            void textureLoaded(const std::vector<TextureLogic::Texture> &textures);

        private:
            void addTexture();
            std::vector<QString> texturesInAtlas;
            std::vector<TextureDrawingPosition> textureDrawingPositions;
            const std::vector<TextureLogic::Texture> *textures = nullptr;

            SelectedTexture *selectedTexture = nullptr;

            TextureLogic::Zoom currentZoom;
            unsigned int currentZoomIndex;

            QSize atlasSize;

            bool intersectionOccured = false;
            bool firstMouse = true;
            int previousMouseX;
            int previousMouseY;
    };
}

#endif //TEXTUREATLASCREATOR_TEXTUREATLAS_H
