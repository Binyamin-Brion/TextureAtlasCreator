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
                                : QWidget{parent}, textureAtlas{std::make_unique<::Atlas::TextureAtlas>(atlasFormat)}
        {
            setMinimumSize(atlasSize);
            setMaximumSize(atlasSize);

            atlasDisplaySize = QSizeF{static_cast<float>(atlasSize.width()), static_cast<float>(atlasSize.height())};

            setFocusPolicy(Qt::StrongFocus);
            setMouseTracking(true);

            textureAtlas->setAtlasWidgetReference(this);

            viewPortOffset = QPoint{0, 0};
        }

        QImage::Format AtlasWidget::getAtlasFormat() const
        {
            return textureAtlas->getAtlasFormat();
        }

        ::Atlas::AtlasInformationBundle AtlasWidget::getAtlasInformation() const
        {
            return ::Atlas::AtlasInformationBundle{textureAtlas->getAtlasFormat(), textureAtlas->getNumberTextures(), textureAtlas->getPercentageAtlasUsed()};
        }

        void AtlasWidget::exportTexture()
        {
            QString newFileLocation = QFileDialog::getSaveFileName(this, tr("Save Image"), "", tr(""));

            if(newFileLocation.isEmpty())
            {
                return;
            }

            // Export as a png image, to make sure no quality is lost from the texture atlas. User can convert to
            // different formats if desired using external tools.

            newFileLocation += ".png";

            if(!textureAtlas->exportImage(newFileLocation))
            {
                QMessageBox::critical(this, "Error Exporting Image", "Failed to save the texture atlas.", QMessageBox::Ok);
            }
        }

        void AtlasWidget::keyPressed(QKeyEvent *event)
        {
            if(event->key() == Qt::Key_Delete)
            {
                textureAtlas->keyPressed(Qt::Key_Delete);

                QWidget::repaint();

                emit currentAtlasInformationChanged(::Atlas::AtlasInformationBundle{textureAtlas->getAtlasFormat(), textureAtlas->getNumberTextures(), textureAtlas->getPercentageAtlasUsed()});
            }
        }

        void AtlasWidget::mouseMoveEvent(QMouseEvent *event)
        {
            auto testPosAgainstAtlasBoundaries = textureAtlas->getAtlasSize();

            int mouseX = event->x();

            int mouseY = event->y();

            if(textureAtlas->textureSelected()) // A texture is selected
            {
                // See fn moveCursorToViewPort for a description of what this does

                if(moveCursorToViewPort(mouseX, mouseY))
                {
                    return;
                }

               // setCursor(Qt::BlankCursor);

                bool resetCursorPositionX = false;

                bool resetCursorPositionY = false;

                /*
                 *  The following explanation applies to the y dimension as well. The reasons for the loweBoundX and higherBoundX
                 *  is to make sure that textures are not moved off screen. There are two scenarios to consider:
                 *
                 *  1. The texture fits entirely within the viewport. In this case, to check if the texture is being moved offscreen,
                 *      check that the cursor does not move beyond the border +/- half the texture width, depending on which side of
                 *      the atlas the texture is approaching
                 *
                 *  2.  The texture does not fit entirely within the viewport. To handle this case, the limits on the texture are
                 *      those derived from the size of the viewport; the exact answer depends on what side of the texture is being checked.
                 *      For example, if the texture is being moved to the left, the stop the cursor when the texture is attempted to being moved
                 *      off screen, limit the position of the cursor to the width of the viewport.
                 *
                 *  These cases differ from the above if statements for making sure the cursor does not go offscreen the following code
                 *  deals with the texture placement behaviour when the borders of the atlas are within the viewport.
                 */

                int lowerBoundX = textureAtlas->getSelectedTextureSize().width() / 2 > (viewPort.width() - 5) ? (viewPort.width() - 5) : textureAtlas->getSelectedTextureSize().width() / 2;

                int upperBoundX;

                if(textureAtlas->getSelectedTextureSize().width() / 2 > (viewPort.width() - 5))
                {
                    upperBoundX = (width() + (width() - viewPort.width())) / 2;
                }
                else
                {
                    upperBoundX = testPosAgainstAtlasBoundaries.width() - textureAtlas->getSelectedTextureSize().width() / 2;
                }

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

                int lowerBoundY = textureAtlas->getSelectedTextureSize().height() / 2 > (viewPort.height() - 5) ? (viewPort.height() - 5) : textureAtlas->getSelectedTextureSize().height() / 2;

                int upperBoundY;

                if(textureAtlas->getSelectedTextureSize().height() / 2 > (viewPort.height() - 5))
                {
                    upperBoundY = (height() + (height() - viewPort.height())) / 2;
                }
                else
                {
                    upperBoundY = testPosAgainstAtlasBoundaries.height() - textureAtlas->getSelectedTextureSize().height() / 2;
                }

                if(mouseY < lowerBoundY) // Cursor trying to move texture offscreen to the top
                {
                    mouseY = lowerBoundY;

                    resetCursorPositionY = true;
                }
                else if(mouseY > upperBoundY)
                {
                    mouseY = upperBoundY;

                    resetCursorPositionY = true;
                }

                // X and Y dimensions are handled separately as, for example, if the cursor is pushing the texture offscreen to the left,
                // it does not mean that the y position of the cursor is pushing the texture offscreen

                if(resetCursorPositionX || resetCursorPositionY)
                {
                    QCursor c = cursor();

                    c.setPos(mapToGlobal(QPoint{mouseX, mouseY}));

                    setCursor(c);
                }

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

        void AtlasWidget::textureButtonPressed(const TextureLogic::Texture &texture)
        {
            try
            {
                textureAtlas->setSelectedTexture(texture);
            }
            catch(std::runtime_error &e)
            {
                QMessageBox::warning(this, tr("Error: Texture Already Loaded"), e.what(), QMessageBox::Ok);
            }
        }

        bool AtlasWidget::setIntersectionBorderWidth(TextureLogic::Texture *texture)
        {
            bool result = textureAtlas->resizeIntersectionBorderWidth(texture);

            QWidget::repaint();

            return result;
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

        void AtlasWidget::updateTextureReferences(const std::vector<std::pair<std::vector<TextureLogic::Texture>, std::vector<unsigned int>>> &textures)
        {
            textureAtlas->textureLoaded(textures);
        }

        void AtlasWidget::wheelEvent(QWheelEvent *event)
        {
            QWidget::wheelEvent(event);

            if(controlKeyDown)
            {
                if(event->angleDelta().y() > 0)
                {
                    textureAtlas->zoomIn();

                    QWidget::repaint();
                }
                else if(event->angleDelta().y() < 0)
                {
                    textureAtlas->zoomOut();

                    QWidget::repaint();
                }
            }
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

        AtlasWidget::~AtlasWidget()
        {

        }

        void AtlasWidget::removeTexture(const TextureLogic::Texture *texture)
        {
            textureAtlas->removeTexture(texture);

            QWidget::repaint();
        }

        // Update the size of this widget to reflect the fact that the user has zoom in or out

        void AtlasWidget::resizeAtlasFactor(float factor)
        {
            atlasDisplaySize *= factor;

            setMinimumSize(atlasDisplaySize.width(), atlasDisplaySize.height());
            setMaximumSize(atlasDisplaySize.width(), atlasDisplaySize.height());
        }

        void AtlasWidget::resizeEvent(QResizeEvent *event)
        {
            QWidget::resizeEvent(event);

            textureAtlas->setAtlasSize(event->size());
        }

        // This function is used by the TextureAtlas to move the cursor when a texture is about to be dragged.
        // This prevents the texture from jumping around the texture has begun to be moved, for the most part:
        // TODO: Figure out why the first time the texture is moved, it still jumps, but not for subsequent texture drags

        void AtlasWidget::moveMouseTo(int x, int y)
        {
            int mouseX = x;

            int mouseY = y;

            // See fn moveCursorToViewPort for a description of what this does.
            // Unlikely this call will change mouseX, or mouseY, but it is a precaution

            moveCursorToViewPort(mouseX, mouseY);

            QCursor c = cursor();

            c.setPos(mapToGlobal(QPoint{x, y}));

            setCursor(c);
        }

        bool AtlasWidget::moveCursorToViewPort(int &mouseX, int &mouseY)
        {
            /*
            *  The following four if statements make sure that the cursor cannot be moved off the widget containing
            *  the textures IF a texture is currently being dragged.
            *
            *  For example, suppose the viewport of the scroll area is in the middle of the widget within the scroll area.
            *
            *  If a texture is being moved, without any restrictions, the texture could continue being moved, say to the left,
            *  even though the cursor has already left the widget holding the textures.
            *
            *  With the restrictions of the if statements, as soon as the cursor tries to move out of the viewport,
            *  it is moved back to the position right before it left.
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