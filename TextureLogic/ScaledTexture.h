//
// Created by BinyBrion on 2019-09-12.
//

#ifndef TEXTUREATLAS_SCALEDTEXTURE_H
#define TEXTUREATLAS_SCALEDTEXTURE_H

#include <QtGui/QImage>
#include <stack>
#include "AccessRestriction/PassKey.h"
#include "Zoom.h"

class QPainter;

namespace PaintFunctions
{
    class PaintHistoryCommand;
}

namespace TextureLogic
{
    class ScaledTexture
    {
        public:

            void addPaintHistorySpecular(PaintFunctions::PaintHistoryCommand *paintHistoryCommand);
            void addPaintHistoryTexture(PaintFunctions::PaintHistoryCommand *paintHistoryCommand);
            const QImage& getImage() const;
            QImage& getSpecularTexture();
            void initialize(const QString &textureLocation, Zoom zoom);
            PaintFunctions::PaintHistoryCommand* removePaintHistorySpecular();
            PaintFunctions::PaintHistoryCommand* removePaintHistoryTexture();

        private:
            QImage image;
            QImage specularTexture;
            std::stack<PaintFunctions::PaintHistoryCommand*> specularPaintHistory;
            std::stack<PaintFunctions::PaintHistoryCommand*> texturePaintHistory;
    };
}

#endif //TEXTUREATLAS_SCALEDTEXTURE_H
