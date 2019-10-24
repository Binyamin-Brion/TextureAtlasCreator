//
// Created by BinyBrion on 2019-09-26.
//

#include <QtGui/QPainter>
#include "RenderArea.h"
#include "TextureLogic/Texture.h"
#include "PaintFunctions/PaintHistoryCommand.h"

#include <QMouseEvent>
#include <QtWidgets/QShortcut>

namespace GUI
{
    namespace CurrentTexture
    {
        RenderArea::RenderArea(CurrentTextureImage currentTextureImage, QWidget *parent) : QWidget{parent}, currentTextureImage{currentTextureImage}, previousMousePosition{-1, -1}
        {
            currentZoom = TextureLogic::Zoom::Normal;

            setFocusPolicy(Qt::StrongFocus);
            setMouseTracking(true);

            brush.setPaintTypeSolid(currentZoom, QSize{25, 25}, QColor{255, 255, 255});

            new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Z), this, SLOT(undoPaintOperation()));
        }

        const PaintFunctions::Brush& RenderArea::getBrush() const
        {
            return brush;
        }

        QImage::Format RenderArea::getCurrentTextureFormat() const
        {
            if(texture == nullptr)
            {
                return QImage::Format_Invalid;
            }

            return textureFormat;
        }

        TextureLogic::Zoom RenderArea::getZoom() const
        {
            return currentZoom;
        }

        void RenderArea::mouseMoveEvent(QMouseEvent *event)
        {
            if(texture != nullptr)
            {
                if(leftMouseButtonDown)
                {
                    for(auto zoom : TextureLogic::AllZoomValues)
                    {
                        float zoomFactor = TextureLogic::GetZoomValue(zoom) / TextureLogic::GetZoomValue(currentZoom);

                        paintTexture(zoom, event->pos() * zoomFactor, brush.getPaintImage(zoom), const_cast<QImage &>(getReferredToImage(zoom)), false);
                    }
                }

                previousMousePosition = event->pos();

                QWidget::repaint();
            }
        }

        void RenderArea::mousePressEvent(QMouseEvent *event)
        {
            if(event->button() == Qt::LeftButton)
            {
                if(texture != nullptr)
                {
                    for(auto zoom : TextureLogic::AllZoomValues)
                    {
                        float zoomFactor = TextureLogic::GetZoomValue(zoom) / TextureLogic::GetZoomValue(currentZoom);

                        paintTexture(zoom, event->pos() * zoomFactor, brush.getPaintImage(zoom), const_cast<QImage &>(getReferredToImage(zoom)), false);
                    }

                    QWidget::repaint();

                    leftMouseButtonDown = true;
                }
            }
        }

        void RenderArea::mouseReleaseEvent(QMouseEvent *event)
        {
            if(event->button() == Qt::LeftButton)
            {
                leftMouseButtonDown = false;

                storePaintHistory();
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

                if(previousMousePosition.x() != -1 && previousMousePosition.y() != -1)
                {
                    painter.drawImage(previousMousePosition.x() - brush.getPaintImage(currentZoom).width() / 2,
                            previousMousePosition.y() - brush.getPaintImage(currentZoom).height() / 2, brush.getPaintImage(currentZoom));
                }
            }
        }

        void RenderArea::setTexture(TextureLogic::Texture *texture)
        {
            this->texture = texture;

            if(texture != nullptr)
            {
                setMinimumSize(texture->getImage(TextureLogic::Zoom::Normal).size());
                setMaximumSize(texture->getImage(TextureLogic::Zoom::Normal).size());

                textureFormat = texture->getImage(TextureLogic::Zoom::Normal).format();
            }

            QWidget::repaint();
        }

        void RenderArea::zoomIn()
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

        void RenderArea::zoomOut()
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

        void RenderArea::undoPaintOperation()
        {
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

                // Remember that a paint history could compromise of more than one paint action if the mouse was dragged
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

        const QImage& RenderArea::getReferredToImage(TextureLogic::Zoom zoom) const
        {
            switch(currentTextureImage)
            {
                case CurrentTextureImage::SelectedTexture:
                    return texture->getImage(zoom);

                case CurrentTextureImage::SpecularTexture:
                    return texture->getSpecularTexture(zoom, {});
            }
        }

        PaintFunctions::PaintHistoryCommand* RenderArea::getReferredToImageHistory(TextureLogic::Zoom zoom) const
        {
            switch(currentTextureImage)
            {
                case CurrentTextureImage::SelectedTexture:
                    return texture->removeRecentPaintHistoryTexture(zoom, {});

                case CurrentTextureImage::SpecularTexture:
                    return texture->removeRecentPaintHistorySpecular(zoom, {});
            }
        }

        void RenderArea::paintTexture(TextureLogic::Zoom zoom, QPoint mousePosition, const QImage &applyImage, QImage &targetImage, bool undoOperation)
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
                appliedBrushAreas[TextureLogic::GetZoomIndex(zoom)].push(paintedArea);
            }
        }

        void RenderArea::storePaintHistory()
        {
            // Since this function is called when the mouse is released over this widget, it is possible for this function
            // to be called even when no painting was done. It should not matter if an empty stack history is passed,
            // but ideally, it should not happen and it is more easy to reason about if no empty stack histories are passed

            if(appliedBrushAreas[0].empty())
            {
                return;
            }

            for(auto zoom : TextureLogic::AllZoomValues)
            {
                auto paintHistoryCommand = new PaintFunctions::PaintHistoryCommand{ appliedBrushAreas[TextureLogic::GetZoomIndex(zoom)]};

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