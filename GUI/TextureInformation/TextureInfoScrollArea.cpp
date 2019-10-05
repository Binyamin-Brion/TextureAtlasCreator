//
// Created by BinyBrion on 10/3/2019.
//

#include "TextureInfoScrollArea.h"
#include "SelectedTextureInformation.h"

#include "TextureLogic/TextureBank.h"

namespace GUI
{
    namespace TextureInformation
    {
        TextureInfoScrollArea::TextureInfoScrollArea(QWidget *parent) : QScrollArea{parent},
                                                                        selectedTextureInformation{new SelectedTextureInformation{this}}
        {
            setLayout(new QHBoxLayout);

            setWidgetResizable(true);

            connect(selectedTextureInformation, &SelectedTextureInformation::reuploadTexture, [this](const QString& textureLocation, const TextureLogic::Texture *texture)
            {
                int index = 0;

                for(const auto &i : textureBank->getTexturesTextureInfo({}))
                {
                    if(texture == &i)
                    {
                        break;
                    }

                    index += 1;
                }

                textureBank->reuploadTexture(textureLocation, {});

                selectedTextureInformation->setTexture(&textureBank->getTexturesTextureInfo({})[index]);
            });
        }

        void TextureInfoScrollArea::selectedTextureModified()
        {
            selectedTextureInformation->selectedTextureModified();
        }

        void TextureInfoScrollArea::setCentralWidget()
        {
            setWidget(selectedTextureInformation);
        }

        void TextureInfoScrollArea::setTexture(const TextureLogic::Texture *texture, AccessRestriction::PassKey<TextureLogic::TextureBank>)
        {
            selectedTextureInformation->setTexture(texture);
        }

        void TextureInfoScrollArea::setTextureBankReference(TextureLogic::TextureBank *textureBank)
        {
            if(this->textureBank == nullptr)
            {
                this->textureBank = textureBank;
            }
        }
    }
}