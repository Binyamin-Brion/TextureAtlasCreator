//
// Created by BinyBrion on 2019-09-15.
//

#include <QtWidgets/QHBoxLayout>
#include "ScrollArea.h"

#include "TextureButtonArea.h"

namespace GUI
{
    namespace LoadResults
    {
        ScrollArea::ScrollArea(QWidget *parent) : QScrollArea{parent}
        {
            setLayout(new QHBoxLayout);

            if(textureButtonArea != nullptr)
            {
                return;
            }

            textureButtonArea = new TextureButtonArea{this};

            setWidget(textureButtonArea);
        }

        void ScrollArea::addTextureButton(const QString &textureLocation)
        {
            textureButtonArea->addTextureButton(textureLocation);
        }

        void ScrollArea::setTextureBankReference(const TextureLogic::TextureBank *textureBank)
        {
            textureButtonArea->setTextureBankReference(const_cast<TextureLogic::TextureBank*>(textureBank));
        }
    }
}