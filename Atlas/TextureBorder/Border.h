//
// Created by BinyBrion on 2019-09-12.
//

#ifndef TEXTUREATLAS_BORDER_H
#define TEXTUREATLAS_BORDER_H

namespace Atlas
{
    namespace TextureBorder
    {
        /**
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

        /**
         * Provides easy access to iterate over enum values. Make sure that if a new enum values is added, it is
        *  added to this array. Keep the enums listed here in the same order as they are declared for consistency.
        */
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

        /**
         * Casts the last enum of Border representing how many enums there are as an unsigned int.
         * Useful to use in loops.
         *
         * @return the unsigned int representation of how many enums are in Border
         */
        constexpr unsigned int NumberBorderValues()
        {
            return static_cast<unsigned int>(Border::ALWAYS_AT_END);
        }

        /**
         * Casts the passed in enum value to an unsigned int representation. Useful for indexing in an array or vector.
         *
         * @param border the enum value to be casted to its associated unsigned int value
         * @return the unsigned int value of the enum passed in
         */
        constexpr unsigned int BorderValue(Border border)
        {
            return static_cast<unsigned int>(border);
        }
    }
};

#endif //TEXTUREATLAS_BORDER_H
