//
// Created by BinyBrion on 2019-09-17.
//

#include "AtlasWidget.h"
#include "Atlas/TextureAtlas.h"

#include <QPainter>

namespace GUI
{
    namespace Atlas
    {
        AtlasWidget::AtlasWidget(QSize atlasSize, QWidget *parent)
                                : QWidget{parent}, textureAtlas{std::make_unique<::Atlas::TextureAtlas>()}, size{atlasSize}
        {
            setMinimumSize(size);
            setMaximumSize(size);
        }

        void AtlasWidget::paintEvent(QPaintEvent *event)
        {
            QWidget::paintEvent(event);

            QPainter painter{this};

            textureAtlas->draw(painter);
        }

        void AtlasWidget::textureButtonPressed(const TextureLogic::Texture &texture)
        {
            textureAtlas->setSelectedTexture(texture);
        }

        void AtlasWidget::updateTextureReferences(const std::vector<TextureLogic::Texture> &textures)
        {
            textureAtlas->textureLoaded(textures);
        }
    }
}