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
    /**
     * Represents a brush that a user can use to paint a currently selected texture.
     * It is immutable. Every time a new brush is needed, it is completely recreated
     * using the setPaintTypeSolid function.
     */

    class Brush
    {
        public:

            /**
             *  Initializes required internal resources.
             */
            Brush();

            /**
             *  Get the paint image used for the passed in zoom.
             *
             * @param zoom that the paint image is used for
             * @return paint area for the passed in zoom
             */
            const QImage& getPaintImage(TextureLogic::Zoom zoom) const;

            /**
             *  Returns whether the user is painting the specular texture.
             *
             * @return true if the user is painting the specular texture
             */
            bool getPaintingSpecularTexture() const;

            /**
             * Sets whether the user is painting the specular texture.
             *
             * @param value true if the user selected to paint the specular texture
             */
            void setPaintingSpecularTexture(bool value);

            /**
             * Resizes the required paint image to reflect the appropriate size for the passed in zoom. The other paint
             * images are scaled as required for the other zoom values.
             *
             * @param zoom that the passed in drawAreaSize is referring to
             * @param drawAreaSize size of the brush
             * @param colour of the brush
             */
            void setPaintTypeSolid(TextureLogic::Zoom zoom, QSize drawAreaSize, QColor colour);

        private:
            std::array<QImage, TextureLogic::NumberZoomElements()> paintImages;
            bool paintingSpecularTexture = false;
    };
}

#endif //TEXTUREATLASCREATOR_BRUSH_H
