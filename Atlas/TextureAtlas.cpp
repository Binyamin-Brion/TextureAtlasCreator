//
// Created by BinyBrion on 2019-09-14.
//

#include "TextureAtlas.h"

#include "TextureLogic/TextureBank.h"
#include "Atlas/SelectedTexture.h"
#include "Exceptions/Atlas/TextureAlreadyLoaded.h"

#include <QPainter>

namespace Atlas
{
    TextureAtlas::TextureAtlas()
    {
        selectedTexture = new SelectedTexture;

        currentZoom = TextureLogic::Zoom::Normal;
    }

    bool TextureAtlas::checkIntersection()
    {
        // A texture can only intersect with other textures if it's moving, in which case a texture must have
        // been selected (does not matter which one, as long as there is one selected)

        if(!selectedTexture->isOpen())
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

            auto& textureNotConst = const_cast<TextureLogic::Texture&>(*i.texture);

            intersectionOccured |= textureNotConst.checkIntersection(selectedTexture->getImageForDrawing(), currentZoom);
        }
    }

    void TextureAtlas::draw(QPainter &painter)
    {
        // Note the order of drawing: the selected texture should be drawn last so that it is always drawn,
        // even when over another texture

        for(const auto &i : textureDrawingPositions)
        {
            painter.drawImage(i.drawingPosition, i.texture->getImage(currentZoom));

            i.texture->drawBorder(painter, currentZoom);
        }

        if(selectedTexture->isOpen())
        {
            painter.drawImage(selectedTexture->getDrawingCoordinates(), selectedTexture->getImageForDrawing().getImage(currentZoom));
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

        return {false, QSize{-1, -1}};
    }

    std::pair<bool, QSize> TextureAtlas::getSelectedTextureSize() const
    {
        // Same idea for using pair and checking if a texture is selected as fn getAtlasSize

        if(selectedTexture->isOpen())
        {
            return {true,
                    QSize{selectedTexture->getImageForDrawing().getImage(currentZoom).width(), selectedTexture->getImageForDrawing().getImage(currentZoom).height()}};
        }

        return{false, QSize{-1, -1}};
    }

    void TextureAtlas::mouseClicked()
    {
        if(selectedTexture->isOpen())
        {
            addTexture();
        }
        else
        {

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
                selectedTextureNotConst.translate(newMouseX, newMouseY);

                firstMouse = false;
            }
            else
            {
                int differenceX = newMouseX - previousMouseX;

                int differenceY = newMouseY - previousMouseY;

                selectedTextureNotConst.translate(differenceX, differenceY);
            }

            previousMouseX = newMouseX;

            previousMouseY = newMouseY;

            selectedTexture->move(mouseX, mouseY, atlasSize);

            checkIntersection();
        }
    }

    // If the texture is opened, then there is chance it is being attempted to be moved to an invalid location.
    // Before that is done, the cursor has to be moved back to a valid location, which this function provides.
    // For example: cursor moving texture outside of defined atlas size. AtlasWidget has to check if such a condition occurs

    std::pair<bool, QPoint> TextureAtlas::resetCursorPosition() const
    {
        if(!selectedTexture->isOpen())
        {
            return {false, QPoint{-1, -1}};
        }

        int newMouseX = selectedTexture->getDrawingCoordinates().x() + selectedTexture->getImageForDrawing().getImage(currentZoom).width() / 2;

        int newMouseY = selectedTexture->getDrawingCoordinates().y() + selectedTexture->getImageForDrawing().getImage(currentZoom).height() / 2;

        return {true, QPoint{newMouseX, newMouseY}};
    }

    void TextureAtlas::resetFirstMouse()
    {
        // Only reset the status indicating it is the first time the cursor is entering the atlas widget if a texture is not
        // opened; otherwise there is a valid initial cursor position to use for a translation

        firstMouse = !selectedTexture->isOpen();
    }

    void TextureAtlas::setAtlasSize(QSize size)
    {
        atlasSize = size;
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

        texturesInAtlas.push_back(texture.textureLocation());
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
    }

    void TextureAtlas::addTexture()
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