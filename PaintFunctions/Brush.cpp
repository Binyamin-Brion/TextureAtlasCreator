//
// Created by BinyBrion on 2019-09-27.
//

#include "Brush.h"
#include <QPixmap>

namespace PaintFunctions
{
    Brush::Brush()
    {
        paintImage = QPixmap{QSize{1, 1}}.toImage();
    }

    const QImage& Brush::getPaintImage() const
    {
        return paintImage;
    }

    void Brush::setPaintTypeSolid(QSize drawAreaSize, QColor colour)
    {
        paintImage = paintImage.scaled(drawAreaSize);
        paintImage.fill(colour);

    }
}