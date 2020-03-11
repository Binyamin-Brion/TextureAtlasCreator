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
        // Beginning of public functions

        ScrollArea::ScrollArea(QSize atlasSize, QImage::Format atlasFormat, QWidget *parent)
                    :
                        QScrollArea{parent},
                        atlasWidget{new AtlasWidget{atlasSize, atlasFormat, this}}
        {
            // Make this scroll area display the widget holding the atlas
            setWidget(atlasWidget);

            // Keyboard shortcuts to zooming in and out of the atlas. Note that this will only work when the keyboard
            // focus is with this object; otherwise this object will never be aware that the users used these shortcuts
            new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Equal), this, SLOT(zoomIn()));

            new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Minus), this, SLOT(zoomOut()));

            // Connections to handle the user scrolling
            connect(horizontalScrollBar(), &QScrollBar::valueChanged, [this](int newValue) { atlasWidget->setViewPortOffsetX(newValue); });

            connect(verticalScrollBar(), &QScrollBar::valueChanged, [this](int newValue) { atlasWidget->setViewPortOffsetY(newValue); });

            // Connection for when the user modified the atlas displayed in this scroll area
            connect(atlasWidget, &AtlasWidget::currentAtlasInformationChanged, [this](::Atlas::AtlasInformationBundle information) { emit currentAtlasInformationChanged(information); });
        }

        void ScrollArea::addTexture(const TextureLogic::Texture &texture, QPoint position)
        {
            atlasWidget->textureButtonPressed(texture, position);
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

        bool ScrollArea::getUnsavedChanges() const
        {
            return atlasWidget->getUnsavedChanges();
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

        void ScrollArea::saveAtlas(const QString &atlasName, const QString &saveLocation) const
        {
            atlasWidget->saveAtlas(atlasName, saveLocation);
        }

        void ScrollArea::scrollContentsBy(int dx, int dy)
        {
            // If the control key is down, then the viewport of this scroll area is not to be changed; it is assumed
            // that the user wishes to change the atlas zoom. In that case, make sure inherited QScrollArea does not
            // processes the change in scrollbars should they occur for whatever reason
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

            // The atlas widget changes its calculation depending on how much of the atlas is currently visible.
            // Thus it needs to be aware of any changes to the viewport through which the atlas is being viewed.
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
            // If the control key is down, then change the zoom. If the user moved the wheel away from themselves (forwards),
            // then zoom in; otherwise zoom out. The atlas widget ensures that the atlas with the new zoom is painted.
            if(controlKeyDown)
            {
                if(event->angleDelta().y() > 0)
                {
                    atlasWidget->zoomIn();
                }
                else if(event->angleDelta().y() < 0)
                {
                    atlasWidget->zoomOut();
                }
            }
            else
            {
                // Otherwise process the event as normal, which would move the scroll bars.
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
