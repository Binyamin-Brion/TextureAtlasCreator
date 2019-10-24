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

        /*
         *  Provides a scrollable view for the SelectedTextureInformation Widget.
         *  Since that widget cannot fit all of its content into the screen space given to it,
         *  a scrollable view is needed to view all of it.
         */

        class TextureInfoScrollArea : public QScrollArea
        {
            public:
                explicit TextureInfoScrollArea(QWidget *parent = nullptr);
                void selectedTextureModified();
                void setCentralWidget();
                void setTexture(const TextureLogic::Texture *texture, AccessRestriction::PassKey<TextureLogic::TextureBank>);
                void setTextureBankReference(TextureLogic::TextureBank *textureBank);

            private:
                SelectedTextureInformation *selectedTextureInformation = nullptr;
                TextureLogic::TextureBank *textureBank = nullptr;

        };
    }
}

#endif //TEXTUREATLASCREATOR_TEXTUREINFOSCROLLAREA_H
