//
// Created by BinyBrion on 2019-09-17.
//

#include "AtlasWidget.h"
#include "Atlas/TextureAtlas.h"

#include <QCoreApplication>
#include <QPainter>
#include <QMouseEvent>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QFileDialog>

namespace GUI
{
    namespace Atlas
    {
        AtlasWidget::AtlasWidget(QSize atlasSize, QImage::Format atlasFormat, QWidget *parent)
                        :
                            QWidget{parent},
                            textureAtlas{std::make_unique<::Atlas::TextureAtlas>(atlasFormat)}
        {
            // The size of this widget is the size of the atlas. By setting the restrictions on the widget,
            // the resizeEvent() function will be called, resizing the texture atlas to the size of this widget
            setMinimumSize(atlasSize);
            setMaximumSize(atlasSize);

            // Widget needs to track cursor if a texture is being dragged around
            setMouseTracking(true);

            // The texture atlas needs to notify this widget of how much to change its size based off of the next
            // zoom value (when user requests to change zoom level)
            textureAtlas->setAtlasWidgetReference(this);

            // At the start of the program, the scroll bars of the scroll area holding this widget are not moved,
            // meaning the viewport through which this viewport is looked at is not moved
            viewPortOffset = QPoint{0, 0};
        }

        bool AtlasWidget::checkTextureNotWithinViewPort(QSize textureSize)
        {
            if(viewPort.width() < textureSize.width() || viewPort.height() < textureSize.height())
            {
                std::string errorMessage;

                errorMessage += "The texture cannot be added or selected as it is too big for the viewport. You must zoom out.\n\n";
                errorMessage += "Viewport Size: " + std::to_string(viewPort.width()) + " x " + std::to_string(viewPort.height()) + "\n";
                errorMessage += "Texture size: " + std::to_string(textureSize.width()) + " x " + std::to_string(textureSize.height());

                QMessageBox::warning(this, tr("Error: Texture Too Large"), errorMessage.c_str(), QMessageBox::Ok);

                return true;
            }

            return false;
        }

        QImage::Format AtlasWidget::getAtlasFormat() const
        {
            return textureAtlas->getAtlasFormat();
        }

        ::Atlas::AtlasInformationBundle AtlasWidget::getAtlasInformation() const
        {
            return ::Atlas::AtlasInformationBundle{textureAtlas->getAtlasFormat(), textureAtlas->getNumberTextures(), textureAtlas->getPercentageAtlasUsed()};
        }

        bool AtlasWidget::getUnsavedChanges()
        {
            return textureAtlas->getUnsavedChanges();
        }

        void AtlasWidget::exportTexture()
        {
            // Tried to have option to save as png in file dialog, but it doesn't show up for some reason. Have to do
            // this manually in code later in the function
            QString newFileLocation = QFileDialog::getSaveFileName(this, tr("Save Image"), QDir::homePath());

            // If no location is selected to save to, then do not attempt to export the atlas
            if(newFileLocation.isEmpty())
            {
                return;
            }

            QString newFileLocationSpecular = newFileLocation;

            // Export as png file for maximum quality.
            if(!newFileLocation.contains(".png"))
            {
                newFileLocation += ".png";
                newFileLocationSpecular +=  "_Specular.png";
            }
            else
            {
                newFileLocationSpecular = newFileLocationSpecular.remove(".png");
                newFileLocationSpecular += "_Specular.png";
            }

            if(!checkValidLocation(newFileLocation))
            {
                // Abort operation if user decided to cancel operation after receiving notice that the location they chose
                // already exists.
                return;
            }

            if(!checkValidLocation(newFileLocationSpecular))
            {
                // Same logic as the above if-block
                return;
            }

            // Export as a png image, to make sure no quality is lost from the texture atlas. User can convert to
            // different formats if desired using external tools.

            if(!textureAtlas->exportImage(newFileLocation, newFileLocationSpecular))
            {
                QMessageBox::critical(this, "Error Exporting Image", "Failed to save the texture atlas.", QMessageBox::Ok);
            }
        }

        void AtlasWidget::keyPressed(QKeyEvent *event)
        {
            if(event->key() == Qt::Key_Delete)
            {
                // This will cause to remove the selected texture from the texture atlas, if there is one. If no texture is selected,
                // then no action is taken by the texture atlas
                textureAtlas->keyPressed(Qt::Key_Delete);

                // Removing texture from atlas changes the atlas visually, and this change should be visible immediately
                QWidget::repaint();

                // Removing texture also changes the statistics of the texture atlas, which should be reflected in the GUI
                emit currentAtlasInformationChanged(::Atlas::AtlasInformationBundle{textureAtlas->getAtlasFormat(), textureAtlas->getNumberTextures(), textureAtlas->getPercentageAtlasUsed()});
            }
            else if(event->key() == Qt::Key_Escape)
            {
                // This will notify the texture atlas the current texture that might be added to the texture atlas will no longer
                // be added.
                textureAtlas->keyPressed(Qt::Key_Escape);

                // This also changes what is drawn around the cursor, as no texture is now drawn. This change should visible immediately
                QWidget::repaint();
            }
        }

        void AtlasWidget::mouseMoveEvent(QMouseEvent *event)
        {
            auto atlasSize = textureAtlas->getAtlasSize();

            int mouseX = event->x();

            int mouseY = event->y();

            if(textureAtlas->textureSelected()) // A texture is selected
            {
                // See lockCursorToViewPort() for a description of what this does
                if(lockCursorToViewPort(mouseX, mouseY))
                {
                    return;
                }

                bool resetCursorPositionX = false;

                bool resetCursorPositionY = false;

                /*
                 * When moving the texture around, check that the cursor does not move beyond the border +/- half
                 * the texture width, depending on which side of the atlas the texture is approaching
                 */

                int lowerBoundX = textureAtlas->getSelectedTextureSize().width() / 2;

                int upperBoundX = atlasSize.width() - textureAtlas->getSelectedTextureSize().width() / 2;

                if(mouseX < lowerBoundX) // Cursor trying to move texture offscreen to the left
                {
                    mouseX = lowerBoundX;

                    resetCursorPositionX = true;
                }
                else if(mouseX > upperBoundX) // Cursor trying to move texture offscreen to the right
                {
                    mouseX = upperBoundX;

                    resetCursorPositionX = true;
                }

                int lowerBoundY = textureAtlas->getSelectedTextureSize().height() / 2;

                int upperBoundY = atlasSize.height() - textureAtlas->getSelectedTextureSize().height() / 2;

                if(mouseY < lowerBoundY) // Cursor trying to move texture offscreen to the top
                {
                    mouseY = lowerBoundY;

                    resetCursorPositionY = true;
                }
                else if(mouseY > upperBoundY) // Cursor trying to move texture to the bottom
                {
                    mouseY = upperBoundY;

                    resetCursorPositionY = true;
                }

                // X and Y dimensions are handled separately as, for example, if the cursor is pushing the texture offscreen to the left,
                // it does not mean that the y position of the cursor is pushing the texture offscreen. Note that even if the new texture
                // position is invalid only along one dimension, then it is still considered invalid along both dimensions.
                if(resetCursorPositionX || resetCursorPositionY)
                {
                    // Move the cursor back to a valid position after mapping the valid reset position relative to the widget
                    QCursor c = cursor();

                    c.setPos(mapToGlobal(QPoint{mouseX, mouseY}));

                    setCursor(c);
                }

                // Keep track of what the most recent valid mouse position is in case a future invalid move has to be reverted.
                // See lockCursorToViewPort()
                previousMouseCoords = QPoint{mouseX, mouseY};
            }

            textureAtlas->mouseMoved(mouseX, mouseY);

            QWidget::repaint();
        }

        void AtlasWidget::mousePressEvent(QMouseEvent *event)
        {
            if(event->button() == Qt::LeftButton)
            {
                textureAtlas->mouseClicked(event->x(), event->y(),  event->button());

                QWidget::repaint();
            }
        }

        void AtlasWidget::mouseReleaseEvent(QMouseEvent *event)
        {
            setCursor(Qt::CrossCursor);

            textureAtlas->mouseReleased(event->x(), event->y(), event->button());

            // Remember that a texture is only placed after the mouse button is released, and only at this point is the texture atlas modified
            emit currentAtlasInformationChanged(::Atlas::AtlasInformationBundle{textureAtlas->getAtlasFormat(), textureAtlas->getNumberTextures(), textureAtlas->getPercentageAtlasUsed()});

            QWidget::repaint();
        }

        void AtlasWidget::paintEvent(QPaintEvent *event)
        {
            QWidget::paintEvent(event);

            QPainter painter{this};

            textureAtlas->draw(painter);

            // Draw a rect showing the border of the texture atlas. If the user zooms out, this is useful to determine
            // how much available space is still left in the atlas
            painter.drawRect(0,0, width() - 1, height() - 1);
        }

        void AtlasWidget::textureButtonPressed(const TextureLogic::Texture &texture, QPoint position)
        {
            // It is taken that if QSize == {-1, -1} it means that the passed in texture will placed at a later point in time in the atlas.
            // Otherwise the position variable specifies the location of the passed in texture after reading from a project file.
            if(position.x() != -1 && position.y() != -1)
            {
                textureAtlas->addTextureWithPosition(texture, position);

                QWidget::repaint();

                return;
            }

            QSize atlasSize = textureAtlas->getAtlasSize();
            QSize textureSize = texture.getImage(textureAtlas->getCurrentZoom()).size();

            // A texture bigger than the texture atlas cannot be placed on the atlas
            if(atlasSize.width() < textureSize.width() || atlasSize.height() < textureSize.height())
            {
                std::string errorMessage;

                errorMessage += "The texture cannot be added as it is too big for the current atlas.\n\n";
                errorMessage += "Atlas Size: " + std::to_string(atlasSize.width()) + " x " + std::to_string(atlasSize.height()) + "\n";
                errorMessage += "Texture size: " + std::to_string(textureSize.width()) + " x " + std::to_string(textureSize.height());

                QMessageBox::warning(this, tr("Error: Texture Too Large"), errorMessage.c_str(), QMessageBox::Ok);

                return;
            }

            // Restriction is in-place that only textures that fit entirely within the viewport are placed.
            // This ensures that checks on valid cursor positions with an open new selected texture remain valid.
            if(checkTextureNotWithinViewPort(texture.getImage(textureAtlas->getCurrentZoom()).size()))
            {
                return;
            }

            try
            {
                textureAtlas->setSelectedTexture(texture);
            }
            catch(std::runtime_error &e)
            {
                QMessageBox::warning(this, tr("Error: Texture Already Loaded"), e.what(), QMessageBox::Ok);
            }
        }

        bool AtlasWidget::newIntersectionBorderWidthValid(TextureLogic::Texture *texture)
        {
            bool result = textureAtlas->newIntersectionBorderWidthValid(texture);

            QWidget::repaint();

            return result;
        }

        void AtlasWidget::saveAtlas(const QString &atlasName, const QString &saveLocation) const
        {
            textureAtlas->saveAtlas(atlasName, saveLocation);
        }

        void AtlasWidget::setViewPort(QSize viewPort)
        {
            this->viewPort = viewPort;
        }

        void AtlasWidget::setViewPortOffsetX(int value)
        {
            viewPortOffset.setX(value);
        }

        void AtlasWidget::setViewPortOffsetY(int value)
        {
            viewPortOffset.setY(value);
        }

        void AtlasWidget::setTextureBankReference(TextureLogic::TextureBank *textureBank)
        {
            textureAtlas->setTextureBankReference(textureBank);
        }

        void AtlasWidget::updateTextureReferences()
        {
            textureAtlas->textureLoaded();
        }

        void AtlasWidget::undoTextureMovement()
        {
            textureAtlas->undoTextureMovement();

            // A texture may have been moved; would be best if the texture is painted in its new location as soon as possible.
            QWidget::repaint();
        }

        void AtlasWidget::zoomIn()
        {
            textureAtlas->zoomIn();

            QWidget::repaint();
        }

        void AtlasWidget::zoomOut()
        {
            textureAtlas->zoomOut();

            QWidget::repaint();
        }

        void AtlasWidget::removeTexture(const TextureLogic::Texture *texture)
        {
            textureAtlas->removeTexture(texture);

            QWidget::repaint();
        }

        void AtlasWidget::resizeAtlasFactor(float factor)
        {
            // Update the size of this widget to reflect the fact that the user has zoom in or out
            QSize newAtlasSize = size() * factor;

            setMinimumSize(newAtlasSize.width(), newAtlasSize.height());
            setMaximumSize(newAtlasSize.width(), newAtlasSize.height());
        }

        void AtlasWidget::resizeEvent(QResizeEvent *event)
        {
            QWidget::resizeEvent(event);

            textureAtlas->setAtlasSize(event->size());
        }

        void AtlasWidget::moveMouseTo(int x, int y)
        {
            // This function is used by the TextureAtlas to move the cursor when a texture is about to be dragged.
            // This prevents the texture from jumping around the texture has begun to be move. This is because movement
            // is done relative to the centre of the texture; if the cursor is not moved to the centre of the texture
            // then the next movement could cause a jump in texture position.

            int mouseX = x;

            int mouseY = y;

            // See lockCursorToViewPort() for a description of what this does.
            // Unlikely this call will change mouseX, or mouseY, but it is a precaution
            lockCursorToViewPort(mouseX, mouseY);

            QCursor c = cursor();

            c.setPos(mapToGlobal(QPoint{x, y}));

            setCursor(c);
        }

        bool AtlasWidget::checkValidLocation(QString &chosenLocation)
        {
            while(true)
            {
                if(QFile::exists(chosenLocation))
                {
                    int response = QMessageBox::warning(this, "Overwrite Confirmation", "The location to be used for saving the texture: \n" +
                                                                                        chosenLocation + " already exists. Save to a new location?", QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

                    if(response == QMessageBox::Yes)
                    {
                        chosenLocation = QFileDialog::getSaveFileName(this, tr("Save Image"), "", tr(""));

                        chosenLocation += ".png";

                        continue;
                    }
                    else if(response == QMessageBox::Cancel)
                    {
                        QMessageBox::information(this, "Cancel Save Operation", "The save operation has been cancelled.", QMessageBox::Ok);

                        return false;
                    }
                }

                return true;
            }
        }

        bool AtlasWidget::lockCursorToViewPort(int &mouseX, int &mouseY)
        {
            /*
             *  The following four if statements make sure that the cursor cannot be moved off the widget containing
             *  the textures IF a texture is currently being dragged. This occurs in the following situation:
             *
             *  -------------------------------------------------------
             *  |          Texture        |                |          |
             *  |_________________________|   Viewport     |  Widget  |
             *  |                                          |          |
             *  |------------------------------------------|          |
             *  |-----------------------------------------------------|
             *
             *  If the texture is being moved to the bottom or down, it will eventually hit the borders of the viewport
             *  before the borders of the widget. In that case, the checks done in mouseMoveEvent() will not prevent the user from moving
             *  texture pass where it can be viewed, as the cursor is not locked to the viewport.
             *
             *  This function prevents that issue. As soon as the cursor is trying to move past the viewport when dragging
             *  a texture, it will be moved back to the border of the viewport. Thus the texture cannot be moved offscreen.
             *
             *  For example, suppose the viewport of the scroll area is in the middle of the widget within the scroll area.
             *
             *  If a texture is being moved, without any restrictions, the texture could continue being moved, say to the left,
             *  even though the cursor has already left the widget holding the textures.
             *
             */

            bool returnTrue = false;

            QCursor c = cursor();

            if(mouseX < viewPortOffset.x()) // Trying to move cursor off the atlas widget to the left
            {
                previousMouseCoords.setX(viewPortOffset.x());

                mouseX = previousMouseCoords.x();

                returnTrue = true;
            }

            if(mouseX >= viewPortOffset.x() + viewPort.width()) // Trying to move cursor off the atlas widget to the right
            {
                previousMouseCoords.setX(viewPort.width() + viewPortOffset.x());

                mouseX = previousMouseCoords.x();

                returnTrue = true;
            }

            if(mouseY < viewPortOffset.y()) // Trying to move cursor off the atlas widget upwards
            {
                previousMouseCoords.setY(viewPortOffset.y());

                mouseY = previousMouseCoords.y();

                returnTrue = true;
            }

            if(mouseY >= viewPortOffset.y() + viewPort.height()) // Trying to move cursor off the atlas widget to the bottom
            {
                previousMouseCoords.setY(viewPort.height() + viewPortOffset.y());

                mouseY = previousMouseCoords.y();

                returnTrue = true;
            }

            if(returnTrue)
            {
                c.setPos(mapToGlobal(previousMouseCoords));

                setCursor(c);

                return true;
            }

            return false;
        }
    }
}