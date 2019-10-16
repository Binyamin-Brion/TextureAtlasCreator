//
// Created by BinyBrion on 2019-09-26.
//

#ifndef TEXTUREATLASCREATOR_CURRENTTEXTURETABWIDGET_H
#define TEXTUREATLASCREATOR_CURRENTTEXTURETABWIDGET_H

#include <QtWidgets/QTabWidget>
#include <array>
#include "AccessRestriction/PassKey.h"
#include "RenderArea.h"

namespace TextureLogic
{
    class TextureBank;
    class Texture;
}

namespace GUI
{
    namespace CurrentTexture
    {
        class ScrollArea;

        class CurrentTextureTabWidget : public QTabWidget
        {
            Q_OBJECT

            public:
                explicit CurrentTextureTabWidget(QWidget *parent = nullptr);
                void setSelectedTexture(TextureLogic::Texture *texture, AccessRestriction::PassKey<TextureLogic::TextureBank>);
                void setTextureBankReference(TextureLogic::TextureBank *textureBank);
                void setTexturesReference(std::vector<std::pair<std::vector<TextureLogic::Texture>, std::vector<unsigned int>>> &textures);

            signals:
                void changedRenderArea(const PaintFunctions::Brush&);
                void repaintSelectedTexture();
                void selectedTextureChanged(QSize, QSize);
                void zoomChanged(TextureLogic::Zoom);

            private:
                std::array<std::pair<QString, ScrollArea*>, 2> currentTexture;
                std::vector<std::pair<std::vector<TextureLogic::Texture>, std::vector<unsigned int>>> *textures = nullptr;
                TextureLogic::TextureBank *textureBank = nullptr;
                int currentTextureIndex;
                QSize selectedTextureSize;
        };
    }
}

#endif //TEXTUREATLASCREATOR_CURRENTTEXTURETABWIDGET_H
