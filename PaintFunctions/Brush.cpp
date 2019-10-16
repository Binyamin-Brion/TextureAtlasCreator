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
            paintImage = QPixmap{QSize{static_cast<int>(exp2f(TextureLogic::NumberZoomElements())), static_cast<int>(exp2f(TextureLogic::NumberZoomElements()))}}.toImage();
        }
    }

    const QImage& Brush::getPaintImage(TextureLogic::Zoom zoom) const
    {
        return paintImages[TextureLogic::GetZoomIndex(zoom)];
    }

    void Brush::setPaintTypeSolid(TextureLogic::Zoom zoom, QSize drawAreaSize, QColor colour)
    {
          for(auto currentZoom : TextureLogic::AllZoomValues)
          {
              float zoomFactor = TextureLogic::GetZoomValue(currentZoom) / TextureLogic::GetZoomValue(zoom);

              paintImages[TextureLogic::GetZoomIndex(currentZoom)] = paintImages[TextureLogic::GetZoomIndex(zoom)].scaled(drawAreaSize * zoomFactor);
              paintImages[TextureLogic::GetZoomIndex(currentZoom)].fill(colour);
          }
    }
}