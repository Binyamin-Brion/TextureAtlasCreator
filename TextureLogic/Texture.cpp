//
// Created by BinyBrion on 2019-09-12.
//

#include "Texture.h"

namespace TextureLogic
{
    Texture::Texture(const QString &texturePath) : _textureLocation{texturePath}
    {
        for(auto &i : AllZoomValues)
        {
            _texture[GetZoomIndex(i)].initialize(_textureLocation, i);
        }

        _textureLocation.replace('\\', '/');

        _textureName = _textureLocation.right(_textureLocation.size() - _textureLocation.lastIndexOf('/'));
    }

    const QImage& Texture::getImage(TextureLogic::Zoom zoom) const
    {
       return  _texture[GetZoomIndex(zoom)].getImage();
    }

    QImage& Texture::getSpecularTexture(TextureLogic::Zoom zoom, AccessRestriction::PassKey<GUI::CurrentTexture::RenderArea>)
    {
        return _texture[GetZoomIndex(zoom)].getSpecularTexture();
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