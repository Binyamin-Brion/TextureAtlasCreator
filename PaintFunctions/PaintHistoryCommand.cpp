//
// Created by BinyBrion on 2019-09-29.
//

#include "PaintFunctions/PaintHistoryCommand.h"

namespace PaintFunctions
{
    PaintHistoryCommand::PaintHistoryCommand(std::stack<PaintedArea> &appliedBrushAreas)
    {
        this->appliedBrushAreas = std::move(appliedBrushAreas);
    }

    std::stack<PaintedArea>& PaintHistoryCommand::getAppliedAreas()
    {
        return appliedBrushAreas;
    }
}