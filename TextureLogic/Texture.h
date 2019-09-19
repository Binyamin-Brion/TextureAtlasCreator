//
// Created by BinyBrion on 2019-09-12.
//

#ifndef TEXTUREATLAS_TEXTURE_H
#define TEXTUREATLAS_TEXTURE_H

#include "TextureLogic/ScaledTexture.h"
#include "TextureLogic/Zoom.h"
#include <array>

class QPainter;

namespace TextureLogic
{
    class Texture
    {
        using TextureImages = std::array<ScaledTexture, NumberZoomElements()>;

        public:
            explicit Texture(const QString &texturePath);
            bool checkIntersection(const Texture &otherTexture, Zoom zoom);
            void drawBorder(QPainter &painter, Zoom zoom) const;
            const QImage& getImage(TextureLogic::Zoom zoom) const;
            TextureImages& textureImages();
            const QString& textureLocation() const;
            const QString& textureName() const;
            void translate(int differenceX, int differenceY);

        private:

            QString _textureLocation;
            QString _textureName;

            TextureImages _texture;
    };
}

#endif //TEXTUREATLAS_TEXTURE_H
