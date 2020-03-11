//
// Created by BinyBrion on 2020-03-09.
//

#ifndef TEXTUREATLASCREATOR_TEXTUREATLASLOADER_H
#define TEXTUREATLASCREATOR_TEXTUREATLASLOADER_H

#include <QtCore/QPoint>
#include <QString>
#include <QSize>

namespace ProjectLoader
{
    /**
     *  Information required for the placement of a texture within an atlas.
     */
    struct TexturePos
    {
        QString textureLocation;
        QPoint position;
    };

    /**
     *  Combines all of the information required to load an atlas.
     */
    struct TextureAtlasLoader
    {
        QString atlasName;
        QSize atlasSize;
        QString format;
        std::vector<TexturePos> textures;
    };
}

#endif //TEXTUREATLASCREATOR_TEXTUREATLASLOADER_H
