//
// Created by BinyBrion on 2019-09-26.
//

#include <QtGui/QPainter>
#include "RenderArea.h"
#include "TextureLogic/Texture.h"

#include <QMouseEvent>

namespace GUI
{
    namespace CurrentTexture
    {
        RenderArea::RenderArea(CurrentTextureImage currentTextureImage, QWidget *parent) : QWidget{parent}, currentTextureImage{currentTextureImage}, previousMousePosition{-1, -1}
        {
            currentZoom = TextureLogic::Zoom::Normal;

            setFocusPolicy(Qt::StrongFocus);
            setMouseTracking(true);

            brush.setPaintTypeSolid(QSize{25, 25}, QColor{255, 255, 255});
        }

        void RenderArea::mouseMoveEvent(QMouseEvent *event)
        {
            if(texture != nullptr)
            {
                if(leftMouseButtonDown)
                {
                    applyBrush(event->pos());
                }

                previousMousePosition = event->pos();

                QWidget::repaint();
            }
        }

        void RenderArea::mousePressEvent(QMouseEvent *event)
        {
            if(event->button() == Qt::LeftButton)
            {
                applyBrush(event->pos());

                QWidget::repaint();

                leftMouseButtonDown = true;
            }
        }

        void RenderArea::mouseReleaseEvent(QMouseEvent *event)
        {
            if(event->button() == Qt::LeftButton)
            {
                leftMouseButtonDown = false;
            }
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

                if(previousMousePosition.x() != -1 && previousMousePosition.y() != -1)
                {
                    painter.drawImage(previousMousePosition.x() - brush.getPaintImage().width() / 2, previousMousePosition.y() - brush.getPaintImage().height() / 2, brush.getPaintImage());
                }
            }
        }

        void RenderArea::setTexture(TextureLogic::Texture *texture)
        {
            this->texture = texture;

            QWidget::repaint();
        }

        void RenderArea::applyBrush(QPoint mousePosition)
        {
            int halfTextureWidth = brush.getPaintImage().width() / 2;

            int halfTextureHeight = brush.getPaintImage().height() / 2;

            int adjustedMouseX;
            int adjustedMouseY;

            int adjustedPixelX;
            int adjustedPixelY;

            for(int i = -halfTextureWidth; i < halfTextureWidth; ++i)
            {
                adjustedMouseX = mousePosition.x() + i;

                if(adjustedMouseX < 0 || adjustedMouseX > texture->getImage(currentZoom).width()  - 1)
                {
                    continue;
                }

                adjustedPixelX = i + halfTextureWidth;

                for(int j = -halfTextureHeight; j < halfTextureHeight; ++j)
                {
                    adjustedMouseY = mousePosition.y() + j;

                    if(adjustedMouseY < 0 || adjustedMouseY > texture->getImage(currentZoom).height()  - 1)
                    {
                        continue;
                    }

                    adjustedPixelY = j + halfTextureHeight;

                    switch(currentTextureImage)
                    {
                        case CurrentTextureImage::SelectedTexture:
                            const_cast<QImage&>(texture->getImage(currentZoom)).setPixelColor(adjustedMouseX, adjustedMouseY, brush.getPaintImage().pixelColor(adjustedPixelX, adjustedPixelY));
                            break;

                        case CurrentTextureImage::SpecularTexture:
                            texture->getSpecularTexture(currentZoom, {}).setPixelColor(adjustedMouseX, adjustedMouseY, brush.getPaintImage().pixelColor(adjustedPixelX, adjustedPixelY));
                            break;
                    }
                }
            }
        }
    }
}