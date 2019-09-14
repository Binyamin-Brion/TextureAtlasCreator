//
// Created by BinyBrion on 2019-09-12.
//

#ifndef TEXTUREATLAS_SCALEDTEXTURE_H
#define TEXTUREATLAS_SCALEDTEXTURE_H

#include <QtGui/QImage>
#include "TextureBorder/SurroundingBorder.h"
#include "Zoom.h"

namespace TextureLogic
{
    class ScaledTexture
    {
        public:
            bool checkIntersection(const ScaledTexture &scaledTexture);
            void initialize(const QString &textureLocation, Zoom zoom);

        private:
            QImage image;
            TextureBorder::SurroundingBorder border;
    };
}

#endif //TEXTUREATLAS_SCALEDTEXTURE_H
