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
    /*
     * Represents a brush that a user can use to paint a currently selected texture.
     * It is immutable. Every time a new brush is needed, it is completely recreated
     * using the setPaintTypeSolid function.
     */

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
