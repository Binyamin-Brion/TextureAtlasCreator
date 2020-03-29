//
// Created by BinyBrion on 2019-09-12.
//

#include "Texture.h"
#include "PaintFunctions/PaintHistoryCommand.h"

namespace TextureLogic
{
    Texture::Texture(const QString &texturePath, unsigned int intersectionBorderWidth, unsigned int selectionBorderWidth)
                :
                    _textureLocation{texturePath}
    {
        // To ensure everything works as expected on Windows and Linux, ensure only forward slashes in file system locations.
        _textureLocation.replace('\\', '/');

        // The texture name is any text after the last slash, minus text after the period denoting the file type.
        _textureName = _textureLocation.right(_textureLocation.size() - _textureLocation.lastIndexOf('/') - 1);

        // File type (and therefore texture type here) is everything after the period in a file location.
        _textureFormat = _textureName.right(_textureLocation.size() - _textureLocation.lastIndexOf('.') - 1);

        // File type not part of texture name.
        _textureName.chop(_textureFormat.size() + 1);

        QString specularTextureLocation = texturePath;
        specularTextureLocation.remove('.' + _textureFormat);
        specularTextureLocation += "_Specular." + _textureFormat;

        // Since the scaled textures are stored in an array, if they do not have default constructors then initialization
        // the array becomes hard, as the array, and therefore the scaled textures, are created before this constructor body is run.
        // Having the scaled texture have an initialize function is a solution to this problem.

        for(auto &i : AllZoomValues)
        {
            _texture[GetZoomIndex(i)].initialize(_textureLocation, specularTextureLocation, i, intersectionBorderWidth, selectionBorderWidth);
        }
    }

    void Texture::addPaintHistorySpecular(Zoom zoom, PaintFunctions::PaintHistoryCommand *paintHistoryCommand, AccessRestriction::PassKey<GUI::CurrentTexture::PaintArea>)
    {
        _texture[GetZoomIndex(zoom)].addPaintHistorySpecular(paintHistoryCommand);
    }

    void Texture::addPaintHistoryTexture(Zoom zoom, PaintFunctions::PaintHistoryCommand *paintHistoryCommand, AccessRestriction::PassKey<GUI::CurrentTexture::PaintArea>)
    {
        _texture[GetZoomIndex(zoom)].addPaintHistoryTexture(paintHistoryCommand);
    }

    void Texture::convertToFormat(QImage::Format newFormat)
    {
        // Since the scaled textures represent the original texture, they must have the same format.
        for(auto &i : AllZoomValues)
        {
            _texture[GetZoomIndex(i)].convertToFormat(newFormat);
        }
    }

    const QImage& Texture::getImage(TextureLogic::Zoom zoom) const
    {
       return  _texture[GetZoomIndex(zoom)].getImage();
    }

    unsigned int Texture::getIntersectionBorderWidth(TextureLogic::Zoom zoom) const
    {
        return _texture[GetZoomIndex(zoom)].getIntersectionBorderWidth();
    }

    unsigned int Texture::getSelectedBorderWidth(TextureLogic::Zoom zoom) const
    {
        return _texture[GetZoomIndex(zoom)].getSelectionBorderWidth();
    }

    const QImage& Texture::getSpecularTexture(TextureLogic::Zoom zoom) const
    {
        return _texture[GetZoomIndex(zoom)].getSpecularTexture();
    }

    PaintFunctions::PaintHistoryCommand* Texture::removeRecentPaintHistorySpecular(Zoom zoom, AccessRestriction::PassKey<GUI::CurrentTexture::PaintArea>)
    {
        return _texture[GetZoomIndex(zoom)].removePaintHistorySpecular();
    }

    PaintFunctions::PaintHistoryCommand* Texture::removeRecentPaintHistoryTexture(Zoom zoom, AccessRestriction::PassKey<GUI::CurrentTexture::PaintArea>)
    {
        return _texture[GetZoomIndex(zoom)].removePaintHistoryTexture();
    }

    void Texture::setIntersectionBorderWidth(unsigned int newWidth, TextureLogic::Zoom zoom, AccessRestriction::PassKey<TextureBank>)
    {
        setIntersectionBorderWidthInternal(newWidth, zoom);
    }

    void Texture::setSelectionBorderWidth(unsigned int newWidth, TextureLogic::Zoom zoom, AccessRestriction::PassKey<TextureBank>)
    {
        // The scaled textures representing different zooms that what was passed in have to be updated so that they remain relative
        // to one another.
        for(auto &i : AllZoomValues)
        {
            float zoomFactor = TextureLogic::GetZoomValue(i) / TextureLogic::GetZoomValue(zoom);

            unsigned int adjustedWidth = newWidth * zoomFactor;

            _texture[GetZoomIndex(i)].setSelectionBorderWidth(adjustedWidth);
        }
    }

    void Texture::setTextureLocation(const QString &textureLocation, AccessRestriction::PassKey<GUI::TextureInformation::SelectedTextureInformation>)
    {
        // Same idea as what is done in constructor
        _textureLocation = textureLocation;

        _textureLocation.replace('\\', '/');

        QString tempTextureName = _textureLocation.right(_textureLocation.size() - _textureLocation.lastIndexOf('/') - 1);

        _textureFormat = tempTextureName.right(tempTextureName.size() - tempTextureName.lastIndexOf('.') - 1);
    }

    const QString& Texture::textureFormat() const
    {
        return _textureFormat;
    }

    const QString &Texture::textureLocation() const
    {
        return _textureLocation;
    }

    const QString &Texture::textureName() const
    {
        return _textureName;
    }

    // Beginning of private functions

    void Texture::setIntersectionBorderWidthInternal(unsigned int newWidth, Zoom zoom)
    {
        // The scaled textures representing different zooms that what was passed in have to be updated so that they remain relative
        // to one another.
        for(auto &i : AllZoomValues)
        {
            float zoomFactor = TextureLogic::GetZoomValue(i) / TextureLogic::GetZoomValue(zoom);

            unsigned int adjustedWidth = newWidth * zoomFactor;

            _texture[GetZoomIndex(i)].setIntersectionWidth(adjustedWidth);
        }
    }
}