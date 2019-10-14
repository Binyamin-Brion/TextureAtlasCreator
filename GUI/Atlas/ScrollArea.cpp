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
        ScrollArea::ScrollArea(QSize atlasSize, QImage::Format atlasFormat, QWidget *parent)
                                : QScrollArea{parent}, atlasWidget{new AtlasWidget{atlasSize, atlasFormat, this}}
        {
            setLayout(new QHBoxLayout{this});

            setWidget(atlasWidget);
       }

        void ScrollArea::addTexture(const TextureLogic::Texture &texture)
        {
            atlasWidget->textureButtonPressed(texture);
        }

        QImage::Format ScrollArea::getAtlasFormat() const
        {
            return atlasWidget->getAtlasFormat();
        }

        void ScrollArea::scrollContentsBy(int dx, int dy)
        {
            QScrollArea::scrollContentsBy(dx, dy);

            atlasWidget->translateViewPort(dx, dy);
        }

        void ScrollArea::removeTexture(const TextureLogic::Texture *texture)
        {
            atlasWidget->removeTexture(texture);
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

        void ScrollArea::updateTextureReferences(const std::vector<std::pair<std::vector<TextureLogic::Texture>, std::vector<unsigned int>>> &textures)
        {
            atlasWidget->updateTextureReferences(textures);
        }
    }
}
