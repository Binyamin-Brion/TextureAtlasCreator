//
// Created by BinyBrion on 2019-09-27.
//

#ifndef TEXTUREATLASCREATOR_BRUSH_H
#define TEXTUREATLASCREATOR_BRUSH_H

#include <QtCore/QRect>
#include <QtGui/QColor>
#include <QtGui/QImage>

namespace PaintFunctions
{
    class Brush
    {
        public:
            Brush();

            const QImage& getPaintImage() const;
            void setPaintTypeSolid(QSize drawAreaSize, QColor colour);

        private:

            QImage paintImage;
    };
}

#endif //TEXTUREATLASCREATOR_BRUSH_H
