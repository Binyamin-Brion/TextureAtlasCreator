//
// Created by BinyBrion on 2019-09-27.
//

#ifndef TEXTUREATLASCREATOR_BRUSH_H
#define TEXTUREATLASCREATOR_BRUSH_H

#include <QtCore/QRect>
#include <QtGui/QColor>
#include <QtGui/QImage>
#include "TextureLogic/Zoom.h"

namespace PaintFunctions
{
    class Brush
    {
        public:
            Brush();

            const QImage& getPaintImage(TextureLogic::Zoom zoom) const;
            void setPaintTypeSolid(TextureLogic::Zoom zoom, QSize drawAreaSize, QColor colour);

        private:

            std::array<QImage, TextureLogic::NumberZoomElements()> paintImages;
    };
}

#endif //TEXTUREATLASCREATOR_BRUSH_H
