//
// Created by BinyBrion on 2019-09-26.
//

#include <QtGui/QPainter>
#include "PaintArea.h"
#include "TextureLogic/Texture.h"
#include "PaintFunctions/PaintHistoryCommand.h"

#include <QMouseEvent>
#include <QtWidgets/QShortcut>

namespace GUI
{
    namespace CurrentTexture
    {
        // Note: any check of "texture != nullptr" is to check that a texture is selected in the texture atlas; ie
        //       there is a texture to which paint operations can be applied to

        // Note: paintedSelectedTexture signal cannot be combined with repaintSelectedTexture signal as a repaint signal is sent
        //       once before the user has made any changes to the texture. This incorrectly prematurely updates the information
        //       in the GUI information panel

        PaintArea::PaintArea(CurrentTextureImage currentTextureImage, QWidget *parent)
                    :
                        QWidget{parent},
                        currentTextureImage{currentTextureImage},
                        previousMousePosition{-1, -1} // Before cursor is moved with texture selected, there is no previous cursor position
        {
            currentZoom = TextureLogic::Zoom::Normal;

            // Widget needs to emit mouse move events
            setMouseTracking(true);

            // Randomly chosen default brush size and colour
            brush.setPaintTypeSolid(currentZoom, QSize{25, 25}, QColor{255, 255, 255});

            new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Z), this, SLOT(undoPaintOperation()));
        }

        const PaintFunctions::Brush& PaintArea::getBrush() const
        {
            return brush;
        }

        QImage::Format PaintArea::getCurrentTextureFormat() const
        {
            if(texture == nullptr)
            {
                return QImage::Format_Invalid;
            }

            return textureFormat;
        }

        TextureLogic::Zoom PaintArea::getZoom() const
        {
            return currentZoom;
        }

        void PaintArea::mouseMoveEvent(QMouseEvent *event)
        {
            if(texture != nullptr)
            {
                if(leftMouseButtonDown)
                {
                    for(auto zoom : TextureLogic::AllZoomValues)
                    {
                        // Depending on the zoom, the position of the cursor for each zoom level. This has to be taken into
                        // account for the paint history so that if paint operations are undone at a different zoom level,
                        // the paint history will still be valid
                        float zoomFactor = TextureLogic::GetZoomValue(zoom) / TextureLogic::GetZoomValue(currentZoom);

                        paintTexture(zoom, event->pos() * zoomFactor, brush.getPaintImage(zoom), const_cast<QImage &>(getReferredToImage(zoom)), false);
                    }
                }

                previousMousePosition = event->pos();

                QWidget::repaint();
            }
        }

        void PaintArea::mousePressEvent(QMouseEvent *event)
        {
            if(event->button() == Qt::LeftButton)
            {
                if(texture != nullptr)
                {
                    // Note that if a user clicks left mouse button, paint operation is done once at the position of the cursor
                    for(auto zoom : TextureLogic::AllZoomValues)
                    {
                        // Same reason as mentioned in mouseMoveEvent()
                        float zoomFactor = TextureLogic::GetZoomValue(zoom) / TextureLogic::GetZoomValue(currentZoom);

                        paintTexture(zoom, event->pos() * zoomFactor, brush.getPaintImage(zoom), const_cast<QImage &>(getReferredToImage(zoom)), false);
                    }

                    QWidget::repaint();

                    leftMouseButtonDown = true;
                }
            }
        }

        void PaintArea::mouseReleaseEvent(QMouseEvent *event)
        {
            if(event->button() == Qt::LeftButton)
            {
                leftMouseButtonDown = false;

                // End of a paint sequence; the paint operations that make up the sequence have to be stored
                storePaintHistory();
            }
        }

        void PaintArea::paintEvent(QPaintEvent *event)
        {
            QPainter painter{this};

            if(texture != nullptr)
            {
                switch(currentTextureImage)
                {
                    case CurrentTextureImage::SelectedTexture:
                        painter.drawImage(QPoint{0, 0}, texture->getImage(currentZoom));

                        // Note that only this case has to force the atlas widget to redraw as if this case is entered,
                        // then the exact same texture shown in this widget is shown in the atlas widget. In order to
                        // synchronize the two, after modifying the texture here where the updates are seen visually immediately,
                        // the atlas widget has to be told to visually update the (same) texture it draws
                        emit repaintSelectedTexture();
                        break;

                    case CurrentTextureImage::SpecularTexture:
                        painter.drawImage(QPoint{0, 0}, texture->getSpecularTexture(currentZoom, {}));
                        break;
                }

                // If statement to prevent the brush from being drawn the first time a texture is selected; would be
                // a little bit weird to give a brush a position when the user hasn't moved cursor over render area with texture selected first
                if(previousMousePosition.x() != -1 && previousMousePosition.y() != -1)
                {
                    painter.drawImage(previousMousePosition.x() - brush.getPaintImage(currentZoom).width() / 2,
                            previousMousePosition.y() - brush.getPaintImage(currentZoom).height() / 2, brush.getPaintImage(currentZoom));
                }
            }
        }

        void PaintArea::setTexture(TextureLogic::Texture *texture)
        {
            this->texture = texture;

            if(texture != nullptr)
            {
                setMinimumSize(texture->getImage(currentZoom).size());
                setMaximumSize(texture->getImage(currentZoom).size());

                // The passed in zoom level should not matter here. currentZoom used here for consistency
                textureFormat = texture->getImage(currentZoom).format();
            }

            QWidget::repaint();
        }

        void PaintArea::zoomIn()
        {
            TextureLogic::Zoom oldZoom = currentZoom;

            currentZoom = TextureLogic::zoomIn(currentZoom);

            if(oldZoom != currentZoom)
            {
                if(texture != nullptr)
                {
                    setMinimumSize(texture->getImage(currentZoom).size());
                    setMaximumSize(texture->getImage(currentZoom).size());

                    QWidget::repaint();
                }
            }
        }

        void PaintArea::zoomOut()
        {
            TextureLogic::Zoom oldZoom = currentZoom;

            currentZoom = TextureLogic::zoomOut(currentZoom);

            if(oldZoom != currentZoom)
            {
                if(texture != nullptr)
                {
                    setMinimumSize(texture->getImage(currentZoom).size());
                    setMaximumSize(texture->getImage(currentZoom).size());

                    QWidget::repaint();
                }
            }
        }

        void PaintArea::undoPaintOperation()
        {
            // This function is executed once per undo operation shortcut! It does not revert all paint operations at once.

            // If no texture is selected and the undo operation is requested, then a crash will occur. Thus a check
            // to make sure that a texture is selected has to be done first.
            if(texture == nullptr)
            {
                return;
            }

            for(auto zoom : TextureLogic::AllZoomValues)
            {
                auto mostRecentPaintHistory = getReferredToImageHistory(zoom);

                if(mostRecentPaintHistory == nullptr)
                {
                    return;
                }

                // Remember that a paint sequence could compromise of more than one paint action if the mouse was dragged
                // while the left button was done, for example.
                while(!mostRecentPaintHistory->getAppliedAreas().empty())
                {
                    PaintFunctions::PaintedArea *paintedArea = &mostRecentPaintHistory->getAppliedAreas().top();

                    paintTexture(TextureLogic::Zoom::ALWAYS_AT_EMD, paintedArea->appliedArea, paintedArea->previousColour, const_cast<QImage&>(getReferredToImage(zoom)), true);

                    mostRecentPaintHistory->getAppliedAreas().pop();
                }

                // Since the resource held by this pointer is only referred to by this pointer now (as another pointer to it
                // in the texture class was removed after the call to getReferredToimageHistoy) it must be deleted now or else a memory leak will occur

                // Reminder: This is a raw pointer as when using std::unique_ptr, compilation issues kept arising that could not be solved (error: use of deleted function)
                delete mostRecentPaintHistory;
            }

            QWidget::repaint();
        }

        /*
         *  The below two functions are convenience functions. Due to the structure of this class, it has to be checked
         *  which texture (selected, specular, etc) the current class is responsible for. This leads to code duplication
         *  every time this has to be checked, and leads to error as more types of texture are added, as the additional
         *  case has to be handled in several areas. Doing that logic in two functions that are called elsewhere reduces
         *  the amount of changes needed and reduces the change of error.
         */

        const QImage& PaintArea::getReferredToImage(TextureLogic::Zoom zoom) const
        {
            switch(currentTextureImage)
            {
                case CurrentTextureImage::SelectedTexture:
                    return texture->getImage(zoom);

                case CurrentTextureImage::SpecularTexture:
                    return texture->getSpecularTexture(zoom, {});
            }

            // This should never be reached, as the switch statement should cover all of the possible enum values. But
            // in case an enum value is added.
            Q_ASSERT_X(false, __PRETTY_FUNCTION__, "\nInvalid code path taken\n");
        }

        PaintFunctions::PaintHistoryCommand* PaintArea::getReferredToImageHistory(TextureLogic::Zoom zoom) const
        {
            switch(currentTextureImage)
            {
                case CurrentTextureImage::SelectedTexture:
                    return texture->removeRecentPaintHistoryTexture(zoom, {});

                case CurrentTextureImage::SpecularTexture:
                    return texture->removeRecentPaintHistorySpecular(zoom, {});
            }

            // This should never be reached, as the switch statement should cover all of the possible enum values. But
            // in case an enum value is added.
            Q_ASSERT_X(false, __PRETTY_FUNCTION__, "\nInvalid code path taken\n");
        }

        void PaintArea::paintTexture(TextureLogic::Zoom zoom, QPoint mousePosition, const QImage &applyImage, QImage &targetImage, bool undoOperation)
        {
            /*
             * When painting a texture, the area that is to be painted is centred around the input given to this function.
             * To do this, the painted area has to be translated so that it is centred around the point given. This can be
             * done by taking half the paint area and applying it to one side of the point, and painting the other half
             * on the other side of the point. This is done in both the x and y dimension.
             */
            int halfTextureWidth = applyImage.width() / 2;

            int halfTextureHeight = applyImage.height() / 2;

            // When iterating the positions to be painted, local variables used may not match actual points on
            // the texture that is to be painted or the source of the paint. Actual usages in loops will clarify this.
            int adjustedMouseX;
            int adjustedMouseY;

            int adjustedPixelX;
            int adjustedPixelY;

            PaintFunctions::PaintedArea paintedArea;

            // The above variable should only be used if this function is not being called to redo a paint operation
            if(!undoOperation)
            {
                paintedArea.appliedArea = mousePosition;
                paintedArea.previousColour = paintedArea.previousColour.scaled(brush.getPaintImage(zoom).width(), brush.getPaintImage(zoom).height());
            }

            for(int i = -halfTextureWidth; i < halfTextureWidth; ++i)
            {
                // Determine if the painted area is outside of the dimensions of the target texture. For example, if the painted
                // area has a width of 10, and the point is at 0, then the points -5, -4, -3, -2 , -1 will be painted. However,
                // these points do not exist, and therefore this iteration of the loop have to be skipped. Applies to both x and y dimensions.
                adjustedMouseX = mousePosition.x() + i;

                if(adjustedMouseX < 0 || adjustedMouseX > targetImage.width() - 1)
                {
                    continue;
                }

                // Same idea as the adjustedMouseX- if the painted area has a width of 10, then the first point on the
                // applyImage variable is 0, not -5. Applies to both x and y dimension.
                adjustedPixelX = i + halfTextureWidth;

                for(int j = -halfTextureHeight; j < halfTextureHeight; ++j)
                {
                    // Same ideas the x-dimension
                    adjustedMouseY = mousePosition.y() + j;

                    if(adjustedMouseY < 0 || adjustedMouseY > targetImage.height() - 1)
                    {
                        continue;
                    }

                    adjustedPixelY = j + halfTextureHeight;

                    // If this function is called to undo a paint operation, it makes no sense to keep track where this paint operation occurred
                    // as that implies a new paint operation is done
                    if(!undoOperation)
                    {
                        paintedArea.previousColour.setPixelColor(adjustedPixelX, adjustedPixelY, targetImage.pixelColor(adjustedMouseX, adjustedMouseY));
                    }

                    // Finally, time to update (one) pixel in the target texture!
                    targetImage.setPixelColor(adjustedMouseX, adjustedMouseY, applyImage.pixelColor(adjustedPixelX, adjustedPixelY));
                }
            }

            // Only store the paintedArea if a new paint operation was done- otherwise it is irrelevant to storing paint history
            if(!undoOperation)
            {
                paintSequence[TextureLogic::GetZoomIndex(zoom)].push(paintedArea);
            }
        }

        void PaintArea::storePaintHistory()
        {
            // Since this function is called when the mouse is released over this widget, it is possible for this function
            // to be called even when no painting was done. It should not matter if an empty stack history is passed,
            // but ideally, it should not happen and it is more easy to reason about if no empty stack histories are passed
            if(paintSequence[0].empty())
            {
                return;
            }

            for(auto zoom : TextureLogic::AllZoomValues)
            {
                // This empties the paint sequence, and makes it ready to hold a new paint sequence when the user starts painting again
                auto paintHistoryCommand = new PaintFunctions::PaintHistoryCommand{paintSequence[TextureLogic::GetZoomIndex(zoom)]};

                switch(currentTextureImage)
                {
                    case CurrentTextureImage::SelectedTexture:
                        texture->addPaintHistoryTexture(zoom, paintHistoryCommand, {});
                        break;

                    case CurrentTextureImage::SpecularTexture:
                        texture->addPaintHistorySpecular(zoom, paintHistoryCommand, {});
                        break;
                }
            }

            emit paintedSelectedTexture();
        }
    }
}