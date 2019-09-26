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

    /*
     *  For the get(Image|SurroundingBorder) functions, there are two versions of each:
     *
     * 1. get(Image|SurroundingBorder)
     * 2. get(Image|SurroundingBorder)ForDrawing
     *
     * If the function is getImage, the difference is that the normal getImage function marks the selected texture as
     * not opened; it is a marker that the selected texture does not hold a texture (it still does- the reference is not
     * reseated) but by marking the selected texture as not open, it changes the logic executed in texture atlas.
     *
     * For the getSurroundingBorder, the difference is that the normal getSurroundingBorder returns a copy of the surrounding
     * border, which is used when the selected texture is being placed back into the atlas. A copy is needed as if it is not done,
     * a newly selected texture will change the surrounding border of the previously selected border. The other version of
     * getSurroundingBorder returns a reference which is to prevent needless copying (though it could be removed in favour of the normal
     * version- logically the result will be the same due to how texture atlas handles the call to these functions)
     */

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

        _isOpen = true;

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

}