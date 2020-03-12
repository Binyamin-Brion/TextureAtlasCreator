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
        // Beginning of public functions

        TextureInfoScrollArea::TextureInfoScrollArea(QWidget *parent)
                                :
                                    QScrollArea{parent},
                                    selectedTextureInformation{new SelectedTextureInformation{this}}
        {
            setWidgetResizable(true);

            connect(selectedTextureInformation, SIGNAL(newIntersectionBorderWidth(TextureLogic::Texture*, TextureLogic::Zoom, unsigned int)),
                    this, SLOT(changeIntersectionBorderWidth(TextureLogic::Texture*, TextureLogic::Zoom, unsigned int)));

            connect(selectedTextureInformation, &SelectedTextureInformation::newSelectionBorderWidth, [this]
                    (TextureLogic::Texture *texture, TextureLogic::Zoom zoom, unsigned int newBorderWidth)
            {
                textureBank->setSelectionBorderWidth(texture, zoom, newBorderWidth);
            });

            connect(selectedTextureInformation, &SelectedTextureInformation::updateTextureButtonLocation, [this](const QString &previousLocation, const QString &newLocation) { textureBank->updateTextureButtonLocation(previousLocation, newLocation); });
        }

        void TextureInfoScrollArea::setCentralWidget()
        {
            // For some reason calling this in the constructor causes the program to crash. Hence this is called by the
            // mainwindow after the object has been created.
            setWidget(selectedTextureInformation);
        }

        void TextureInfoScrollArea::selectedTextureModified()
        {
            // The name of the new texture has to be updated to include the words 'modified'.
            selectedTextureInformation->selectedTextureModified();
        }

        void TextureInfoScrollArea::setTexture(const TextureLogic::Texture *texture, AccessRestriction::PassKey<TextureLogic::TextureBank>)
        {
            selectedTextureInformation->setTexture(texture);
        }

        void TextureInfoScrollArea::setTextureBankReference(TextureLogic::TextureBank *textureBank)
        {
            // Logically this function should only be called once; this check enforces that idea
            if(this->textureBank == nullptr)
            {
                this->textureBank = textureBank;
            }
        }

        // Beginning of private slots

        void TextureInfoScrollArea::changeIntersectionBorderWidth(TextureLogic::Texture *texture, TextureLogic::Zoom zoom, unsigned int newBorderWidth)
        {
            // If a new intersection border width was specified, tell the texture bank to update that
            // texture; however, after the texture bank tries to update the width and that operation cannot be done,
            // then update the line edit in SelectedTextureInformation to display the older border width

            unsigned int previousBorderWidth = texture->getIntersectionBorderWidth(zoom);

            if(textureBank->setIntersectionBorderWidth(texture, zoom, newBorderWidth))
            {
                selectedTextureInformation->setIntersectionWidthLineEdit(previousBorderWidth);
            }
        }

        void TextureInfoScrollArea::reuploadTexture(const QString &textureLocation, const TextureLogic::Texture *texture, unsigned int intersectionBorderWidth, unsigned int selectionBorderWidth)
        {
            // When a texture was saved, the old one has to be uploaded. This means that the texture vector may be reallocated and
            // so new the reference to the contents of those vector have to be updated.

            unsigned int formatIndex = TextureHelperFunctions::indexFormat(selectedTextureInformation->getSelectedTextureFormat(), true);

            // The texture within the texture vector can still be safely accessed through an index. That index has to be found first.
            int index = 0;

            for(const auto &i : textureBank->getTextures()[formatIndex].first)
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
            selectedTextureInformation->setTexture(&textureBank->getTextures()[formatIndex].first[index]);
        }
    }
}