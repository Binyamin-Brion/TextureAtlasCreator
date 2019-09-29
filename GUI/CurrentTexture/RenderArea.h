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
                void mouseMoveEvent(QMouseEvent *event) override;
                void mousePressEvent(QMouseEvent *event) override;
                void mouseReleaseEvent(QMouseEvent *event) override;
                void paintEvent(QPaintEvent *event) override;
                void setTexture(TextureLogic::Texture *texture);

            signals:
                void repaintSelectedTexture();

            private:
                void applyBrush(QPoint mousePosition);

                TextureLogic::Texture *texture = nullptr;
                CurrentTextureImage currentTextureImage;
                TextureLogic::Zoom currentZoom;

                PaintFunctions::Brush brush;

                QPoint previousMousePosition;
                bool leftMouseButtonDown = false;
        };
    }
}

#endif //TEXTUREATLASCREATOR_RENDERAREA_H
