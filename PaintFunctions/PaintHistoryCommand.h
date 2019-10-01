//
// Created by BinyBrion on 2019-09-29.
//

#ifndef TEXTUREATLASCREATOR_PAINTHISTORYCOMMAND_H
#define TEXTUREATLASCREATOR_PAINTHISTORYCOMMAND_H

#include <vector>
#include "GUI/CurrentTexture/CurrentTextureImage.h"
#include "PaintedArea.h"
#include <QPixmap>
#include <stack>

namespace PaintFunctions
{
    class PaintHistoryCommand
    {
        public:
            explicit PaintHistoryCommand(std::stack<PaintedArea> &appliedBrushAreas);
            std::stack<PaintedArea>& getAppliedAreas();

        private:
            std::stack<PaintedArea> appliedBrushAreas;
    };
}

#endif //TEXTUREATLASCREATOR_PAINTHISTORYCOMMAND_H
