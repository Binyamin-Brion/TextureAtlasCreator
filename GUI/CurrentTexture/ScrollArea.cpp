//
// Created by BinyBrion on 10/2/2019.
//

#include <QtWidgets/QHBoxLayout>
#include "ScrollArea.h"
#include "RenderArea.h"
#include <QKeyEvent>

namespace GUI
{
    namespace CurrentTexture
    {
        ScrollArea::ScrollArea(CurrentTextureImage currentTextureImage, QWidget *parent)
                    :
                        QScrollArea{parent},
                        renderArea{new RenderArea{currentTextureImage, this}}
        {
            setWidget(renderArea);

            // Forwards these signals to the the CurrentTextureTabWidget as this widget does not have the capabilities
            // to act upon receiving this signals
            connect(renderArea, &RenderArea::repaintSelectedTexture, [this]() { emit repaintSelectedTexture(); });

            connect(renderArea, &RenderArea::paintedSelectedTexture, [this]() { emit paintedSelectedTexture(); });
        }

        void ScrollArea::enterEvent(QEvent *event)
        {
            QScrollArea::enterEvent(event);

            QWidget::grabKeyboard();
        }

        const PaintFunctions::Brush &ScrollArea::getBrush() const
        {
            return renderArea->getBrush();
        }

        QImage::Format ScrollArea::getCurrentTextureFormat() const
        {
            return renderArea->getCurrentTextureFormat();
        }

        TextureLogic::Zoom ScrollArea::getZoom() const
        {
            return renderArea->getZoom();
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
            renderArea->setTexture(texture);

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
                    renderArea->zoomIn();

                    emit zoomChanged(renderArea->getZoom());

                    QWidget::repaint();
                }
                else if(event->angleDelta().y() < 0)
                {
                    renderArea->zoomOut();

                    emit zoomChanged(renderArea->getZoom());

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