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

    /*
     *  Logically holds textures inside one bigger texture to create a texture atlas. Internally, holds references
     *  to textures held inside the TextureAtlas class. This class implements the logic of adding textures together
     *  to create a texture atlas, such as for example making sure that two textures do not overlap.
     */

    class TextureAtlas
    {
        public:
            explicit TextureAtlas(QImage::Format atlasFormat);
            bool checkIntersection();
            void draw(QPainter &painter);
            QImage::Format getAtlasFormat() const;
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
            void textureLoaded(const std::vector<std::pair<std::vector<TextureLogic::Texture>, std::vector<unsigned int>>> &textures);
            void zoomIn();
            void zoomOut();

        private:
            void addTexture(SelectedTexture *selectedTexture);
            void updateSelectedTexturesZoom(TextureLogic::Zoom newZoom, float zoomFactor);
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

            TextureLogic::Zoom currentZoom;
            unsigned int currentZoomIndex;

            QSize atlasSize;

            bool intersectionOccured = false;
            bool ignoreMouseRelease = false;
            bool leftMouseButtonDown = false;

            QPoint newMousePosition;

            QImage::Format atlasFormat;
    };
}

#endif //TEXTUREATLASCREATOR_TEXTUREATLAS_H
