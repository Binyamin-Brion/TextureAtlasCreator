//
// Created by BinyBrion on 10/3/2019.
//

#include "TextureInfoScrollArea.h"
#include "SelectedTextureInformation.h"

namespace GUI
{
    namespace TextureInformation
    {
        TextureInfoScrollArea::TextureInfoScrollArea(QWidget *parent) : QScrollArea{parent},
                                                                        selectedTextureInformation{new SelectedTextureInformation{this}}
        {
            setLayout(new QHBoxLayout);

            setWidgetResizable(true);
        }

        void TextureInfoScrollArea::setCentralWidget()
        {
            setWidget(selectedTextureInformation);
        }

        void TextureInfoScrollArea::setTexture(const TextureLogic::Texture *texture, AccessRestriction::PassKey<TextureLogic::TextureBank>)
        {
            selectedTextureInformation->setTexture(texture);
        }
    }
}