//
// Created by BinyBrion on 2019-09-29.
//

#include "PaintFunctions/PaintHistoryCommand.h"

namespace PaintFunctions
{
    PaintHistoryCommand::PaintHistoryCommand(std::stack<PaintedArea> &appliedBrushAreas)
    {
        this->appliedBrushAreas = std::move(appliedBrushAreas);

        // To ensure that the passed in stack remains in a known state, assign it to an empty stack
        // Remember that move semantics leave object in valid, but unspecified state
        appliedBrushAreas = std::stack<PaintedArea>{};
    }

    std::stack<PaintedArea>& PaintHistoryCommand::getAppliedAreas()
    {
        return appliedBrushAreas;
    }
}