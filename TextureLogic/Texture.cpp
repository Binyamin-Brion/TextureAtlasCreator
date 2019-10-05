//
// Created by BinyBrion on 2019-09-12.
//

#include "Texture.h"
#include "PaintFunctions/PaintHistoryCommand.h"

namespace TextureLogic
{
    Texture::Texture(const QString &texturePath) : _textureLocation{texturePath}
    {
        for(auto &i : AllZoomValues)
        {
            _texture[GetZoomIndex(i)].initialize(_textureLocation, i);
        }

        _textureLocation.replace('\\', '/');

        _textureName = _textureLocation.right(_textureLocation.size() - _textureLocation.lastIndexOf('/') - 1);

        _textureFormat = _textureName.right(_textureLocation.size() - _textureLocation.lastIndexOf('.') - 1);

        _textureName.chop(_textureFormat.size() + 1);
    }

    void Texture::addPaintHistorySpecular(PaintFunctions::PaintHistoryCommand *paintHistoryCommand, AccessRestriction::PassKey<GUI::CurrentTexture::RenderArea>)
    {
        _texture[GetZoomIndex(Zoom::Normal)].addPaintHistorySpecular(paintHistoryCommand);
    }

    void Texture::addPaintHistoryTexture(PaintFunctions::PaintHistoryCommand *paintHistoryCommand, AccessRestriction::PassKey<GUI::CurrentTexture::RenderArea>)
    {
        _texture[GetZoomIndex(Zoom::Normal)].addPaintHistoryTexture(paintHistoryCommand);
    }

    const QImage& Texture::getImage(TextureLogic::Zoom zoom) const
    {
       return  _texture[GetZoomIndex(zoom)].getImage();
    }

    QImage& Texture::getSpecularTexture(TextureLogic::Zoom zoom, AccessRestriction::PassKey<GUI::CurrentTexture::RenderArea>)
    {
        return _texture[GetZoomIndex(zoom)].getSpecularTexture();
    }

    const QString& Texture::getTextureDescription(AccessRestriction::PassKey<GUI::TextureInformation::SelectedTextureInformation>) const
    {
        return textureDescription;
    }

    PaintFunctions::PaintHistoryCommand* Texture::removeRecentPaintHistorySpecular(AccessRestriction::PassKey<GUI::CurrentTexture::RenderArea>)
    {
        return _texture[GetZoomIndex(Zoom::Normal)].removePaintHistorySpecular();
    }

    PaintFunctions::PaintHistoryCommand* Texture::removeRecentPaintHistoryTexture(AccessRestriction::PassKey<GUI::CurrentTexture::RenderArea>)
    {
        return _texture[GetZoomIndex(Zoom::Normal)].removePaintHistoryTexture();
    }

    void Texture::setTextureDescription(const QString &description, AccessRestriction::PassKey<GUI::TextureInformation::SelectedTextureInformation>)
    {
        int descriptionLengthDifference = description.size() - textureDescription.size();

        if(descriptionLengthDifference > 0)
        {
            //textureDescription += description.mid(textureDescription.size() + 1);
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

    Texture::TextureImages &Texture::textureImages()
    {
        return _texture;
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