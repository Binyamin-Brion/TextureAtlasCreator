//
// Created by BinyBrion on 2019-09-17.
//

#include <QtWidgets/QHBoxLayout>
#include "ScrollArea.h"

#include "AtlasWidget.h"

namespace GUI
{
    namespace Atlas
    {
        ScrollArea::ScrollArea(QSize atlasSize, QWidget *parent) : QScrollArea{parent}, atlasWidget{new AtlasWidget{atlasSize, this}}
        {
            setLayout(new QHBoxLayout{this});

            setWidget(atlasWidget);
       }

        void ScrollArea::addTexture(const TextureLogic::Texture &texture)
        {
            atlasWidget->textureButtonPressed(texture);
        }

        void ScrollArea::scrollContentsBy(int dx, int dy)
        {
            QScrollArea::scrollContentsBy(dx, dy);

            atlasWidget->translateViewPort(dx, dy);
        }

        void ScrollArea::repaintSelectedTexture()
        {
            atlasWidget->update();
        }

        void ScrollArea::resizeEvent(QResizeEvent *event)
        {
            QScrollArea::resizeEvent(event);

            atlasWidget->setViewPort(QSize{viewport()->width(), viewport()->height()});
        }

        void ScrollArea::setTextureBankReference(TextureLogic::TextureBank *textureBank)
        {
            atlasWidget->setTextureBankReference(textureBank);
        }

        void ScrollArea::updateTextureReferences(const std::vector<TextureLogic::Texture> &textures)
        {
            atlasWidget->updateTextureReferences(textures);
        }
    }
}
