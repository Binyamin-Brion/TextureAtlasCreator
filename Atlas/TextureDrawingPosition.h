//
// Created by BinyBrion on 2019-09-15.
//

#ifndef TEXTUREATLASCREATOR_TEXTUREDRAWINGPOSITION_H
#define TEXTUREATLASCREATOR_TEXTUREDRAWINGPOSITION_H

#include <QPoint>

#include "Atlas/TextureBorder/SurroundingBorder.h"
#include "TextureLogic/Zoom.h"

namespace TextureLogic
{
    class Texture;
}

namespace Atlas
{
    /*
     * Stores information needed to draw a texture, as well as to update the texture reference stored if the old
     * reference becomes invalid.
     */

    struct TextureDrawingPosition
    {
        const TextureLogic::Texture *texture = nullptr;
        int index = -1;
        QPointF drawingPosition;
        std::array<TextureBorder::SurroundingBorder, ::TextureLogic::NumberZoomElements()> surroundingBorder;

    };
}

#endif //TEXTUREATLASCREATOR_TEXTUREDRAWINGPOSITION_H
