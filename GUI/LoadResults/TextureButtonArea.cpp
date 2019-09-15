//
// Created by BinyBrion on 2019-09-15.
//

#include "TextureButtonArea.h"

#include "TextureButton.h"
#include "TextureLogic/TextureBank.h"

namespace GUI
{
    namespace LoadResults
    {

        TextureButtonArea::TextureButtonArea(QWidget *parent) : QWidget{parent}
        {
            setMinimumSize(1600, 900);
        }

        void TextureButtonArea::addTextureButton(const QString &textureLocation)
        {
            textureButtons.push_back(new TextureButton{textureLocation, this});

            connect(textureButtons.back(), SIGNAL(buttonClicked(const QString&)), this, SLOT(textureButtonClicked(const QString&)));
        }

        void TextureButtonArea::setTextureBankReference(TextureLogic::TextureBank *textureBank)
        {
            if(textureBank != nullptr)
            {
                return;
            }

            this->textureBank = textureBank;
        }

        void TextureButtonArea::textureButtonClicked(const QString &textureLocation)
        {
            textureBank->addImage(textureLocation, {});
        }
    }
}