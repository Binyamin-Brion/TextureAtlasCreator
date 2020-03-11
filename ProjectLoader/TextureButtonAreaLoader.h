//
// Created by BinyBrion on 2020-03-09.
//

#ifndef TEXTUREATLASCREATOR_TEXTUREBUTTONAREALOADER_H
#define TEXTUREATLASCREATOR_TEXTUREBUTTONAREALOADER_H

#include <QString>

namespace ProjectLoader
{
    /**
     *  Combines the information required to load all of the textures stored in a texture button area.
     */
    struct TextureData
    {
        QString textureLocation;
        unsigned int intersectionWidth;
        unsigned int selectionWidth;
    };

    /**
     *  Combines the information required to represent a tab in the loaded textures widget.
     */
    struct TextureButtonAreaLoader
    {
        QString areaName;
        std::vector<TextureData> textures;
    };
}

#endif //TEXTUREATLASCREATOR_TEXTUREBUTTONAREALOADER_H
