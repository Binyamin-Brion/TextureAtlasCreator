//
// Created by BinyBrion on 2019-09-12.
//

#ifndef TEXTUREATLAS_SURROUNDINGBORDER_H
#define TEXTUREATLAS_SURROUNDINGBORDER_H

#include <QtCore/QRect>
#include <QtGui/QColor>
#include "Border.h"

class QPainter;

namespace Tests
{
    namespace SurroundingBorder
    {
        class TestSurroundingBorder;
    }
}

namespace Atlas
{
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
                friend class Tests::SurroundingBorder::TestSurroundingBorder;

            public:

                bool checkIntersection(const SurroundingBorder &otherBorder);
                void draw(QPainter &painter) const;
                bool getSelectedBorderVisible() const;
                QPointF getTopLeftCorner() const;
                void initialize(int textureWidth, int textureHeight, int intersectionBorderWidth, int selectedBorderWidth);
                void setSelectedBorderVisible(bool value);
                void translate(float differenceX, float differenceY);

            private:

                // Entire area used includes the actual texture plus its surrounding border. It is used for
                // intersection tests.

                QRectF entireTextureArea;

                // Rectangles that make up the texture border. These are the rectangles that are drawn if the texture
                // is selected or if another texture intersects this texture.

                QRectF intersectionBorder[NumberBorderValues()];
                QRectF selectedBorder[NumberBorderValues()];

                // These variables exist for testing purposes; otherwise the widths can be obtained through
                // a respective texture
                int intersectionBorderWidth;
                int selectedBorderWidth;

                QColor intersectionColour;
                QColor selectionColour;

                bool drawIntersectionBorder = false;
                bool drawSelectionBorder = false;
        };
    }
}

#endif //TEXTUREATLAS_SURROUNDINGBORDER_H
