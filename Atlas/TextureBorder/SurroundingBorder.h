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
        /**
         *  Represents the surrounding border around a texture. It is used to determine if another texture is
         *  overlapping with the current texture, in which case it will draw its border in a certain colour. Additionally,
         *  it is used to show that the user selected the texture by being drawn in a different colour.
         *
         *  This is done using a series of rectangle. See "SurroundingBorder.cpp" for a visual  of how this border looks like.
         */

        class SurroundingBorder
        {
                friend class Tests::SurroundingBorder::TestSurroundingBorder;

            public:

                /**
                 * Checks if two surrounding borders overlap.
                 *
                 * @param otherBorder the border to check if it overlaps with border
                 * @return true if this border and the other border overlap
                 */
                bool checkIntersection(const SurroundingBorder &otherBorder);

                /**
                 * Draws the border onto the widget that the passed in painter came from.
                 * Note: if both intersection and selection border are marked for drawing, only the intersection border
                 *       will be drawn.
                 *
                 * @param painter the painter object to be used for painting the textures
                 */
                void draw(QPainter &painter) const;

                /**
                 * Initializes the surrounding border with the values passed in. This is used as the selected texture
                 * changes, and the as such so does the surrounding border variable in class SelectedTexture.
                 *
                 * @param textureWidth the width of the surrounding border
                 * @param textureHeight the height of the surrounding border
                 * @param intersectionBorderWidth the width of the intersection border
                 * @param selectedBorderWidth the width of the selection border
                 */
                void initialize(int textureWidth, int textureHeight, int intersectionBorderWidth, int selectedBorderWidth);

                /**
                 * Set whether the selection border should be drawn.
                 * Note: if the intersection border is also marked to be drawn, the selected border will still not be drawn.
                 *
                 * @param value true if the selected border should be drawn
                 */
                void setSelectedBorderVisible(bool value);

                /**
                 * Translates the selected border by the given amount. Used when the texture that is associated with its
                 * surrounding border moves.
                 *
                 * @param differenceX the amount the surrounding border should be translated in the x-direction
                 * @param differenceY  the amount the surrounding border should be translated in the y-direction
                 */
                void translate(float differenceX, float differenceY);

            private:

                // Entire area used includes the actual texture plus its surrounding border. It is used for intersection tests.
                QRectF entireTextureArea;

                // Rectangles that make up the texture border. These are the rectangles that are drawn if the texture
                // is selected or if another texture intersects this texture.
                QRectF intersectionBorder[NumberBorderValues()];
                QRectF selectedBorder[NumberBorderValues()];

                // These variables exist for testing purposes; otherwise the widths can be obtained through a respective texture
                int intersectionBorderWidth;
                int selectedBorderWidth;

                // Holds the colour that the surrounding border should be painted with
                QColor intersectionColour;
                QColor selectionColour;

                // Controls whether the surrounding border is drawn (in case of selection border, if it is even considered to be drawn)
                bool drawIntersectionBorder = false;
                bool drawSelectionBorder = false;
        };
    }
}

#endif //TEXTUREATLAS_SURROUNDINGBORDER_H
