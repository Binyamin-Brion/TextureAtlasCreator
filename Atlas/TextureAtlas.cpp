//
// Created by BinyBrion on 2019-09-14.
//

#include "TextureAtlas.h"

#include "TextureLogic/TextureBank.h"
#include "Atlas/SelectedTexture.h"
#include "Exceptions/Atlas/TextureAlreadyLoaded.h"

#include <QPainter>
#include "GUI/Atlas/AtlasWidget.h"

namespace Atlas
{
    TextureAtlas::TextureAtlas()
    {
        selectedTexture = new SelectedTexture;

        selectedExistingTexture = new SelectedTexture;

        currentZoom = TextureLogic::Zoom::Normal;

        currentZoomIndex = ::TextureLogic::GetZoomIndex(currentZoom);
    }

    bool TextureAtlas::checkIntersection()
    {
        // A texture can only intersect with other textures if it's moving, in which case a texture must have
        // been selected (does not matter which one, as long as there is one selected)

        if(!selectedTexture->isOpen() && !selectedExistingTexture->isOpen())
        {
            return false;
        }

        // Note: The selected texture must be passed as a parameter, as the texture passed in to the check intersection
        // border will not have its border drawn, even if there's an intersection (visually that would look off)
        // TLDR: Do not cast away constness on selectedTexture->getImageForDrawing and write selectedTexture.checkIntersection(other Texture)

        intersectionOccured = false;

        for(auto &i : textureDrawingPositions)
        {
            // Checking for an intersection may change the state of a texture (it may give a mark to itself internally to
            // draw its border). Therefore the references used when checking for intersections cannot be const

            if(selectedTexture->isOpen())
            {
                intersectionOccured |= i.surroundingBorder[currentZoomIndex].checkIntersection(selectedTexture->getSurroundingBorderForDrawing()[currentZoomIndex]);
            }
            else
            {
                intersectionOccured |= i.surroundingBorder[currentZoomIndex].checkIntersection(selectedExistingTexture->getSurroundingBorderForDrawing()[currentZoomIndex]);
            }
        }
    }

    void TextureAtlas::draw(QPainter &painter)
    {
        // Note the order of drawing: the selected texture should be drawn last so that it is always drawn,
        // even when over another texture

        for(const auto &i : textureDrawingPositions)
        {
            painter.drawImage(i.drawingPosition, i.texture->getImage(currentZoom));

            i.surroundingBorder[currentZoomIndex].draw(painter);

           // i.texture->drawBorder(painter, currentZoom);
        }

        if(selectedTexture->isOpen())
        {
            painter.drawImage(selectedTexture->getDrawingCoordinates(), selectedTexture->getImageForDrawing().getImage(currentZoom));
        }
        if(selectedExistingTexture->isOpen())
        {
            painter.drawImage(selectedExistingTexture->getDrawingCoordinates(), selectedExistingTexture->getImageForDrawing().getImage(currentZoom));

            selectedExistingTexture->getSurroundingBorderForDrawing()[currentZoomIndex].draw(painter);
        }
    }

    std::pair<bool, QSize> TextureAtlas::getAtlasSize() const
    {
        // The result of this function is intended to be used if a texture is selected, as the result is used to determine
        // if a texture can further moved in a direction and when to reset cursor (hence true or false as first parameter of pair)

        if(selectedTexture->isOpen())
        {
            return {true, atlasSize};
        }
        else if(selectedExistingTexture->isOpen())
        {
            return {leftMouseButtonDown, atlasSize};
        }

        return {false, QSize{-1, -1}};
    }

    std::pair<bool, QPoint> TextureAtlas::getDrawingCoordinates() const
    {
      // Same idea for using pair and checking if a texture is selected as fn getAtlasSize

        if(selectedTexture->isOpen())
        {
            return {true, selectedTexture->getDrawingCoordinates()};
        }
        else if(selectedExistingTexture->isOpen())
        {
            return {true,selectedTexture->getDrawingCoordinates()};
        }

        return{false, QPoint{-1, -1}};
    }

    std::pair<bool, QSize> TextureAtlas::getSelectedTextureSize() const
    {
        // Same idea for using pair and checking if a texture is selected as fn getAtlasSize

        if(selectedTexture->isOpen())
        {
            return {true,
                    QSize{selectedTexture->getImageForDrawing().getImage(currentZoom).width(), selectedTexture->getImageForDrawing().getImage(currentZoom).height()}};
        }
        else if(selectedExistingTexture->isOpen())
        {
            return {true,
                    QSize{selectedExistingTexture->getImageForDrawing().getImage(currentZoom).width(), selectedExistingTexture->getImageForDrawing().getImage(currentZoom).height()}};
        }

        return{false, QSize{-1, -1}};
    }

    void TextureAtlas::mouseClicked(int mouseX, int mouseY, int mouseButton)
    {
        leftMouseButtonDown = mouseButton == Qt::LeftButton;

        if(selectedExistingTexture->isOpen())
        {
            //selectedExistingTexture->setDrawSelectedSurroundingBorder(false);

            if(mouseX >= selectedExistingTexture->getDrawingCoordinates().x() && mouseX <= selectedExistingTexture->getDrawingCoordinates().x() + selectedExistingTexture->getImageForDrawing().getImage(currentZoom).width())
            {
                if(mouseY >= selectedExistingTexture->getDrawingCoordinates().y() && mouseY <= selectedExistingTexture->getDrawingCoordinates().y() + selectedExistingTexture->getImageForDrawing().getImage(currentZoom).height())
                {
                    previousDrawingCoords = selectedExistingTexture->getDrawingCoordinates();

                    ignoreMouseRelease = false;
                }
                else
                {
                    selectedExistingTexture->setDrawSelectedSurroundingBorder(false);

                    addTexture(selectedExistingTexture);
                }
            }
            else
            {
                selectedExistingTexture->setDrawSelectedSurroundingBorder(false);

                addTexture(selectedExistingTexture);
            }
        }

       // if(!selectedExistingTexture->isOpen())
        {
            bool deleteTextureDrawingPosition = false;

            unsigned int deleteIndex = 0;

            for(auto &i : textureDrawingPositions)
            {
                int currentTextureWidth = i.texture->getImage(currentZoom).width();

                int currentTextureHeight = i.texture->getImage(currentZoom).height();

                if(mouseX >= i.drawingPosition.x() && mouseX <= i.drawingPosition.x() + currentTextureWidth)
                {
                    if(mouseY >= i.drawingPosition.y() && mouseY <= i.drawingPosition.y() + currentTextureHeight)
                    { printf("Movuing \n");
                        if(selectedExistingTexture->isOpen())
                        {
                            addTexture(selectedExistingTexture);
                        }

                        i.surroundingBorder[currentZoomIndex].setSelectedBorderVisible(true);

                        selectedExistingTexture->setTexture(*i.texture, i.index);

                        selectedExistingTexture->translateSurroundingBorder(i.drawingPosition.x(), i.drawingPosition.y());

                        selectedExistingTexture->move(i.drawingPosition.x() + i.texture->getImage(currentZoom).width() / 2,
                                              i.drawingPosition.y() + i.texture->getImage(currentZoom).height() / 2,
                                              atlasSize);

                        selectedExistingTexture->setDrawSelectedSurroundingBorder(true);

                        previousDrawingCoords = i.drawingPosition;

                        printf("Previous: %d, %d \n", previousDrawingCoords.x(), previousDrawingCoords.y());

                        ignoreMouseRelease = true;

                        deleteTextureDrawingPosition = true;
                    }
                }

                if(!deleteTextureDrawingPosition)
                {
                    deleteIndex += 1;

                    i.surroundingBorder[currentZoomIndex].setSelectedBorderVisible(false);
                }
            }

            if(deleteTextureDrawingPosition)
            {
                textureDrawingPositions.erase(textureDrawingPositions.begin() + deleteIndex);
            }
        }
    }

    void TextureAtlas::mouseMoved(int mouseX, int mouseY)
    {
        if(selectedTexture->isOpen())
        {
            // When the selected texture is being moved, not only do its drawing coordinates have to be updated, but
            // also its border. This border is contained within the Texture class and a mutable reference is needed to update it,
            // and so a reference to it must not be const in order for that surrounding border to updated

            auto& selectedTextureNotConst = const_cast<TextureLogic::Texture&>(selectedTexture->getImageForDrawing());

            // Translate the new cursor position so that it is relative to the centre of the texture; the result
            // is that the newMouse positions describe the topleft of the texture, or in other words the drawing coordinates

            int newMouseX = mouseX - selectedTexture->getImageForDrawing().getImage(currentZoom).width() / 2;

            int newMouseY = mouseY - selectedTexture->getImageForDrawing().getImage(currentZoom).height() / 2;

            // The first time the texture is moved is a special case, as there is no initial cursor position

            if(firstMouse)
            {
                selectedTexture->translateSurroundingBorder(newMouseX, newMouseY);

                //selectedTextureNotConst.translate(newMouseX, newMouseY);

                firstMouse = false;
            }
            else
            {
                int differenceX = newMouseX - previousMouseX;

                int differenceY = newMouseY - previousMouseY;

                selectedTexture->translateSurroundingBorder(differenceX, differenceY);

                //selectedTextureNotConst.translate(differenceX, differenceY);
            }

            previousMouseX = newMouseX;

            previousMouseY = newMouseY;

            selectedTexture->move(mouseX, mouseY, atlasSize);

            checkIntersection();
        }
        else if(selectedExistingTexture->isOpen())
        {
            if(!leftMouseButtonDown)
            {
                return;
            }

            if(!ignoreMouseRelease)
            {
                QPoint drawingCoords = selectedExistingTexture->getDrawingCoordinates();
                auto& currentImage = selectedExistingTexture->getImageForDrawing().getImage(currentZoom);

                moveMouse = true;

                newMousePosition = {drawingCoords.x() + currentImage.width() / 2, drawingCoords.y() + currentImage.height() / 2};

                atlasWidget->moveMouseTo(newMousePosition.x(), newMousePosition.y());
            }

            moveMouse = false;

            ignoreMouseRelease = true;

            //mouseMovedTextureSelected = true;

            auto& selectedTextureNotConst = const_cast<TextureLogic::Texture&>(selectedExistingTexture->getImageForDrawing());

            // Translate the new cursor position so that it is relative to the centre of the texture; the result
            // is that the newMouse positions describe the topleft of the texture, or in other words the drawing coordinates

            int newMouseX = mouseX - selectedExistingTexture->getImageForDrawing().getImage(currentZoom).width() / 2;

            int newMouseY = mouseY - selectedExistingTexture->getImageForDrawing().getImage(currentZoom).height() / 2;

            if(firstMouse)
            { printf("First mouse \n");
                selectedExistingTexture->translateSurroundingBorder(newMouseX, newMouseY);

                //selectedTextureNotConst.translate(newMouseX, newMouseY);

                firstMouse = false;
            }
            else
            {
                int differenceX = newMouseX - previousMouseX;

                int differenceY = newMouseY - previousMouseY;

                selectedExistingTexture->translateSurroundingBorder(differenceX, differenceY);

                //selectedTextureNotConst.translate(differenceX, differenceY);
            }

            previousMouseX = newMouseX;

            previousMouseY = newMouseY;

            selectedExistingTexture->move(mouseX, mouseY, atlasSize);

            checkIntersection();
        }
    }

    void TextureAtlas::mouseReleased(int mouseX, int mouseY, int mouseButton)
    {
        if(mouseButton == Qt::LeftButton)
        {
            leftMouseButtonDown = false;
        }

        if(selectedExistingTexture->isOpen())
        {
            if(intersectionOccured)
            {
                printf("Previous: %d, %d \n", previousDrawingCoords.x(), previousDrawingCoords.y());

                previousDrawingCoords.setX(previousDrawingCoords.x() + selectedExistingTexture->getImageForDrawing().getImage(currentZoom).width() / 2);
                previousDrawingCoords.setY(previousDrawingCoords.y() + selectedExistingTexture->getImageForDrawing().getImage(currentZoom).height() / 2);

                selectedExistingTexture->move(previousDrawingCoords.x(), previousDrawingCoords.y(), atlasSize);

//                selectedExistingTexture->translateSurroundingBorder(translation.x(), translation.y());
//                selectedExistingTexture->setDrawingCoordinates(previousDrawingCoords);

                firstMouse = true;

                checkIntersection();
            }
            else if(!ignoreMouseRelease)
            {
                if(mouseX >= selectedExistingTexture->getDrawingCoordinates().x() && mouseX <= selectedExistingTexture->getDrawingCoordinates().x() + selectedExistingTexture->getImageForDrawing().getImage(currentZoom).width())
                {
                    if(mouseY >= selectedExistingTexture->getDrawingCoordinates().y() && mouseY <= selectedExistingTexture->getDrawingCoordinates().y() + selectedExistingTexture->getImageForDrawing().getImage(currentZoom).height())
                    {
                        selectedExistingTexture->setDrawSelectedSurroundingBorder(false);

                        addTexture(selectedExistingTexture);
                    }
                }
            }
        }

        if(selectedTexture->isOpen())
        {
            addTexture(selectedTexture);
        }
    }

    std::pair<bool, QPoint> TextureAtlas::moveMouseTo() const
    {
        return {moveMouse, newMousePosition};
    }

    // If the texture is opened, then there is chance it is being attempted to be moved to an invalid location.
    // Before that is done, the cursor has to be moved back to a valid location, which this function provides.
    // For example: cursor moving texture outside of defined atlas size. AtlasWidget has to check if such a condition occurs

    std::pair<bool, QPoint> TextureAtlas::resetCursorPosition() const
    {
        if(!selectedTexture->isOpen() && !selectedExistingTexture->isOpen())
        {
            return {false, QPoint{-1, -1}};
        }

        int newMouseX;

        int newMouseY;

        if(selectedTexture->isOpen())
        {
            newMouseX = selectedTexture->getDrawingCoordinates().x() + selectedTexture->getImageForDrawing().getImage(currentZoom).width() / 2;

            newMouseY = selectedTexture->getDrawingCoordinates().y() + selectedTexture->getImageForDrawing().getImage(currentZoom).height() / 2;
        }
        else if(selectedExistingTexture->isOpen())
        {
            newMouseX = selectedExistingTexture->getDrawingCoordinates().x() + selectedExistingTexture->getImageForDrawing().getImage(currentZoom).width() / 2;

            newMouseY = selectedExistingTexture->getDrawingCoordinates().y() + selectedExistingTexture->getImageForDrawing().getImage(currentZoom).height() / 2;
        }
        else
        {
            Q_ASSERT_X(false, __PRETTY_FUNCTION__, "No texture selected, but somehow this if branch got executed!");
        }

        return {true, QPoint{newMouseX, newMouseY}};
    }

    void TextureAtlas::resetFirstMouse()
    {
        // Only reset the status indicating it is the first time the cursor is entering the atlas widget if a texture is not
        // opened; otherwise there is a valid initial cursor position to use for a translation

        if(selectedExistingTexture->isOpen())
        {
            firstMouse = false;

            return;
        }

        firstMouse = !selectedTexture->isOpen();
    }

    void TextureAtlas::setAtlasSize(QSize size)
    {
        atlasSize = size;
    }

    void TextureAtlas::setAtlasWidgetReference(GUI::Atlas::AtlasWidget *atlasWidget)
    {
        this->atlasWidget = atlasWidget;
    }

    void TextureAtlas::setSelectedTexture(const TextureLogic::Texture &texture)
    {
        // In a texture atlas, there is only one of each atlas. If that texture is already put in the atlas, then it is an error
        // to place it again and the user must be notified

        bool textureLoadedAlready = false;

        for(const auto &i : texturesInAtlas)
        {
            if(i == texture.textureLocation())
            {
                textureLoadedAlready = true;

                break;
            }
        }

        if(textureLoadedAlready)
        {
            std::string errorMessage;

            errorMessage += "The texture: ";
            errorMessage += texture.textureLocation().toStdString() + " has already been loaded.\n\n";
            errorMessage += "A texture cannot be added to an atlas twice.\n";
            errorMessage += "The requested operation will be aborted.";

            throw ::Exceptions::Atlas::TextureAlreadyLoaded{errorMessage};
        }

        selectedTexture->setTexture(texture);

        addTexture(selectedExistingTexture);

        for(auto &i : textureDrawingPositions)
        {
            i.surroundingBorder[currentZoomIndex].setSelectedBorderVisible(false);
        }
    }

    void TextureAtlas::textureLoaded(const std::vector<TextureLogic::Texture> &textures)
    {
        // Texture has been added to the texture bank. Note that when adding a texture to the image, the old vector of textures
        // might be moved in memory due to reallocation of the vector. Therefore references to that vector have to be reset just in case.

        this->textures = &textures;

        for(auto &i : textureDrawingPositions)
        {
            i.texture = &((*this->textures)[i.index]);
        }

        if(selectedExistingTexture->isOpen())
        {
            selectedExistingTexture->setTextureReference(((*this->textures)[selectedExistingTexture->getTextureIndex()]));

            firstMouse = true;
        }
    }

    void TextureAtlas::addTexture(SelectedTexture *selectedTexture)
    {
        if(intersectionOccured)
        {
            return;
        }

        if(selectedTexture->isOpen())
        {
            textureDrawingPositions.emplace_back();

            textureDrawingPositions.back().drawingPosition = selectedTexture->getDrawingCoordinates();

            textureDrawingPositions.back().texture = &selectedTexture->getImage();

            textureDrawingPositions.back().surroundingBorder = selectedTexture->getSurroundingBorder();

            texturesInAtlas.push_back(selectedTexture->getImage().textureLocation());

            for(int i = 0; i < textures->size(); ++i)
            {
                if((*textures)[i].textureLocation() == selectedTexture->getTextureLocation())
                {
                    textureDrawingPositions.back().index = i;

                    break;
                }
            }

            if(textureDrawingPositions.back().index == -1)
            {
                Q_ASSERT_X(false, __PRETTY_FUNCTION__, "Selected texture has a location not found in texture bank!");
            }
        }

//        auto textureInBank = std::find_if(textureBank->getTextures({}).begin(), textureBank->getTextures({}).end(),
//                                          [&textureLocation](const TextureLogic::Texture &texture)
//                                          {
//                                              return textureLocation ==  texture.textureLocation();
//                                          });
//
//        if(textureInBank != textures->end())
//        {
//            textureDrawingPositions.emplace_back();
//
//            textureDrawingPositions.back().drawingPosition = textureCoordinates;
//
//            textureDrawingPositions.back().texture = &*textureInBank;
//        }
    }
}