//
// Created by BinyBrion on 2019-09-29.
//

#ifndef TEXTUREATLASCREATOR_PAINTHISTORYCOMMAND_H
#define TEXTUREATLASCREATOR_PAINTHISTORYCOMMAND_H

#include <vector>
#include "PaintedArea.h"
#include <QPixmap>
#include <stack>

namespace PaintFunctions
{
    /**
     * Stores all of the paints that were made with a brush. It is simply a style wrapper
     * over a stack, which is all it contains.
     */

    class PaintHistoryCommand
    {
        public:

            /**
             * Initializes the history with the stack passed in. This operation consumes the passed in stack, leaving it empty.
             *
             * @param appliedBrushAreas history of paint operations performed
             */
            explicit PaintHistoryCommand(std::stack<PaintedArea> &appliedBrushAreas);

            /**
             * Get the stack of the paint history.
             *
             * @return mutable reference to the stack containing the history of paint operations
             */
            std::stack<PaintedArea>& getAppliedAreas();

        private:
            std::stack<PaintedArea> appliedBrushAreas;
    };
}

#endif //TEXTUREATLASCREATOR_PAINTHISTORYCOMMAND_H
