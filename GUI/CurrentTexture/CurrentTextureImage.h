//
// Created by BinyBrion on 2019-09-26.
//

#ifndef TEXTUREATLASCREATOR_CURRENTTEXTUREIMAGE_H
#define TEXTUREATLASCREATOR_CURRENTTEXTUREIMAGE_H

namespace GUI
{
    namespace CurrentTexture
    {
        enum class CurrentTextureImage : unsigned int
        {
            SelectedTexture= 0,
            SpecularTexture = 1,
        };

        constexpr unsigned int GetCurrentTextureImageValue(CurrentTextureImage currentTextureImage)
        {
            return static_cast<unsigned int>(currentTextureImage);
        }
    }
}

#endif //TEXTUREATLASCREATOR_CURRENTTEXTUREIMAGE_H
