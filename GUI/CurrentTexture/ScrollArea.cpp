//
// Created by BinyBrion on 10/2/2019.
//

#include <QtWidgets/QHBoxLayout>
#include "ScrollArea.h"
#include "RenderArea.h"

namespace GUI
{
    namespace CurrentTexture
    {
        ScrollArea::ScrollArea(CurrentTextureImage currentTextureImage, QWidget *parent) : QScrollArea{parent}
        {
            setLayout(new QHBoxLayout);

            renderArea = new RenderArea{currentTextureImage, this};

            setWidget(renderArea);

            connect(renderArea, &RenderArea::repaintSelectedTexture, [this]()
            {
                emit repaintSelectedTexture();
            });
        }

        const PaintFunctions::Brush &ScrollArea::getBrush() const
        {
            return renderArea->getBrush();
        }

        void ScrollArea::setTexture(TextureLogic::Texture *texture)
        {
            renderArea->setTexture(texture);

            QWidget::repaint();
        }
    }
}