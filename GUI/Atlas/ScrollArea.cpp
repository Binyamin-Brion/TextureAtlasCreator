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

            connect(this, SIGNAL(passTextureButtonInfo(const TextureLogic::Texture&)), atlasWidget, SLOT(textureButtonPressed(const TextureLogic::Texture&)));
        }

        void ScrollArea::updateTextureReferences(const std::vector<TextureLogic::Texture> &textures)
        {
            atlasWidget->updateTextureReferences(textures);
        }

        void ScrollArea::textureButtonPressed(const TextureLogic::Texture &texture)
        {
            emit passTextureButtonInfo(texture);
        }
    }
}
