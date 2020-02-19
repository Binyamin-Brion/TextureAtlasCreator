//
// Created by BinyBrion on 2019-09-17.
//

#include <QtWidgets/QHBoxLayout>
#include "ScrollArea.h"

#include "AtlasWidget.h"
#include <QKeyEvent>
#include <QtWidgets/QShortcut>
#include <QScrollBar>
namespace GUI
{
    namespace Atlas
    {
        ScrollArea::ScrollArea(QSize atlasSize, QImage::Format atlasFormat, QWidget *parent)
                                : QScrollArea{parent}, atlasWidget{new AtlasWidget{atlasSize, atlasFormat, this}}
        {
            setLayout(new QHBoxLayout{this});

            setWidget(atlasWidget);

            new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Equal), this, SLOT(zoomIn()));

            new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Minus), this, SLOT(zoomOut()));

            connect(horizontalScrollBar(), &QScrollBar::valueChanged, [this](int newValue)
            {
                atlasWidget->setViewPortOffsetX(newValue);
            });

            connect(verticalScrollBar(), &QScrollBar::valueChanged, [this](int newValue)
            {
                atlasWidget->setViewPortOffsetY(newValue);
            });

            connect(atlasWidget, &AtlasWidget::currentAtlasInformationChanged, [this](::Atlas::AtlasInformationBundle information)
            {
                emit currentAtlasInformationChanged(information);
            });
        }

        void ScrollArea::addTexture(const TextureLogic::Texture &texture)
        {
            atlasWidget->textureButtonPressed(texture);
        }

        void ScrollArea::enterEvent(QEvent *event)
        {
            // The scroll area takes control of all keyboard input once the cursor has entered it
            // to ensure that the short cuts to change the zoom are received and are not given to another widget.
            // This is not strictly required, but it does not hurt.

            QWidget::grabKeyboard();
        }

        void ScrollArea::exportTexture()
        {
            atlasWidget->exportTexture();
        }

        QImage::Format ScrollArea::getAtlasFormat() const
        {
            return atlasWidget->getAtlasFormat();
        }

        void ScrollArea::keyPressEvent(QKeyEvent *event)
        {
            if(event->key() == Qt::Key_Control)
            {
                controlKeyDown = true;
            }

            atlasWidget->keyPressed(event);
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
            QWidget::releaseKeyboard();
        }

        ::Atlas::AtlasInformationBundle ScrollArea::getAtlasInformation() const
        {
            return atlasWidget->getAtlasInformation();
        }

        void ScrollArea::scrollContentsBy(int dx, int dy)
        {
            if(controlKeyDown)
            {
                return;
            }

            QScrollArea::scrollContentsBy(dx, dy);
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

        bool ScrollArea::newIntersectionBorderWidthValid(TextureLogic::Texture *texture)
        {
            return atlasWidget->newIntersectionBorderWidthValid(texture);
        }

        void ScrollArea::setTextureBankReference(TextureLogic::TextureBank *textureBank)
        {
            atlasWidget->setTextureBankReference(textureBank);
        }

        void ScrollArea::updateTextureReferences()
        {
            atlasWidget->updateTextureReferences();
        }

        void ScrollArea::wheelEvent(QWheelEvent *event)
        {
            if(controlKeyDown)
            {
                if(event->angleDelta().y() > 0)
                {
                    atlasWidget->zoomIn();

                    QWidget::repaint();
                }
                else if(event->angleDelta().y() < 0)
                {
                    atlasWidget->zoomOut();

                    QWidget::repaint();
                }
            }
            else
            {
                QScrollArea::wheelEvent(event);
            }
        }

        void ScrollArea::zoomIn()
        {
            atlasWidget->zoomIn();
        }

        void ScrollArea::zoomOut()
        {
            atlasWidget->zoomOut();
        }
    }
}
