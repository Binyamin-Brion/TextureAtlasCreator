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

                // Paint event can still be called even when no texture is selected.
                if(specularTexture != nullptr)
                {
                    painter.drawImage(QPoint{0, 0}, specularTexture->getSpecularTexture(currentZoom));
                }
            }

            void DisplayArea::repaintSpecularTexture()
            {
                // Note: calling QWidget::repaint() causes errors relating to threading (Qt internal errors, as this program does
                //       not use threads). Thus some updates to the specular texture may have a small delay, but at least it works.
                //       Not sure why using repaint() causes these issues.
                QWidget::update();
            }

            void DisplayArea::setSpecularTexture(const ::TextureLogic::Texture *specularTexture)
            {
                this->specularTexture = specularTexture;

                if(specularTexture != nullptr)
                {
                    QImage::Format textureFormat = specularTexture->getImage(::TextureLogic::Zoom::Normal).format();

                    // Drawing is not allowed on other formats as such operations are not valid; though in this case viewing
                    // is not allowed as such textures are assumed to not have specular textures
                    if(textureFormat != QImage::Format_RGB32 && textureFormat != QImage::Format_ARGB32)
                    {
                        setEnabled(false);

                        return;
                    }

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
        }
    }
}