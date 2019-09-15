//
// Created by BinyBrion on 2019-09-15.
//

#ifndef TEXTUREATLASCREATOR_TEXTUREDRAWINGPOSITION_H
#define TEXTUREATLASCREATOR_TEXTUREDRAWINGPOSITION_H

#include <QPoint>

namespace TextureLogic
{
    class Texture;
}

namespace Atlas
{
    struct TextureDrawingPosition
    {
        const TextureLogic::Texture *texture = nullptr;
        QPoint drawingPosition;
    };
}

#endif //TEXTUREATLASCREATOR_TEXTUREDRAWINGPOSITION_H
