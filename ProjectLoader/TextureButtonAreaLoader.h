//
// Created by BinyBrion on 2020-03-09.
//

#ifndef TEXTUREATLASCREATOR_TEXTUREBUTTONAREALOADER_H
#define TEXTUREATLASCREATOR_TEXTUREBUTTONAREALOADER_H

#include <QString>

namespace ProjectLoader
{
    struct TextureData
    {
        QString textureLocation;
        unsigned int intersectionWidth;
        unsigned int selectionWidth;
    };

    struct TextureButtonAreaLoader
    {
        QString areaName;
        std::vector<TextureData> textures;
    };
}

#endif //TEXTUREATLASCREATOR_TEXTUREBUTTONAREALOADER_H
