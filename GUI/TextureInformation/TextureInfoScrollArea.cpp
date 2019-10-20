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

            connect(selectedTextureInformation, &SelectedTextureInformation::newIntersectionBorderWidth, [this]
                    (TextureLogic::Texture *texture, TextureLogic::Zoom zoom, unsigned int newBorderWidth)
            {

                unsigned int previousBorderWidth = texture->getIntersectionBorderWidth(zoom);

                if(textureBank->setIntersectionBorderWidth(texture, zoom, newBorderWidth))
                {
                    selectedTextureInformation->setIntersectionWidthLineEdit(previousBorderWidth);
                }
            });

            connect(selectedTextureInformation, &SelectedTextureInformation::newSelectionBorderWidth, [this]
                    (TextureLogic::Texture *texture, TextureLogic::Zoom zoom, unsigned int newBorderWidth)
            {
                textureBank->setSelectionBorderWidth(texture, zoom, newBorderWidth);
            });

            connect(selectedTextureInformation, &SelectedTextureInformation::reuploadTexture, [this](const QString& textureLocation, const TextureLogic::Texture *texture,
                    unsigned int intersectionBorderWidth, unsigned int selectionBorderWidth)
            {
                unsigned int formatIndex = TextureHelperFunctions::indexFormat(selectedTextureInformation->getSelectedTextureFormat(), true);

                int index = 0;

                for(const auto &i : textureBank->getTexturesTextureInfo({})[formatIndex].first)
                {
                    if(texture == &i)
                    {
                        break;
                    }

                    index += 1;
                }

                textureBank->reuploadTexture(textureLocation, intersectionBorderWidth, selectionBorderWidth, {});

                selectedTextureInformation->setTexture(&textureBank->getTexturesTextureInfo({})[formatIndex].first[index]);
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