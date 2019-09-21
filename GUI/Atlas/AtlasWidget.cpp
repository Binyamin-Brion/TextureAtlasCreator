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
        }

        void AtlasWidget::leaveEvent(QEvent *event)
        {
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

            if(testPosAgainstAtlasBoundaries.first) // A texture is selected
            {
                bool resetCursorPositionX = false;

                bool resetCursorPositionY = false;

                if(event->x() < textureAtlas->getSelectedTextureSize().second.width() / 2) // Cursor trying to move texture offscreen to the left
                {
                    mouseX = textureAtlas->resetCursorPosition().second.x();

                    resetCursorPositionX = true;
                }
                else if(event->x() > testPosAgainstAtlasBoundaries.second.width() - textureAtlas->getSelectedTextureSize().second.width() / 2) // Cursor trying to move texture offscreen to the right
                {
                    mouseX = textureAtlas->resetCursorPosition().second.x();

                    resetCursorPositionX = true;
                }

                if(event->y() < textureAtlas->getSelectedTextureSize().second.height() / 2) // Cursor trying to move texture offscreen to the top
                {
                    mouseY = textureAtlas->resetCursorPosition().second.y();

                    resetCursorPositionY = true;
                }
                else if(event->y() > testPosAgainstAtlasBoundaries.second.height() - textureAtlas->getSelectedTextureSize().second.height() / 2) // Cursor trying to move texture offscreen to the bottom
                {
                    mouseY = textureAtlas->resetCursorPosition().second.y();

                    resetCursorPositionY = true;
                }

                // X and Y dimensions are handled separately as, for example, if the cursor is pushing the texture offscreen to the left,
                // it does not mean that the y position of the cursor is pushing the texture offscreen

                if(resetCursorPositionX)
                {
                    QCursor c = cursor();

                    c.setPos(mapToGlobal(QPoint{textureAtlas->resetCursorPosition().second.x(), mouseY}));

                    setCursor(c);
                }

                if(resetCursorPositionY)
                {
                    QCursor c = cursor();

                    c.setPos(mapToGlobal(QPoint{mouseX, textureAtlas->resetCursorPosition().second.y()}));

                    setCursor(c);
                }
            }

            textureAtlas->mouseMoved(event->x(), event->y());

            QWidget::repaint();
        }

        void AtlasWidget::mousePressEvent(QMouseEvent *event)
        {
            if(event->button() == Qt::LeftButton)
            {
                textureAtlas->mouseClicked(event->x(), event->y());

                QWidget::repaint();
            }
        }

        void AtlasWidget::paintEvent(QPaintEvent *event)
        {
            QWidget::paintEvent(event);

            QPainter painter{this};

            textureAtlas->draw(painter);
        }

        void AtlasWidget::textureButtonPressed(const TextureLogic::Texture &texture)
        {
            try
            {
                textureAtlas->setSelectedTexture(texture);
            }
            catch(::Exceptions::Atlas::TextureAlreadyLoaded &e)
            {
                QMessageBox::warning(this, tr("Error: Texture Already Loaded"), e.what(), QMessageBox::Ok);
            }
        }

        void AtlasWidget::updateTextureReferences(const std::vector<TextureLogic::Texture> &textures)
        {
            textureAtlas->textureLoaded(textures);
        }

        AtlasWidget::~AtlasWidget()
        {

        }

        void AtlasWidget::resizeEvent(QResizeEvent *event)
        {
            QWidget::resizeEvent(event);

            textureAtlas->setAtlasSize(event->size());
        }
    }
}