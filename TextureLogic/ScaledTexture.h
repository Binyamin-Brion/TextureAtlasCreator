//
// Created by BinyBrion on 2019-09-12.
//

#ifndef TEXTUREATLAS_SCALEDTEXTURE_H
#define TEXTUREATLAS_SCALEDTEXTURE_H

#include <QtGui/QImage>
#include "AccessRestriction/PassKey.h"
#include "Zoom.h"

class QPainter;

namespace TextureLogic
{
    class ScaledTexture
    {
        public:

            const QImage& getImage() const;
            QImage& getSpecularTexture();
            void initialize(const QString &textureLocation, Zoom zoom);

        private:
            QImage image;
            QImage specularTexture;
    };
}

#endif //TEXTUREATLAS_SCALEDTEXTURE_H
