//
// Created by BinyBrion on 2019-10-01.
//

#ifndef TEXTUREATLASCREATOR_PAINTEDAREA_H
#define TEXTUREATLASCREATOR_PAINTEDAREA_H

#include <QtCore/QPoint>
#include <QPixmap>

namespace PaintFunctions
{
    /**
     * Used for storing the paint history. Keeps track of what the currently texture looked like before
     * it was painted on. Using size of the painted area as well as the top left corner of the painted area,
     * a part of a texture can be referred to.
     */

    struct PaintedArea
    {
        /**
         *  Initializes resources.
         */
        PaintedArea()
        {
            // Has to be initialized to known state before it is used.
            previousColour = QPixmap{QSize{1, 1}}.toImage();
        }

        QPoint appliedArea;
        QImage previousColour;
    };
}

#endif //TEXTUREATLASCREATOR_PAINTEDAREA_H
