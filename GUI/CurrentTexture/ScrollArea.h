//
// Created by BinyBrion on 10/2/2019.
//

#ifndef TEXTUREATLASCREATOR_SCROLLAREA_H
#define TEXTUREATLASCREATOR_SCROLLAREA_H

#include <QtWidgets/QScrollArea>
#include "CurrentTextureImage.h"
#include "TextureLogic/Zoom.h"

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
                void enterEvent(QEvent *event) override;
                const PaintFunctions::Brush& getBrush() const;
                QImage::Format getCurrentTextureFormat() const;
                TextureLogic::Zoom getZoom() const;
                void keyPressEvent(QKeyEvent *event) override;
                void keyReleaseEvent(QKeyEvent *event) override;
                void leaveEvent(QEvent *event) override;
                void setTexture(TextureLogic::Texture *texture);
                void wheelEvent(QWheelEvent *event) override;

            signals:
                void paintedSelectedTexture();
                void repaintSelectedTexture();
                void zoomChanged(TextureLogic::Zoom);

            private:
                RenderArea *renderArea = nullptr;
                bool controlKeyDown = false;
        };
    }
}

#endif //TEXTUREATLASCREATOR_SCROLLAREA_H
