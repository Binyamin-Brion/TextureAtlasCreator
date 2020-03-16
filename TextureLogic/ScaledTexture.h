//
// Created by BinyBrion on 2019-09-12.
//

#ifndef TEXTUREATLAS_SCALEDTEXTURE_H
#define TEXTUREATLAS_SCALEDTEXTURE_H

#include <QtGui/QImage>
#include <stack>
#include "HelperUtilities/PassKey.h"
#include "Zoom.h"

class QPainter;

namespace PaintFunctions
{
    class PaintHistoryCommand;
}

namespace TextureLogic
{
    /**
     * Represents a scaled version of an original texture, and all of the associated information of a texture, such as
     * border widths and paint history.
     */

    class ScaledTexture
    {
        public:

            /**
             *  Add to the paint history of this texture's specular map after a paint operation was performed.
             *
             * @param paintHistoryCommand the history of the paint operation on the specular map
             */
            void addPaintHistorySpecular(PaintFunctions::PaintHistoryCommand *paintHistoryCommand);

            /**
             *  Same as addPaintHistorySpecular, but for the diffuse texture (ie texture itself).
             *
             * @param paintHistoryCommand the history of the paint operation on the diffuse map
             */
            void addPaintHistoryTexture(PaintFunctions::PaintHistoryCommand *paintHistoryCommand);

            /**
             * Converts this texture to the format specified.
             *
             * @param newFormat that this texture should have in the future
             */
            void convertToFormat(QImage::Format newFormat);

            /**
             * Get reference to the scaled texture itself.
             *
             * @return immutable reference to the texture resource
             */
            const QImage& getImage() const;

            /**
             * Get the intersection border width for this scaled texture.
             *
             * @return intersection border width for this texture
             */
            unsigned int getIntersectionBorderWidth() const;

            /**
             * Get the selection border width for this scaled texture.
             *
             * @return selection border width for this texture
             */
            unsigned int getSelectionBorderWidth() const;

            /**
             * Get reference to the specular map for this texture.
             *
             * @return reference to the specular map
             */
            const QImage& getSpecularTexture() const;

            /**
             * Serves as the constructor for this class, initializing all required variables.
             *
             * @param textureLocation of the texture this object is representing
             * @param zoom that the texture should be scaled to and that this object will be represent
             * @param intersectionBorderWidth of the texture at the passed in zoom level
             * @param selectionBorderWidth of the texture at the passed in zoom level
             */
            void initialize(const QString &textureLocation, const QString &specularTextureLocation, Zoom zoom, unsigned int intersectionBorderWidth, unsigned int selectionBorderWidth);

            /**
             * Gets the history of the most recent specular map paint history. If there is no more recent paint history, which would happen
             * if this function is called repeatedly, a nullptr is returned.
             *
             * @return pointer to the most recent specular map paint history
             */
            PaintFunctions::PaintHistoryCommand* removePaintHistorySpecular();

            /**
             * Gets the history of the most recent diffuse map paint history. If there is no more recent paint history, which would happen
             * if this function is called repeatedly, a nullptr is returned.
             *
             * @return pointer to the most recent diffuse map paint history
             */
            PaintFunctions::PaintHistoryCommand* removePaintHistoryTexture();

            /**
             * Set the intersection border width for this scaled texture.
             *
             * @param borderWidth new intersection border width
             */
            void setIntersectionWidth(unsigned int borderWidth);

            /**
             * Set the intersection border width for this selection texture.
             *
             * @param borderWidth new selection border width
             */
            void setSelectionBorderWidth(unsigned int borderWidth);

        private:
            // The actual texture resource; holds the scaled texture. Renaming this variable would be a lot of work as
            // this name is used in code and comments throughout project; decent risk renaming this and something breaking.
            // It is not worth renaming, though this variable should have been named appropriately in the first place.
            QImage image;
            QImage specularTexture;

            // Stack is used as LIFO behaviour logically models undo operations, which is what paint operation history are used for.
            std::stack<PaintFunctions::PaintHistoryCommand*> specularPaintHistory;
            std::stack<PaintFunctions::PaintHistoryCommand*> texturePaintHistory;

            // These represent the border widths for the zoom level specified for this instance; NOT the original texture
            // at 100% (ie Normal) zoom.
            unsigned int intersectionBorderWidth;
            unsigned int selectionBorderWidth;
    };
}

#endif //TEXTUREATLAS_SCALEDTEXTURE_H
