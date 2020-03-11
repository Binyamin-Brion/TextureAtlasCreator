//
// Created by BinyBrion on 10/2/2019.
//

#include <QtWidgets/QHBoxLayout>
#include "ScrollArea.h"
#include "PaintArea.h"
#include <QKeyEvent>

namespace GUI
{
    namespace CurrentTexture
    {
        ScrollArea::ScrollArea(CurrentTextureImage currentTextureImage, QWidget *parent)
                    :
                QScrollArea{parent},
                paintArea{new PaintArea{currentTextureImage, this}}
        {
            setWidget(paintArea);

            // Forwards these signals to the the CurrentTextureTabWidget as this widget does not have the capabilities
            // to act upon receiving this signals
            connect(paintArea, &PaintArea::repaintSelectedTexture, [this]() { emit repaintSelectedTexture(); });

            connect(paintArea, &PaintArea::paintedSelectedTexture, [this]() { emit paintedSelectedTexture(); });
        }

        void ScrollArea::enterEvent(QEvent *event)
        {
            QScrollArea::enterEvent(event);

            QWidget::grabKeyboard();
        }

        const PaintFunctions::Brush &ScrollArea::getBrush() const
        {
            return paintArea->getBrush();
        }

        QImage::Format ScrollArea::getCurrentTextureFormat() const
        {
            return paintArea->getCurrentTextureFormat();
        }

        TextureLogic::Zoom ScrollArea::getZoom() const
        {
            return paintArea->getZoom();
        }

        void ScrollArea::keyPressEvent(QKeyEvent *event)
        {
            if(event->key() == Qt::Key_Control)
            {
                controlKeyDown = true;
            }
        }

        void ScrollArea::keyReleaseEvent(QKeyEvent *event)
        {
            if(event->key() == Qt::Key_Control)
            {
                controlKeyDown = false;
            }
        }

        void ScrollArea::leaveEvent(QEvent *event)
        {
            QScrollArea::leaveEvent(event);

            QWidget::releaseKeyboard();
        }

        void ScrollArea::setTexture(TextureLogic::Texture *texture)
        {
            paintArea->setTexture(texture);

            QWidget::repaint();
        }

        void ScrollArea::wheelEvent(QWheelEvent *event)
        {
            // If the control key is down, then change the zoom. If the user moved the wheel away from themselves (forwards),
            // then zoom in; otherwise zoom out. The render area ensures that the texture with the new zoom is painted.
            if(controlKeyDown)
            {
                if(event->angleDelta().y() > 0)
                {
                    paintArea->zoomIn();

                    emit zoomChanged(paintArea->getZoom());

                    QWidget::repaint();
                }
                else if(event->angleDelta().y() < 0)
                {
                    paintArea->zoomOut();

                    emit zoomChanged(paintArea->getZoom());

                    QWidget::repaint();
                }
            }
            else
            {
                // Otherwise process the event as normal, which would move the scroll bars.
                QScrollArea::wheelEvent(event);
            }
        }
    }
}