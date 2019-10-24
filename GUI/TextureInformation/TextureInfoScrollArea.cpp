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

            // If a new intersection border width was specified, tell the texture bank to update that
            // texture; however, after the texture bank tries to update the width and that operation cannot be done,
            // then update the line edit in SelectedTextureInformation to display the older border width

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

            // When a texture was saved, the old one has to be uploaded. This means that the texture vector may be reallocated and
            // so new the reference to the contents of those vector have to be updated.

            connect(selectedTextureInformation, &SelectedTextureInformation::reuploadTexture, [this](const QString& textureLocation, const TextureLogic::Texture *texture,
                    unsigned int intersectionBorderWidth, unsigned int selectionBorderWidth)
            {
                unsigned int formatIndex = TextureHelperFunctions::indexFormat(selectedTextureInformation->getSelectedTextureFormat(), true);

                // The texture within the texture vector can still be safely accessed through an index. That index has to be found first.

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

                // Update the reference to the currently selected texture to make sure that it still points to a valid texture in the texture vector,
                // in case it got reallocated.

                selectedTextureInformation->setTexture(&textureBank->getTexturesTextureInfo({})[formatIndex].first[index]);
            });
        }

        // This is called when the currently selected texture is modified in the CurrentTexture Render Area. The name
        // of the new texture has to be updated to include the words 'modified'.

        void TextureInfoScrollArea::selectedTextureModified()
        {
            selectedTextureInformation->selectedTextureModified();
        }

        // Usually this operation is done in the constructor; however, for some reason when this was done with this
        // scroll area a crash occurred. This is a work around; this is called from the mainwindow class.

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