//
// Created by BinyBrion on 2019-09-17.
//

#include "AtlasWidget.h"
#include "Atlas/TextureAtlas.h"

#include <QCoreApplication>
#include <QPainter>
#include <QMouseEvent>
#include <QResizeEvent>

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
            textureAtlas->resetFirstMouse();

//            QCursor c = cursor();
//
//            auto newCursorInformation = textureAtlas->resetCursorPosition();
//
//            if(newCursorInformation.first)
//            {
//                c.setPos(mapToGlobal(newCursorInformation.second));
//
//                setCursor(c);
//            }
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

            if(testPosAgainstAtlasBoundaries.first) // A texture is selected
            {
                bool resetCursorPositionX = false;

                bool resetCursorPositionY = false;

                if(event->x() < textureAtlas->getSelectedTextureSize().second.width() / 2)
                {
                    mouseX = textureAtlas->resetCursorPosition().second.x();

                    resetCursorPositionX = true;
                }
                else if(event->x() > testPosAgainstAtlasBoundaries.second.width() - textureAtlas->getSelectedTextureSize().second.width() / 2)
                {
                    mouseX = textureAtlas->resetCursorPosition().second.x();

                    resetCursorPositionX = true;
                }

                if(event->y() < textureAtlas->getSelectedTextureSize().second.height() / 2)
                {
                    mouseY = textureAtlas->resetCursorPosition().second.y();

                    resetCursorPositionY = true;
                }
                else if(event->y() > testPosAgainstAtlasBoundaries.second.height() - textureAtlas->getSelectedTextureSize().second.height() / 2)
                {
                    mouseY = textureAtlas->resetCursorPosition().second.y();

                    resetCursorPositionY = true;
                }

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
            if(event->button())
            {
                textureAtlas->mouseClicked();
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
            textureAtlas->setSelectedTexture(texture);
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