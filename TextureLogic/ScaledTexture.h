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
            void convertToFormat(QImage::Format newFormat);
            const QImage& getImage() const;
            unsigned int getIntersectionBorderWidth() const;
            unsigned int getSelectionBorderWidth() const;
            QImage& getSpecularTexture();
            void initialize(const QString &textureLocation, Zoom zoom, unsigned int intersectionBorderWidth, unsigned int selectionBorderWidth);
            PaintFunctions::PaintHistoryCommand* removePaintHistorySpecular();
            PaintFunctions::PaintHistoryCommand* removePaintHistoryTexture();
            void setIntersectionWidth(unsigned int borderWidth);
            void setSelectionBorderWidth(unsigned int borderWidth);

        private:
            QImage image;
            QImage specularTexture;
            std::stack<PaintFunctions::PaintHistoryCommand*> specularPaintHistory;
            std::stack<PaintFunctions::PaintHistoryCommand*> texturePaintHistory;

            unsigned int intersectionBorderWidth;
            unsigned int selectionBorderWidth;
    };
}

#endif //TEXTUREATLAS_SCALEDTEXTURE_H
