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

    class TextureAtlas
    {
        public:
            TextureAtlas();
            bool checkIntersection();
            void draw(QPainter &painter);
            std::pair<bool, QSize> getAtlasSize() const;
            std::pair<bool, QSize> getSelectedTextureSize() const;
            void keyPressed(int keyID);
            void mouseClicked(int mouseX, int mouseY, int mouseButton);
            void mouseMoved(int mouseX, int mouseY);
            void mouseReleased(int mouseX, int mouseY, int mouseButton);
            void removeTexture(const TextureLogic::Texture *texture);
            void setAtlasSize(QSize size);
            void setAtlasWidgetReference(GUI::Atlas::AtlasWidget *atlasWidget);
            void setSelectedTexture(const TextureLogic::Texture &texture);
            void setTextureBankReference(TextureLogic::TextureBank *textureBank);
            void textureLoaded(const std::vector<TextureLogic::Texture> &textures);

        private:
            void addTexture(SelectedTexture *selectedTexture);
            GUI::Atlas::AtlasWidget *atlasWidget = nullptr;
            std::vector<TextureDrawingPosition> textureDrawingPositions;
            const std::vector<TextureLogic::Texture> *textures = nullptr;
            TextureLogic::TextureBank *textureBank = nullptr;

            SelectedTexture *selectedTexture = nullptr;
            SelectedTexture *selectedExistingTexture = nullptr;
            QPoint previousDrawingCoords;

            TextureLogic::Zoom currentZoom;
            unsigned int currentZoomIndex;

            QSize atlasSize;

            bool intersectionOccured = false;
            bool ignoreMouseRelease = false;
            bool leftMouseButtonDown = false;

            QPoint newMousePosition;
    };
}

#endif //TEXTUREATLASCREATOR_TEXTUREATLAS_H
