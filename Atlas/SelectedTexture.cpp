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

    int SelectedTexture::getTextureIndex() const
    {
        return textureIndex;
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

#define SurroundingBorder std::array<TextureBorder::SurroundingBorder, ::TextureLogic::NumberZoomElements()>

    const SurroundingBorder& SelectedTexture::getSurroundingBorderForDrawing() const
    {
        return surroundingBorder;
    }

    SurroundingBorder SelectedTexture::getSurroundingBorder() const
    {
        return surroundingBorder;
    }

#undef SurroundingBorder

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

        QPoint previousDrawingCoords = drawingCoordinates;

        auto currentZoomImage = selectedTexture->getImage(currentZoom);

        if(mouseX - currentZoomImage.width() / 2 < 0)
        {
            drawingCoordinates.setX(0);
        }
        else if(mouseX + currentZoomImage.width() / 2 > boundaries.width())
        {
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

      //  printf("Now drawing at: %d, %d \n", drawingCoordinates.x(), drawingCoordinates.y());

        for(auto &i : surroundingBorder)
        {
            i.translate(drawingCoordinates.x() - previousDrawingCoords.x(), drawingCoordinates.y() - previousDrawingCoords.y());
        }
    }

    void SelectedTexture::setDrawingCoordinates(QPoint drawingCoords)
    {
        drawingCoordinates = drawingCoords;
    }

    void SelectedTexture::setDrawSelectedSurroundingBorder(bool value)
    {
        surroundingBorder[::TextureLogic::GetZoomIndex(currentZoom)].setSelectedBorderVisible(value);
    }

    void SelectedTexture::setTexture(const TextureLogic::Texture &selectedTexture, int index)
    {
        this->selectedTexture = &selectedTexture;

        textureIndex = index;

        drawingCoordinates.setX(0);
        drawingCoordinates.setY(0);

        previousMouseX = 0;

        previousMouseY = 0;

        _isOpen = true;

        firstMouse = false;

        int loopCounter = 0;

        for(auto &i : ::TextureLogic::AllZoomValues)
        {
            surroundingBorder[loopCounter].initialize(selectedTexture.getImage(i).width(), selectedTexture.getImage(i).height());

            loopCounter += 1;
        }
    }

    void SelectedTexture::setTextureReference(const TextureLogic::Texture &selectedTexture)
    {
        this->selectedTexture = &selectedTexture;
    }

    void SelectedTexture::setZoom(TextureLogic::Zoom zoom)
    {
        currentZoom = zoom;
    }

    void SelectedTexture::translateSurroundingBorder(int x, int y)
    {
//        for(auto &i : surroundingBorder)
//        {
//            i.translate(x, y);
//        }
    }

}