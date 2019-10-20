//
// Created by BinyBrion on 2019-09-15.
//

#include "SelectedTexture.h"
#include <QPainter>

#include "TextureLogic/Texture.h"

namespace Atlas
{
    QPointF SelectedTexture::getDrawingCoordinates() const
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

    // This macro is to make the following two functions easier to read- nothing more. That is why it is undefined after the two functions.

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
        QPointF previousDrawingCoords = drawingCoordinates;

        auto currentZoomImage = selectedTexture->getImage(currentZoom);

        // Remember that when moving the texture, it is done so relative to the centre of the texture.
        // Thus while the cursor is moved to the centre of the texture when moving the texture around,
        // the texture's edges are checked to make sure they don't go off screen. If they do, the texture
        // is prevent from being drawn off screen. The cursor is also locked from going offscreen, but it is
        // possible that for a brief second, the texture cursor is allowed to move the texture offscreen.
        // In that case, these checks make sure that at no time the texture is drawn offscreen.

        if(mouseX - currentZoomImage.width() / 2 < 0) // Don't allow texture to be drawn offscreen to the left
        {
            drawingCoordinates.setX(0);
        }
        else if(mouseX + currentZoomImage.width() / 2 > boundaries.width()) // Don't allow texture to be drawn offscreen to the right
        {
            drawingCoordinates.setX(boundaries.width() - currentZoomImage.width());
        }
        else // Valid X drawing position
        {
            drawingCoordinates.setX(mouseX - currentZoomImage.width() / 2);
        }

        if(mouseY - currentZoomImage.height() / 2 < 0) // Don't allow texture to be drawn offscreen to the top
        {
            drawingCoordinates.setY(0);
        }
        else if(mouseY + currentZoomImage.height() / 2 > boundaries.height()) // Don't allow texture to be drawn offscreen to the bottom
        {
            drawingCoordinates.setY(boundaries.height() - currentZoomImage.height());
        }
        else // Valid Y drawing position
        {
            drawingCoordinates.setY(mouseY - currentZoomImage.height() / 2);
        }

        for(auto &i : surroundingBorder)
        {
            i.translate(drawingCoordinates.x() - previousDrawingCoords.x(), drawingCoordinates.y() - previousDrawingCoords.y());
        }
    }

    void SelectedTexture::setDrawingCoordinates(QPointF drawingCoords)
    {
        QPointF previousDrawingCoords = drawingCoordinates;

        drawingCoordinates = drawingCoords;

        for(auto &i : surroundingBorder)
        {
            i.translate(drawingCoordinates.x() - previousDrawingCoords.x(), drawingCoordinates.y() - previousDrawingCoords.y());
        }
    }

    void SelectedTexture::setDrawSelectedSurroundingBorder(bool value)
    {
        for(auto &i : TextureLogic::AllZoomValues)
        {
            surroundingBorder[::TextureLogic::GetZoomIndex(i)].setSelectedBorderVisible(value);
        }
    }

    // This function (re)initializes an instance of this class to represent the texture passed in

    void SelectedTexture::setTexture(const TextureLogic::Texture &selectedTexture, int index)
    {
        this->selectedTexture = &selectedTexture;

        // This is used to update the reference to the texture if the old reference becomes invalidated. See TextureBank.cpp for more details.

        textureIndex = index;

        drawingCoordinates.setX(0);
        drawingCoordinates.setY(0);

        _isOpen = true;

        int loopCounter = 0;

        for(auto &i : ::TextureLogic::AllZoomValues)
        {
            surroundingBorder[loopCounter].initialize(selectedTexture.getImage(i).width(), selectedTexture.getImage(i).height(),
                    selectedTexture.getIntersectionBorderWidth(i), selectedTexture.getSelectedBorderWidth(i));

            loopCounter += 1;
        }
    }

    // If a texture is loaded while a selected texture is open, then it is possible that the reference stored in an instance
    // of this class is no longer valid. See TextureBank.cpp for more details. In that case, the reference to the old texture
    // needs to be updated to point to the new location in memory of the selected texture.

    void SelectedTexture::setTextureReference(const TextureLogic::Texture &selectedTexture)
    {
        this->selectedTexture = &selectedTexture;
    }

    void SelectedTexture::setZoom(TextureLogic::Zoom zoom, float zoomFactor)
    {
        currentZoom = zoom;

        QPointF oldDrawingCoordinates = drawingCoordinates;

        drawingCoordinates *= zoomFactor;

        QPointF translation = drawingCoordinates - oldDrawingCoordinates;

        for(auto &i : surroundingBorder)
        {
            i.translate(translation.x(), translation.y());
        }
   }
}