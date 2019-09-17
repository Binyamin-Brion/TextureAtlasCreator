//
// Created by BinyBrion on 2019-09-14.
//

#include "TextureAtlas.h"

#include "TextureLogic/TextureBank.h"
#include "Atlas/SelectedTexture.h"

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

    }

    void TextureAtlas::draw(QPainter &painter)
    {
        for(const auto &i : textureDrawingPositions)
        {
            painter.drawImage(i.drawingPosition, i.texture->getImage(currentZoom));
        }

        if(selectedTexture->isOpen())
        {
            painter.drawImage(selectedTexture->getDrawingCoordinates(), selectedTexture->getImage().getImage(currentZoom));
        }
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
            selectedTexture->move(mouseX, mouseY);
        }
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
            // Throw appropriate texture
        }

        selectedTexture->setTexture(texture);
    }


    void TextureAtlas::textureLoaded(const std::vector<TextureLogic::Texture> &textures)
    {
        // Texture has been added to the texture bank. Note that when adding a texture to the image, the old vector of textures
        // might be moved in memory due to reallocation of the vector. Therefore references to that vector have to be reset just in case.

        this->textures = &textures;

        for(int i = 0; i < this->textures->size() - 1; ++i)
        {
            textureDrawingPositions[i].texture = &((*this->textures)[i]);
        }
    }

    void TextureAtlas::addTexture()
    {
        if(selectedTexture->isOpen())
        {
            textureDrawingPositions.emplace_back();

            textureDrawingPositions.back().drawingPosition = selectedTexture->getDrawingCoordinates();

            textureDrawingPositions.back().texture = &selectedTexture->getImage();
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