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
        class CurrentTextureTabWidget : public QTabWidget
        {
            Q_OBJECT

            public:
                explicit CurrentTextureTabWidget(QWidget *parent = nullptr);
                void setSelectedTexture(TextureLogic::Texture *texture, AccessRestriction::PassKey<TextureLogic::TextureBank>);
                void setTextureBankReference(TextureLogic::TextureBank *textureBank);
                void setTexturesReference(std::vector<TextureLogic::Texture> &textures);

            signals:
                void repaintSelectedTexture();

            private:
                std::array<std::pair<QString, RenderArea*>, 2> currentTexture;
                std::vector<TextureLogic::Texture> *textures = nullptr;
                TextureLogic::TextureBank *textureBank = nullptr;
                int currentTextureIndex;
        };
    }
}

#endif //TEXTUREATLASCREATOR_CURRENTTEXTURETABWIDGET_H
