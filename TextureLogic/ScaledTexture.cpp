//
// Created by BinyBrion on 2019-09-12.
//

#include "ScaledTexture.h"
#include <limits>

namespace TextureLogic
{
    bool ScaledTexture::checkIntersection(const TextureLogic::ScaledTexture &scaledTexture)
    {
        return border.checkIntersection(scaledTexture.border);
    }

    void ScaledTexture::initialize(const QString &textureLocation, TextureLogic::Zoom zoom)
    {
        if(!image.load(textureLocation))
        {
            throw std::runtime_error{"Unable to load texture: " + textureLocation.toStdString()};
        }

        if(image.width() < 0 || image.height() < 0)
        {
            throw std::runtime_error{"Image dimensions are negative!"};
        }

        auto zoomFactorValue = GetZoomValue(zoom);

        auto imageWidthUnsigned = static_cast<unsigned int>(image.width());

        auto imageHeightUnsigned = static_cast<unsigned int>(image.height());

        if( (imageWidthUnsigned * zoomFactorValue) > std::numeric_limits<int>::max())
        {
            throw std::runtime_error{"After zoom, width is too large."};
        }

        if( (imageHeightUnsigned * zoomFactorValue) > std::numeric_limits<int>::max())
        {
            throw std::runtime_error{"After zoom, height is too large."};
        }

        image = image.scaled(image.width() * zoomFactorValue, image.height() * zoomFactorValue, Qt::KeepAspectRatio);

        border.initialize(image.width(), image.height());
    }
}