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
    struct TexturePos
    {
        QString textureLocation;
        QPoint position;
    };

    struct TextureAtlasLoader
    {
        QString atlasName;
        QSize atlasSize;
        QString format;
        std::vector<TexturePos> textures;
    };
}

#endif //TEXTUREATLASCREATOR_TEXTUREATLASLOADER_H
