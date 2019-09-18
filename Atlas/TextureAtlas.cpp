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
            painter.drawImage(selectedTexture->getDrawingCoordinates(), selectedTexture->getImageForDrawing().getImage(currentZoom));
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
            selectedTexture->move(mouseX, mouseY, atlasSize);
        }
    }

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
            // Throw appropriate texture
        }

        selectedTexture->setTexture(texture);
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