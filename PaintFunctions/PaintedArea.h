//
// Created by BinyBrion on 2019-10-01.
//

#ifndef TEXTUREATLASCREATOR_PAINTEDAREA_H
#define TEXTUREATLASCREATOR_PAINTEDAREA_H

#include <QtCore/QPoint>
#include <QPixmap>

namespace PaintFunctions
{
    struct PaintedArea
    {
        PaintedArea()
        {
            previousColour = QPixmap{QSize{1, 1}}.toImage();
        }

        QPoint appliedArea;
        QImage previousColour;
    };
}

#endif //TEXTUREATLASCREATOR_PAINTEDAREA_H
