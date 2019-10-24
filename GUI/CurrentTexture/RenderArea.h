//
// Created by BinyBrion on 2019-09-26.
//

#ifndef TEXTUREATLASCREATOR_RENDERAREA_H
#define TEXTUREATLASCREATOR_RENDERAREA_H

#include <QtWidgets/QWidget>
#include "CurrentTextureImage.h"
#include "TextureLogic/Zoom.h"
#include "PaintFunctions/Brush.h"
#include "PaintFunctions/PaintedArea.h"
#include <stack>

namespace TextureLogic
{
    class Texture;
}

namespace PaintFunctions
{
    class PaintHistoryCommand;
}

namespace GUI
{
    namespace CurrentTexture
    {
        /*
         *  Provides an area to modify the currently selected texture.
         */

        class RenderArea : public QWidget
        {
            Q_OBJECT

            public:
                explicit RenderArea(CurrentTextureImage currentTextureImage, QWidget *parent = nullptr);
                const PaintFunctions::Brush& getBrush() const;
                QImage::Format getCurrentTextureFormat() const;
                TextureLogic::Zoom getZoom() const;
                void mouseMoveEvent(QMouseEvent *event) override;
                void mousePressEvent(QMouseEvent *event) override;
                void mouseReleaseEvent(QMouseEvent *event) override;
                void paintEvent(QPaintEvent *event) override;
                void setTexture(TextureLogic::Texture *texture);
                void zoomIn();
                void zoomOut();

            signals:
                void paintedSelectedTexture();
                void repaintSelectedTexture();

            private slots:
                void undoPaintOperation();

            private:
                const QImage& getReferredToImage(TextureLogic::Zoom zoom) const;
                PaintFunctions::PaintHistoryCommand* getReferredToImageHistory(TextureLogic::Zoom zoom) const;
                void paintTexture(TextureLogic::Zoom zoom, QPoint mousePosition, const QImage &applyImage, QImage &targetImage, bool undoOperation);
                void storePaintHistory();

                TextureLogic::Texture *texture = nullptr;
                CurrentTextureImage currentTextureImage;
                TextureLogic::Zoom currentZoom;

                PaintFunctions::Brush brush;
                std::array<std::stack<PaintFunctions::PaintedArea>, TextureLogic::NumberZoomElements()> appliedBrushAreas;

                QPoint previousMousePosition;
                bool leftMouseButtonDown = false;

                QImage::Format textureFormat;
        };
    }
}

#endif //TEXTUREATLASCREATOR_RENDERAREA_H
