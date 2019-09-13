//
// Created by BinyBrion on 2019-09-12.
//

#ifndef TEXTUREATLAS_SCALEDTEXTURE_H
#define TEXTUREATLAS_SCALEDTEXTURE_H

#include <QtGui/QImage>
#include "Zoom.h"

namespace TextureLogic
{
    class ScaledTexture
    {
        public:
            void initialize(const QString &textureLocation, Zoom zoom);


        private:
            QImage image;
    };
}

#endif //TEXTUREATLAS_SCALEDTEXTURE_H
