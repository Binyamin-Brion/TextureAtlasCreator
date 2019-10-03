//
// Created by BinyBrion on 10/2/2019.
//

#ifndef TEXTUREATLASCREATOR_SCROLLAREA_H
#define TEXTUREATLASCREATOR_SCROLLAREA_H

#include <QtWidgets/QScrollArea>
#include "CurrentTextureImage.h"

namespace PaintFunctions
{
    class Brush;
}

namespace TextureLogic
{
    class Texture;
}

namespace GUI
{
    namespace CurrentTexture
    {
        class RenderArea;

        class ScrollArea : public QScrollArea
        {
            Q_OBJECT

            public:
                ScrollArea(CurrentTextureImage currentTextureImage, QWidget *parent = nullptr);
                const PaintFunctions::Brush& getBrush() const;
                void setTexture(TextureLogic::Texture *texture);

            signals:
                void repaintSelectedTexture();

            private:
                RenderArea *renderArea = nullptr;
        };
    }
}

#endif //TEXTUREATLASCREATOR_SCROLLAREA_H
