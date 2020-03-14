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

        ::Atlas::AtlasInformationBundle ScrollArea::getCurrentAtlasInformation() const
        {
            return atlasWidget->getAtlasInformation();
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
            else if(event->key() == Qt::Key_Z)
            {
                // Can't get keyboard shortcuts to work when this function exists. Could use keyboard shortcuts
                // instead of this function for all keyboard input, but the state of each key still needs to be
                // tracked. To do this with shortcuts would involve a lot of slots, one for each shortcut. This
                // is because shortcuts can't pass information, such as a shortcut identifier, to a slot, which would
                // allow one slot to be used.
                atlasWidget->undoTextureMovement();
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

            // To prevent control issues if the user zooms in while a texture is selected, and the zoom in is not valid
            // because the texture is outside of the viewport, this variable has to be reset. If it is not, after the user
            // gets a message about an invalid zoom, the wheel events continue to erroneously attempt to process a wheel request
            // as a zoom, even if user is not on control key anymore. This action means every time a user zooms, they have
            // to repress the control key. This is better than unexpected behaviour.
            controlKeyDown = false;
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
