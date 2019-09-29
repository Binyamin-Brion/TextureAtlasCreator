//
// Created by BinyBrion on 2019-09-26.
//

#ifndef TEXTUREATLASCREATOR_RENDERAREA_H
#define TEXTUREATLASCREATOR_RENDERAREA_H

#include <QtWidgets/QWidget>
#include "CurrentTextureImage.h"
#include "TextureLogic/Zoom.h"
#include "PaintFunctions/Brush.h"

namespace TextureLogic
{
    class Texture;
}

namespace GUI
{
    namespace CurrentTexture
    {
        class RenderArea : public QWidget
        {
            Q_OBJECT

            public:
                explicit RenderArea(CurrentTextureImage currentTextureImage, QWidget *parent = nullptr);
                void paintEvent(QPaintEvent *event) override;
                void setTexture(TextureLogic::Texture *texture);

            signals:
                void repaintSelectedTexture();

            private:
                TextureLogic::Texture *texture = nullptr;
                CurrentTextureImage currentTextureImage;
                TextureLogic::Zoom currentZoom;

                PaintFunctions::Brush brush;
        };
    }
}

#endif //TEXTUREATLASCREATOR_RENDERAREA_H
