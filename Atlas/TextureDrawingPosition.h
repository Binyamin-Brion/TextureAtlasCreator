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
    struct TextureDrawingPosition
    {
        const TextureLogic::Texture *texture = nullptr;
        int index = -1;
        QPoint drawingPosition;
        std::array<TextureBorder::SurroundingBorder, ::TextureLogic::NumberZoomElements()> surroundingBorder;

    };
}

#endif //TEXTUREATLASCREATOR_TEXTUREDRAWINGPOSITION_H
