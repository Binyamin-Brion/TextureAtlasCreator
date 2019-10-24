//
// Created by BinyBrion on 2019-09-12.
//

#ifndef TEXTUREATLAS_BORDER_H
#define TEXTUREATLAS_BORDER_H

namespace Atlas
{
    namespace TextureBorder
    {
        /*
         * Specifies which part of the texture is being compared against with another texture.
         * No more enum values should be added, as the below values describe the entire area around a
         * rectangle.
         */

        enum class Border : unsigned int
        {
            Top = 0,
            Right = 1,
            Bottom = 2,
            Left = 3,
            TopLeft = 4,
            TopRight = 5,
            BottomRight = 6,
            BottomLeft = 7,
            ALWAYS_AT_END = 8 // Number of preceding enums
        };

        // Provides easy access to iterate over enum values. Make sure that if a new enum values is added, it is
        // added to this array. Keep the enums listed here in the same order as they are declared for consistency.

        const static Border AllBorders[] =
                {
                    Border::Top,
                    Border::Right,
                    Border::Bottom,
                    Border::Left,
                    Border::TopLeft,
                    Border::TopRight,
                    Border::BottomRight,
                    Border::BottomLeft
                };

        constexpr unsigned int NumberBorderValues()
        {
            return static_cast<unsigned int>(Border::ALWAYS_AT_END);
        }

        constexpr unsigned int BorderValue(Border border)
        {
            return static_cast<unsigned int>(border);
        }
    }
};

#endif //TEXTUREATLAS_BORDER_H
