//
// Created by binybrion on 3/12/20.
//

#include <QtGui/QPainter>
#include "DisplayArea.h"
#include "../TextureLogic/Texture.h"

namespace GUI
{
    namespace CurrentTexture
    {
        namespace SpecularTexture
        {

            DisplayArea::DisplayArea(QWidget *parent)
                    :
                    QWidget{parent}
            {
                currentZoom = TextureLogic::Zoom::Normal;
            }

            void DisplayArea::paintEvent(QPaintEvent *event)
            {
                QPainter painter{this};

                if(specularTexture != nullptr)
                {
                    painter.drawImage(QPoint{0, 0}, const_cast<::TextureLogic::Texture*>(specularTexture)->getSpecularTexture(currentZoom, {}));
                }
            }

            void DisplayArea::setSpecularTexture(const ::TextureLogic::Texture *specularTexture)
            {
                this->specularTexture = specularTexture;

                if(specularTexture != nullptr)
                {
                    setMinimumSize(specularTexture->getImage(currentZoom).size());
                    setMaximumSize(specularTexture->getImage(currentZoom).size());
                }
            }

            void DisplayArea::zoomIn()
            {
                TextureLogic::Zoom oldZoom = currentZoom;

                currentZoom = TextureLogic::zoomIn(currentZoom);

                if(oldZoom != currentZoom)
                {
                    if(specularTexture != nullptr)
                    {
                        setMinimumSize(specularTexture->getImage(currentZoom).size());
                        setMaximumSize(specularTexture->getImage(currentZoom).size());

                        QWidget::repaint();
                    }
                }
            }

            void DisplayArea::zoomOut()
            {
                TextureLogic::Zoom oldZoom = currentZoom;

                currentZoom = TextureLogic::zoomOut(currentZoom);

                if(oldZoom != currentZoom)
                {
                    if(specularTexture != nullptr)
                    {
                        setMinimumSize(specularTexture->getImage(currentZoom).size());
                        setMaximumSize(specularTexture->getImage(currentZoom).size());

                        QWidget::repaint();
                    }
                }
            }

            void DisplayArea::repaintSpecularTexture()
            {
                QWidget::update();
            }
        }
    }
}