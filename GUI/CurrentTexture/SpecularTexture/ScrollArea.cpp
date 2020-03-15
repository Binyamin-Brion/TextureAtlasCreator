//
// Created by binybrion on 3/12/20.
//

#include "ScrollArea.h"
#include "DisplayArea.h"
#include <QKeyEvent>

namespace GUI
{
    namespace CurrentTexture
    {
        namespace SpecularTexture
        {
            ScrollArea::ScrollArea(QWidget *parent)
                    :
                        QScrollArea(parent),
                        displayArea{new DisplayArea{this}}
            {
                setWidget(displayArea);
            }

            void ScrollArea::enterEvent(QEvent *event)
            {
                QWidget::enterEvent(event);

                QWidget::grabKeyboard();
            }

            void ScrollArea::keyPressEvent(QKeyEvent *event)
            {
                if (event->key() == Qt::Key_Control)
                {
                    controlKeyDown = true;
                }
            }

            void ScrollArea::keyReleaseEvent(QKeyEvent *event)
            {
                if (event->key() == Qt::Key_Control)
                {
                    controlKeyDown = false;
                }
            }

            void ScrollArea::leaveEvent(QEvent *event)
            {
                QScrollArea::leaveEvent(event);

                QWidget::releaseKeyboard();
            }

            void ScrollArea::repaintSelectedTexture()
            {
                displayArea->repaintSpecularTexture();
            }

            void ScrollArea::setSpecularTexture(const TextureLogic::Texture *texture)
            {
                displayArea->setSpecularTexture(texture);

            }

            void ScrollArea::wheelEvent(QWheelEvent *event)
            {
                // If the control key is down, then change the zoom. If the user moved the wheel away from themselves (forwards),
                // then zoom in; otherwise zoom out. The render area ensures that the texture with the new zoom is painted.
                if (controlKeyDown)
                {
                    if (event->angleDelta().y() > 0)
                    {
                        displayArea->zoomIn();

                        emit zoomChanged(displayArea->getZoom());

                        QWidget::repaint();

                    } else if (event->angleDelta().y() < 0)
                    {
                        displayArea->zoomOut();

                        emit zoomChanged(displayArea->getZoom());

                        QWidget::repaint();
                    }
                }
                else
                {
                    // Otherwise process the event as normal, which would move the scroll bars.
                    QScrollArea::wheelEvent(event);
                }

                // See GUI/Atlas/ScrollArea.cpp for explanation of why this is done
                controlKeyDown = false;
            }
        }
    }
}
