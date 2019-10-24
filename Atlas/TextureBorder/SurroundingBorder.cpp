//
// Created by BinyBrion on 2019-09-12.
//

#include "SurroundingBorder.h"

#include <QPainter>

namespace Atlas
{
    namespace TextureBorder
    {
        bool SurroundingBorder::checkIntersection(const SurroundingBorder &otherBorder)
        {
            // Intersection of the borders are commutative. It does not matter in what order the borders are checked.
            // Both of these operations should be the same- but just in case Qt's implementation of QRect gives different
            // results based off of the order of QRect implementation

            if(entireTextureArea.intersects(otherBorder.entireTextureArea) || otherBorder.entireTextureArea.intersects(entireTextureArea))
            {
                drawIntersectionBorder = true;

                const_cast<SurroundingBorder*>(&otherBorder)->drawIntersectionBorder = true;

                return true;
            }

            drawIntersectionBorder = false;

            const_cast<SurroundingBorder*>(&otherBorder)->drawIntersectionBorder = false;

            return false;
        }

        void SurroundingBorder::draw(QPainter &painter) const
        {
            // Make sure to restore the painter to its original state once the drawing of the border is done
            // A copy is done as it is not sure if when setting new pen if the old one is deleted- keeping a reference
            // to that would be a dangling reference

            const QPen previousPen = painter.pen();

            QPen pen{QColor{0, 0, 0,}, 0};

            painter.setPen(pen);

            if(drawIntersectionBorder)
            {
                for(const QRectF &rect : intersectionBorder)
                {
                    QPainterPath painterPath;

                    painterPath.addRect(rect);

                    painter.fillPath(painterPath, intersectionColour);

                    painter.drawPath(painterPath);
                }
            }
            else if(drawSelectionBorder)
            {
                for(const QRectF &rect : selectedBorder)
                {
                    QPainterPath painterPath;

                    painterPath.addRect(rect);

                    painter.fillPath(painterPath, selectionColour);

                    painter.drawPath(painterPath);
                }
            }

            painter.setPen(previousPen);
        }

        bool SurroundingBorder::getSelectedBorderVisible() const
        {
            return drawSelectionBorder;
        }

        QPointF SurroundingBorder::getTopLeftCorner() const
        {
            return entireTextureArea.topLeft();
        }

        /*
        *  The surrounding border looks like this:
        *
        *  ___________________________
        *  |_1_|________2________|_3_|
        *  |   |                 |   |
        *  |   |                 |   |
        *  |   |                 |   |
        *  |_4_|________9________|_5_|
        *  |_6_|________7________|_8_|
        *
        *  1. Top Left
        *  2. Top
        *  3. Top Right
        *  4. Left
        *  5. Right
        *  6. Bottom Left
        *  7. Bottom
        *  8. Bottom Right
        *  9. Texture Area
        *
        *  10. Sum of [1, 9]
        *
        *  The width of each rectangle depends on the specified border width (intersectionBorderWidth or  selectedBorderWidth)
        */

        void SurroundingBorder::initialize(int textureWidth, int textureHeight, int intersectionBorderWidth, int selectedBorderWidth)
        {
            this->intersectionBorderWidth = intersectionBorderWidth;

            this->selectedBorderWidth = selectedBorderWidth;

            intersectionBorder[BorderValue(Border::Top)] = QRect{0, -intersectionBorderWidth, textureWidth, intersectionBorderWidth};

            intersectionBorder[BorderValue(Border::Right)] = QRect{textureWidth, 0, intersectionBorderWidth, textureHeight};

            intersectionBorder[BorderValue(Border::Bottom)] = QRect{0, textureHeight, textureWidth, intersectionBorderWidth};

            intersectionBorder[BorderValue(Border::Left)] = QRect{-intersectionBorderWidth, 0, intersectionBorderWidth, textureHeight};

            intersectionBorder[BorderValue(Border::TopLeft)] = QRect{-intersectionBorderWidth, -intersectionBorderWidth, intersectionBorderWidth, intersectionBorderWidth};

            intersectionBorder[BorderValue(Border::TopRight)] = QRect{textureWidth, -intersectionBorderWidth, intersectionBorderWidth, intersectionBorderWidth};

            intersectionBorder[BorderValue(Border::BottomRight)] = QRect{textureWidth, textureHeight, intersectionBorderWidth, intersectionBorderWidth};

            intersectionBorder[BorderValue(Border::BottomLeft)] = QRect{-intersectionBorderWidth, textureHeight, intersectionBorderWidth, intersectionBorderWidth};


            selectedBorder[BorderValue(Border::Top)] = QRect{0, -selectedBorderWidth, textureWidth, selectedBorderWidth};

            selectedBorder[BorderValue(Border::Right)] = QRect{textureWidth, 0, selectedBorderWidth, textureHeight};

            selectedBorder[BorderValue(Border::Bottom)] = QRect{0, textureHeight, textureWidth, selectedBorderWidth};

            selectedBorder[BorderValue(Border::Left)] = QRect{-selectedBorderWidth, 0, selectedBorderWidth, textureHeight};

            selectedBorder[BorderValue(Border::TopLeft)] = QRect{-selectedBorderWidth, -selectedBorderWidth, selectedBorderWidth, selectedBorderWidth};

            selectedBorder[BorderValue(Border::TopRight)] = QRect{textureWidth, -selectedBorderWidth, selectedBorderWidth, selectedBorderWidth};

            selectedBorder[BorderValue(Border::BottomRight)] = QRect{textureWidth, textureHeight, selectedBorderWidth, selectedBorderWidth};

            selectedBorder[BorderValue(Border::BottomLeft)] = QRect{-selectedBorderWidth, textureHeight, selectedBorderWidth, selectedBorderWidth};

            entireTextureArea = QRect{-intersectionBorderWidth, -intersectionBorderWidth, textureWidth + 2 * intersectionBorderWidth, textureHeight + 2 * intersectionBorderWidth};

            intersectionColour = QColor{210, 0, 0};

            selectionColour = QColor{205, 98, 5};
        }

        void SurroundingBorder::setSelectedBorderVisible(bool value)
        {
            drawSelectionBorder = value;
        }

        void SurroundingBorder::translate(float differenceX, float differenceY)
        {
            for(QRectF &rect : intersectionBorder)
            {
                rect.translate(differenceX, differenceY);
            }

            for(QRectF &rect : selectedBorder)
            {
                rect.translate(differenceX, differenceY);
            }

            entireTextureArea.translate(differenceX, differenceY);
        }
    }
}