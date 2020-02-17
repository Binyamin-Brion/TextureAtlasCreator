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
     * For the getSurroundingBorder, the difference is that the ForDrawing version returns an immutable reference while
     * the other version does not. Not all access to the surrounding border needs to modify it, so have an ability to
     * have a const-reference to it is useful. If a copy is needed, the caller can call either version, preferably
     * the ForDrawing version and copy the contents of the reference.
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

    SurroundingBorder& SelectedTexture::getSurroundingBorder()
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
        // Needed when calculating translation for the surrounding border
        QPointF previousDrawingCoords = drawingCoordinates;

        auto currentZoomImage = selectedTexture->getImage(currentZoom);

        // Remember that when moving the texture, it is done so relative to the centre of the texture.
        // Thus while the cursor is moved to the centre of the texture when moving the texture around,
        // the texture's edges are checked to make sure they don't go off screen. If they do, the texture
        // is prevent from being drawn off screen. The cursor is also locked from going off-screen, but it is
        // possible that for a brief second, the texture cursor is allowed to move the texture off-screen.
        // In that case, these checks make sure that at no time the texture is drawn off-screen.
        if((mouseX - currentZoomImage.width() / 2) < 0) // Don't allow texture to be drawn off-screen to the left
        {
            drawingCoordinates.setX(0);
        }
        else if((mouseX + currentZoomImage.width() / 2) > boundaries.width()) // Don't allow texture to be drawn off-screen to the right
        {
            drawingCoordinates.setX(boundaries.width() - currentZoomImage.width());
        }
        else // Valid X drawing position
        {
            drawingCoordinates.setX(mouseX - currentZoomImage.width() / 2);
        }

        if((mouseY - currentZoomImage.height() / 2) < 0) // Don't allow texture to be drawn off-screen to the top
        {
            drawingCoordinates.setY(0);
        }
        else if((mouseY + currentZoomImage.height() / 2) > boundaries.height()) // Don't allow texture to be drawn off-screen to the bottom
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
        // Needed when calculating translation for the surrounding border
        QPointF previousDrawingCoords = drawingCoordinates;

        drawingCoordinates = drawingCoords;

        for(auto &i : surroundingBorder)
        {
            i.translate(drawingCoordinates.x() - previousDrawingCoords.x(), drawingCoordinates.y() - previousDrawingCoords.y());
        }
    }

    void SelectedTexture::setDrawSelectedSurroundingBorder(bool value)
    {
        // Logically, all of the surrounding borders (one at each zoom level) together are visible or not, regardless
        // if a particular zoom is being looked at
        for(auto &i : TextureLogic::AllZoomValues)
        {
            surroundingBorder[::TextureLogic::GetZoomIndex(i)].setSelectedBorderVisible(value);
        }
    }

    /*
     *  The differences between the setTextureXXX functions are that setTexture() assumes initializes all of the variables
     *  associated with drawing a selected texture. On the other hand, setTextureReference() simply updates the texture
     *  reference, without touching any other variables. To be honest, it probably would still be find setTextureReference()
     *  reinitialized all associated variables as setTexture() does, negating its need. Though at the time of writing the
     *  comments, it is hard to check this. Considering how little difference it makes to the code, it is better to leave
     *  the two functions as they are.
     */

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

    void SelectedTexture::setTextureReference(const TextureLogic::Texture &selectedTexture)
    {
        // If a texture is loaded while a selected texture is open, then it is possible that the reference stored in an instance
        // of this class is no longer valid. See TextureBank.cpp for more details. In that case, the reference to the old texture
        // needs to be updated to point to the new location in memory of the selected texture.
        this->selectedTexture = &selectedTexture;
    }

    void SelectedTexture::setZoom(TextureLogic::Zoom zoom, float zoomFactor)
    {
        currentZoom = zoom;

        QPointF oldDrawingCoordinates = drawingCoordinates;

        drawingCoordinates *= zoomFactor;

        QPointF translation = drawingCoordinates - oldDrawingCoordinates;

        // This program moves all of the surrounding borders (one at each zoom level). This does not have to be done, as
        // the surrounding borders for each zoom level could have its drawing position calculated every single time the
        // texture is moved. However, it is decided to keep all of the surrounding borders in the same place relative to
        // each other (which does not matter as only the current zoom border is shown) as its easier to reason about
        for(auto &i : surroundingBorder)
        {
            i.translate(translation.x(), translation.y());
        }
   }
}