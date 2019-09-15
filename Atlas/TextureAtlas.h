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

namespace Atlas
{
    class SelectedTexture;

    class TextureAtlas
    {
        public:
            TextureAtlas();
            bool checkIntersection();
            void draw(QPainter &painter);
            void setSelectedTexture(const TextureLogic::Texture &texture, AccessRestriction::PassKey<TextureLogic::TextureBank>);
            void mouseClicked();
            void mouseMoved(int mouseX, int mouseY);
            void setTextureBankReference(TextureLogic::TextureBank *textureBank);
            void textureLoaded(AccessRestriction::PassKey<TextureLogic::TextureBank>);

        private:
            void addTexture();
            TextureLogic::TextureBank *textureBank = nullptr;
            std::vector<QString> texturesInAtlas;
            std::vector<TextureDrawingPosition> textureDrawingPositions;
            const std::vector<TextureLogic::Texture> *textures = nullptr;

            SelectedTexture *selectedTexture = nullptr;

            TextureLogic::Zoom currentZoom;
    };
}

#endif //TEXTUREATLASCREATOR_TEXTUREATLAS_H
