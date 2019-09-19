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

    bool Texture::checkIntersection(const TextureLogic::Texture &otherTexture, TextureLogic::Zoom zoom)
    {
        if(this == &otherTexture)
        {
            return false;
        }

        return _texture[GetZoomIndex(zoom)].checkIntersection(otherTexture._texture[GetZoomIndex(zoom)]);
    }

    void Texture::drawBorder(QPainter &painter, Zoom zoom) const
    {
        _texture[GetZoomIndex(zoom)].drawBorder(painter);
    }

    const QImage& Texture::getImage(TextureLogic::Zoom zoom) const
    {
       return  _texture[GetZoomIndex(zoom)].getImage();
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

    void Texture::translate(int differenceX, int differenceY)
    {
        for(auto &i : _texture)
        {
            i.translate(differenceX, differenceY);
        }
    }
}