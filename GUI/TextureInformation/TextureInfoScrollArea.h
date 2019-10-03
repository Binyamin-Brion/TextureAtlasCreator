//
// Created by BinyBrion on 10/3/2019.
//

#ifndef TEXTUREATLASCREATOR_TEXTUREINFOSCROLLAREA_H
#define TEXTUREATLASCREATOR_TEXTUREINFOSCROLLAREA_H

#include <QtWidgets/QScrollArea>
#include "AccessRestriction/PassKey.h"

namespace TextureLogic
{
    class Texture;
    class TextureBank;
}

namespace GUI
{
    namespace TextureInformation
    {
        class SelectedTextureInformation;

        class TextureInfoScrollArea : public QScrollArea
        {
            public:
                explicit TextureInfoScrollArea(QWidget *parent = nullptr);
                void setCentralWidget();
                void setTexture(const TextureLogic::Texture *texture, AccessRestriction::PassKey<TextureLogic::TextureBank>);

            private:
                SelectedTextureInformation *selectedTextureInformation = nullptr;

        };
    }
}

#endif //TEXTUREATLASCREATOR_TEXTUREINFOSCROLLAREA_H
