//
// Created by BinyBrion on 2019-09-12.
//

#ifndef TEXTUREATLAS_SURROUNDINGBORDER_H
#define TEXTUREATLAS_SURROUNDINGBORDER_H

#include <QtCore/QRect>
#include <QtGui/QColor>
#include "TextureLogic/TextureBorder/Border.h"

class QPainter;

namespace TextureLogic
{
    class Texture;

    namespace TextureBorder
    {
        /*
         *  Represents the surrounding border around a texture. It is used to determine if another texture is
         *  overlapping with the current texture, in which case it will draw its border in a certain colour. Additionally,
         *  it is used to show that the user selected the texture by being drawn in a different colour.
         *
         *  This is done using a series of rectangle. See "SurroundingBorder.cpp" for a visual
         *  of how this border looks like.
         */

        class SurroundingBorder
        {
            public:
                SurroundingBorder(int textureWidth, int textureHeight);
                bool checkIntersection(const SurroundingBorder &otherBorder);
                void draw(QPainter &painter);
                void setSelectedBorderVisible(bool value);
                void translate(int differenceX, int differenceY);

            private:

                // Entire area used includes the actual texture plus its surrounding border. It is used for
                // intersection tests.

                QRect entireTextureArea;

                // Rectangles that make up the texture border. These are the rectangles that are drawn if the texture
                // is selected or if another texture intersects this texture.

                QRect intersectionBorder[NumberBorderValues()];
                QRect selectedBorder[NumberBorderValues()];

                int intersectionBorderWidth = 5;
                int selectedBorderWidth = 2;

                QColor intersectionColour;
                QColor selectionColour;

                bool drawIntersectionBorder = false;
                bool drawSelectionBorder = false;
        };
    }
}

#endif //TEXTUREATLAS_SURROUNDINGBORDER_H