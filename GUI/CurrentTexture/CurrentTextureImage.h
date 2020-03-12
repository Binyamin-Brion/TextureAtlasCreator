//
// Created by BinyBrion on 2019-09-26.
//

#ifndef TEXTUREATLASCREATOR_CURRENTTEXTUREIMAGE_H
#define TEXTUREATLASCREATOR_CURRENTTEXTUREIMAGE_H

namespace GUI
{
    namespace CurrentTexture
    {
        /**
         * Enum to represent what type of texture a render area is holding.
         */

        enum class CurrentTextureImage : unsigned int
        {
            SelectedTexture = 0,
            SpecularTexture = 1,
        };

        /**
         * Converts a CurrentTextureImage enum to its integer equivalent representation
         *
         * @param currentTextureImage the CurrentTextureImage enum to convert to an integer
         * @return integer representation of the CurrentTextureImage passed into this function
         */
        constexpr unsigned int GetCurrentTextureImageValue(CurrentTextureImage currentTextureImage)
        {
            return static_cast<unsigned int>(currentTextureImage);
        }
    }
}

#endif //TEXTUREATLASCREATOR_CURRENTTEXTUREIMAGE_H
