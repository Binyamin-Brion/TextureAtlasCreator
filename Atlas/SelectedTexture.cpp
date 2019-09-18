//
// Created by BinyBrion on 2019-09-15.
//

#include "SelectedTexture.h"
#include <QPainter>

#include "TextureLogic/Texture.h"

namespace Atlas
{
    QPoint SelectedTexture::getDrawingCoordinates() const
    {
        return drawingCoordinates;
    }

    const TextureLogic::Texture &SelectedTexture::getImage()
    {
        _isOpen = false;
        
        return *selectedTexture;
    }

    const TextureLogic::Texture& SelectedTexture::getImageForDrawing() const
    {
        return *selectedTexture;
    }

    const QString &SelectedTexture::getTextureLocation() const
    {
        return selectedTexture->textureLocation();
    }

    bool SelectedTexture::isOpen() const
    {
        return _isOpen;
    }

    void SelectedTexture::move(int mouseX, int mouseY, QSize boundaries)
    {
        if(firstMouse)
        {
            previousMouseX = mouseX;

            previousMouseY = mouseY;

            firstMouse = true;
        }

        auto currentZoomImage = selectedTexture->getImage(currentZoom);

        if(mouseX - currentZoomImage.width() / 2 < 0)
        {
            drawingCoordinates.setX(0);
        }
        else if(mouseX + currentZoomImage.width() / 2 > boundaries.width())
        { printf("%d, %d \n", boundaries.width(), currentZoomImage.width());
            drawingCoordinates.setX(boundaries.width() - currentZoomImage.width());
        }
        else
        {
            drawingCoordinates.setX(mouseX - currentZoomImage.width() / 2);
        }

        if(mouseY - currentZoomImage.height() / 2 < 0)
        {
            drawingCoordinates.setY(0);
        }
        else if(mouseY + currentZoomImage.height() / 2 > boundaries.height())
        {
            drawingCoordinates.setY(boundaries.height() - currentZoomImage.height());
        }
        else
        {
            drawingCoordinates.setY(mouseY - currentZoomImage.height() / 2);
        }

    }

    void SelectedTexture::setTexture(const TextureLogic::Texture &selectedTexture)
    {
        this->selectedTexture = &selectedTexture;

        drawingCoordinates.setX(0);
        drawingCoordinates.setY(0);

        previousMouseX = 0;

        previousMouseY = 0;

        _isOpen = true;

        firstMouse = false;
    }

    void SelectedTexture::setZoom(TextureLogic::Zoom zoom)
    {
        currentZoom = zoom;
    }

}