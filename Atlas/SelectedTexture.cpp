//
// Created by BinyBrion on 2019-09-15.
//

#include "SelectedTexture.h"
#include <QPainter>

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

    bool SelectedTexture::isOpen() const
    {
        return _isOpen;
    }

    void SelectedTexture::move(int mouseX, int mouseY)
    {
        if(firstMouse)
        {
            previousMouseX = mouseX;

            previousMouseY = mouseY;

            firstMouse = true;
        }

        int differenceX = mouseX - previousMouseX;

        int differenceY = mouseY - previousMouseY;

        drawingCoordinates.setX(drawingCoordinates.x() + differenceX);

        drawingCoordinates.setY(drawingCoordinates.y() + differenceY);
    }

    void SelectedTexture::setTexture(const TextureLogic::Texture &selectedTexture)
    {
        this->selectedTexture = &selectedTexture;

        drawingCoordinates.setX(0);
        drawingCoordinates.setY(0);

        previousMouseX = 0;

        previousMouseY = 0;
    }

}