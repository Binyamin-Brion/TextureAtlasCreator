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
        // Beginning of public functions

        ScrollArea::ScrollArea(QWidget *parent)
                    :
                        QScrollArea{parent}
        {
            textureButtonArea = new TextureButtonArea{this};

            setWidget(textureButtonArea);
        }

        void ScrollArea::addTextureButton(const QString &textureLocation, unsigned int intersectionBorderWidth, unsigned int selectionBorderWidth)
        {
            textureButtonArea->addTextureButton(textureLocation, intersectionBorderWidth, selectionBorderWidth);
        }

        void ScrollArea::deleteTextureButtons()
        {
            textureButtonArea->deleteTextureButtons();
        }

        const TextureButtonArea* ScrollArea::getTextureArea() const
        {
            return textureButtonArea;
        }

        void ScrollArea::setTextureBankReference(TextureLogic::TextureBank *textureBank)
        {
            textureButtonArea->setTextureBankReference(textureBank);
        }
    }
}