//
// Created by BinyBrion on 2019-09-26.
//

#include <QtGui/QPainter>
#include "RenderArea.h"
#include "TextureLogic/Texture.h"

namespace GUI
{
    namespace CurrentTexture
    {
        RenderArea::RenderArea(CurrentTextureImage currentTextureImage, QWidget *parent) : QWidget{parent}, currentTextureImage{currentTextureImage}
        {
            currentZoom = TextureLogic::Zoom::Normal;

            brush.setPaintTypeSolid(QSize{5, 5}, QColor{255, 255, 255});
        }

        void RenderArea::paintEvent(QPaintEvent *event)
        {
            QPainter painter{this};

            if(texture != nullptr)
            {
                switch(currentTextureImage)
                {
                    case CurrentTextureImage::SelectedTexture:
                        painter.drawImage(QPoint{0, 0}, texture->getImage(currentZoom));
                        emit repaintSelectedTexture();
                        break;

                    case CurrentTextureImage::SpecularTexture:
                        painter.drawImage(QPoint{0, 0}, texture->getSpecularTexture(currentZoom, {}));
                        break;
                }
            }
        }

        void RenderArea::setTexture(TextureLogic::Texture *texture)
        {
            this->texture = texture;

            QWidget::repaint();
        }
    }
}