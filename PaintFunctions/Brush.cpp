//
// Created by BinyBrion on 2019-09-27.
//

#include "Brush.h"
#include <QPixmap>

namespace PaintFunctions
{
    Brush::Brush()
    {
        for(auto &paintImage : paintImages)
        {
            // Initialize the resource to a known state; the actual size of the paint image will be calculated in setPaintTypeSolid()
            paintImage = QPixmap{QSize{1,1}}.toImage();
        }
    }

    const QImage& Brush::getPaintImage(TextureLogic::Zoom zoom) const
    {
        return paintImages[TextureLogic::GetZoomIndex(zoom)];
    }

    bool Brush::getPaintingSpecularTexture() const
    {
        return paintingSpecularTexture;
    }

    void Brush::setPaintingSpecularTexture(bool value)
    {
        paintingSpecularTexture = value;
    }

    void Brush::setPaintTypeSolid(TextureLogic::Zoom zoom, QSize drawAreaSize, QColor colour)
    {
        // When a brush size is selected, it is done with respective to a given zoom. The brush size changes
        // depending on the zoom of the currently selected texture. This is calculated ahead of time for convenience.
          for(auto currentZoom : TextureLogic::AllZoomValues)
          {
              float zoomFactor = TextureLogic::GetZoomValue(currentZoom) / TextureLogic::GetZoomValue(zoom);

              paintImages[TextureLogic::GetZoomIndex(currentZoom)] = paintImages[TextureLogic::GetZoomIndex(zoom)].scaled(drawAreaSize * zoomFactor);
              paintImages[TextureLogic::GetZoomIndex(currentZoom)].fill(colour);
          }
    }
}