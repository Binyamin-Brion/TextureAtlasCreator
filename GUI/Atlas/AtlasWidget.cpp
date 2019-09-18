//
// Created by BinyBrion on 2019-09-17.
//

#include "AtlasWidget.h"
#include "Atlas/TextureAtlas.h"

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
            QCursor c = cursor();

            auto newCursorInformation = textureAtlas->resetCursorPosition();

            if(newCursorInformation.first)
            {
                c.setPos(mapToGlobal(newCursorInformation.second));

                setCursor(c);
            }
        }

        void AtlasWidget::mouseMoveEvent(QMouseEvent *event)
        {
            textureAtlas->mouseMoved(event->x(), event->y());

            QWidget::repaint();
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