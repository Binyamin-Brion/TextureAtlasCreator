//
// Created by BinyBrion on 2019-09-12.
//

#include "Texture.h"
#include "PaintFunctions/PaintHistoryCommand.h"

namespace TextureLogic
{
    Texture::Texture(const QString &texturePath, unsigned int intersectionBorderWidth, unsigned int selectionBorderWidth) : _textureLocation{texturePath}
    {
        for(auto &i : AllZoomValues)
        {
            _texture[GetZoomIndex(i)].initialize(_textureLocation, i, intersectionBorderWidth, selectionBorderWidth);
        }

        _textureLocation.replace('\\', '/');

        _textureName = _textureLocation.right(_textureLocation.size() - _textureLocation.lastIndexOf('/') - 1);

        _textureFormat = _textureName.right(_textureLocation.size() - _textureLocation.lastIndexOf('.') - 1);

        _textureName.chop(_textureFormat.size() + 1);
    }

    void Texture::addPaintHistorySpecular(Zoom zoom, PaintFunctions::PaintHistoryCommand *paintHistoryCommand, AccessRestriction::PassKey<GUI::CurrentTexture::RenderArea>)
    {
        _texture[GetZoomIndex(zoom)].addPaintHistorySpecular(paintHistoryCommand);
    }

    void Texture::addPaintHistoryTexture(Zoom zoom, PaintFunctions::PaintHistoryCommand *paintHistoryCommand, AccessRestriction::PassKey<GUI::CurrentTexture::RenderArea>)
    {
        _texture[GetZoomIndex(zoom)].addPaintHistoryTexture(paintHistoryCommand);
    }

    void Texture::convertToFormat(QImage::Format newFormat)
    {
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

    QImage& Texture::getSpecularTexture(TextureLogic::Zoom zoom, AccessRestriction::PassKey<GUI::CurrentTexture::RenderArea>)
    {
        return _texture[GetZoomIndex(zoom)].getSpecularTexture();
    }

    const QString& Texture::getTextureDescription() const
    {
        return textureDescription;
    }

    PaintFunctions::PaintHistoryCommand* Texture::removeRecentPaintHistorySpecular(Zoom zoom, AccessRestriction::PassKey<GUI::CurrentTexture::RenderArea>)
    {
        return _texture[GetZoomIndex(zoom)].removePaintHistorySpecular();
    }

    PaintFunctions::PaintHistoryCommand* Texture::removeRecentPaintHistoryTexture(Zoom zoom, AccessRestriction::PassKey<GUI::CurrentTexture::RenderArea>)
    {
        return _texture[GetZoomIndex(zoom)].removePaintHistoryTexture();
    }

    void Texture::setIntersectionBorderWidth(unsigned int newWidth, TextureLogic::Zoom zoom, AccessRestriction::PassKey<TextureBank>)
    {
        for(auto &i : AllZoomValues)
        {
            float zoomFactor = TextureLogic::GetZoomValue(i) / TextureLogic::GetZoomValue(zoom);

            unsigned int adjustedWidth = newWidth * zoomFactor;

            _texture[GetZoomIndex(i)].setIntersectionWidth(adjustedWidth);
        }
    }

    void Texture::setSelectionBorderWidth(unsigned int newWidth, TextureLogic::Zoom zoom, AccessRestriction::PassKey<TextureBank>)
    {
        for(auto &i : AllZoomValues)
        {
            float zoomFactor = TextureLogic::GetZoomValue(i) / TextureLogic::GetZoomValue(zoom);

            unsigned int adjustedWidth = newWidth * zoomFactor;

            _texture[GetZoomIndex(i)].setSelectionBorderWidth(adjustedWidth);
        }
    }

    void Texture::setTextureDescription(const QString &description, AccessRestriction::PassKey<GUI::TextureInformation::SelectedTextureInformation>)
    {
        int descriptionLengthDifference = description.size() - textureDescription.size();

        if(descriptionLengthDifference > 0)
        {
            textureDescription += description[description.size() - 1];
        }
        else
        {
            textureDescription.chop(1);
        }
    }

    void Texture::setTextureLocation(const QString &textureLocation, AccessRestriction::PassKey<GUI::TextureInformation::SelectedTextureInformation>)
    {
        _textureLocation = textureLocation;

        _textureLocation.replace('\\', '/');

        QString tempTextureName = _textureLocation.right(_textureLocation.size() - _textureLocation.lastIndexOf('/') - 1);

        _textureFormat = tempTextureName.right(tempTextureName.size() - tempTextureName.lastIndexOf('.') - 1);
    }

    void Texture::setTextureName(const QString &textureName, AccessRestriction::PassKey<GUI::TextureInformation::SelectedTextureInformation>)
    {
        _textureName = textureName;
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
}