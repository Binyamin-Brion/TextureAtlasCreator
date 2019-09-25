//
// Created by BinyBrion on 2019-09-17.
//

#include "AtlasWidget.h"
#include "Atlas/TextureAtlas.h"
#include "Exceptions/Atlas/TextureAlreadyLoaded.h"

#include <QCoreApplication>
#include <QPainter>
#include <QMouseEvent>
#include <QResizeEvent>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QScrollArea>

namespace GUI
{
    namespace Atlas
    {
        AtlasWidget::AtlasWidget(QSize atlasSize, QWidget *parent)
                                : QWidget{parent}, textureAtlas{std::make_unique<::Atlas::TextureAtlas>()}, size{atlasSize}
        {
            setMinimumSize(size);
            setMaximumSize(size);

            setFocusPolicy(Qt::StrongFocus);
            setMouseTracking(true);

            textureAtlas->setAtlasWidgetReference(this);

            viewPortOffset = QPoint{0, 0};
        }

        void AtlasWidget::enterEvent(QEvent *event)
        {

        }

        void AtlasWidget::leaveEvent(QEvent *event)
        {
            QWidget::leaveEvent(event);

            // Note that just because this function is called that the texture atlas may not reset its first mouse
            // status; it depends on whether a texture is selected

            textureAtlas->resetFirstMouse();

        }

        void AtlasWidget::keyPressEvent(QKeyEvent *event)
        {
            if(event->key() == Qt::Key_Escape)
            {
                QCoreApplication::quit();
            }
        }

        void AtlasWidget::mouseMoveEvent(QMouseEvent *event)
        {
            auto testPosAgainstAtlasBoundaries = textureAtlas->getAtlasSize();

            int mouseX = event->x();

            int mouseY = event->y();

            /*
             *  Note that the cursor must stay in a certain area for the entire texture to be rendered onscreen:
             *    ___________________________
             *   |   _____________________   |
             *   |  |                     |  |
             *   |  |                     |  |
             *   |  |        (1)          |  |
             *   |  |                     |  |
             *   |  |                     |  |
             *   |  |_____________________|  |
             *   |_____________(2)___________|
             *
             *    The width of (2) is half the texture width to the left and right, and half the texture height to the top and bottom.
             *
             *    In order for the cursor to push the texture offscreen, it must remain in area (1).
             */
            printf("%d \n", mouseX);

            if(mouseX < viewPortOffset.x())
            {
                QCursor c = cursor();

                previousMouseCoords.setX(viewPortOffset.x());

                c.setPos(mapToGlobal(previousMouseCoords));

                setCursor(c);

                return;
            }

            if(mouseX >= viewPortOffset.x() + viewPort.width())
            {
                QCursor c = cursor();

                previousMouseCoords.setX(viewPort.width() + viewPortOffset.x());

                c.setPos(mapToGlobal(previousMouseCoords));

                setCursor(c);

                return;
            }

            if(mouseY < viewPortOffset.y())
            {
                QCursor c = cursor();

                previousMouseCoords.setY(viewPortOffset.y());

                c.setPos(mapToGlobal(previousMouseCoords));

                setCursor(c);

                return;
            }

            if(mouseY >= viewPortOffset.y() + viewPort.height())
            {
                QCursor c = cursor();

                previousMouseCoords.setY(viewPort.height() + viewPortOffset.y());

                c.setPos(mapToGlobal(previousMouseCoords));

                setCursor(c);

                return;
            }

            if(testPosAgainstAtlasBoundaries.first) // A texture is selected
            {
                cursorCanLeaveWidget = false;

                setCursor(Qt::BlankCursor);

                if(textureAtlas->moveMouseTo().first)
                {
                    QCursor c = cursor();

                    c.setPos(mapToGlobal(QPoint{textureAtlas->moveMouseTo().second.x(), textureAtlas->moveMouseTo().second.y()}));

                    setCursor(c);

                    mouseX = textureAtlas->moveMouseTo().second.x();

                    mouseY = textureAtlas->moveMouseTo().second.y();
                }

                bool resetCursorPositionX = false;

                bool resetCursorPositionY = false;

                int lowerBoundX = textureAtlas->getSelectedTextureSize().second.width() / 2 > (viewPort.width() - 5) ? (viewPort.width() - 5) : textureAtlas->getSelectedTextureSize().second.width() / 2;

                int upperBoundX;

                if(textureAtlas->getSelectedTextureSize().second.width() / 2 > (viewPort.width() - 5))
                {
                    upperBoundX = (width() + (width() - viewPort.width())) / 2;
                }
                else
                {
                    upperBoundX = testPosAgainstAtlasBoundaries.second.width() - textureAtlas->getSelectedTextureSize().second.width() / 2;
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

                int lowerBoundY = textureAtlas->getSelectedTextureSize().second.height() / 2 > (viewPort.height() - 5) ? (viewPort.height() - 5) : textureAtlas->getSelectedTextureSize().second.height() / 2;

                int upperBoundY;

                if(textureAtlas->getSelectedTextureSize().second.height() / 2 > (viewPort.height() - 5))
                {
                    upperBoundY = (height() + (height() - viewPort.height())) / 2;
                }
                else
                {
                    upperBoundY = testPosAgainstAtlasBoundaries.second.height() - textureAtlas->getSelectedTextureSize().second.height() / 2;
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

                if(resetCursorPositionX)
                {
                    QCursor c = cursor();

                    c.setPos(mapToGlobal(QPoint{mouseX, mouseY}));

                    setCursor(c);
                }

                if(resetCursorPositionY)
                {
                    QCursor c = cursor();

                    c.setPos(mapToGlobal(QPoint{mouseX, mouseY}));

                    setCursor(c);
                }

                previousMouseCoords = QPoint{mouseX, mouseY};
            }
            else
            {
                cursorCanLeaveWidget = true;
            }

            textureAtlas->mouseMoved(event->x(), event->y());

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

            QWidget::repaint();
        }

        void AtlasWidget::paintEvent(QPaintEvent *event)
        {
            QWidget::paintEvent(event);

            QPainter painter{this};

            textureAtlas->draw(painter);
        }

        void AtlasWidget::textureButtonPressed(const TextureLogic::Texture &texture)
        {printf("Height at buttonp ress: %d \n", height());
            try
            {
                textureAtlas->setSelectedTexture(texture);
            }
            catch(::Exceptions::Atlas::TextureAlreadyLoaded &e)
            {
                QMessageBox::warning(this, tr("Error: Texture Already Loaded"), e.what(), QMessageBox::Ok);
            }
        }

        void AtlasWidget::setViewPort(QSize viewPort)
        {
            printf("Viewport: %d, %d \n", viewPort.width(), viewPort.height());

            this->viewPort = QSize{viewPort.width(), viewPort.height()};
        }

        void AtlasWidget::updateTextureReferences(const std::vector<TextureLogic::Texture> &textures)
        {
            textureAtlas->textureLoaded(textures);
        }

        void AtlasWidget::translateViewPort(int dx, int dy)
        {
            viewPortOffset.setX(viewPortOffset.x() - dx);

            viewPortOffset.setY(viewPortOffset.y() - dy);
        }

        AtlasWidget::~AtlasWidget()
        {

        }

        void AtlasWidget::resizeEvent(QResizeEvent *event)
        {
            QWidget::resizeEvent(event);

            textureAtlas->setAtlasSize(event->size());
        }

        void AtlasWidget::moveMouseTo(int x, int y)
        {
            int mousePosY = y;

            if(mousePosY > viewPortOffset.y() + viewPort.height())
            {
                mousePosY = viewPortOffset.y() + viewPort.height() - 5;
            }

            QCursor c = cursor();

            c.setPos(mapToGlobal(QPoint{x, mousePosY}));

            setCursor(c);
        }
    }
}