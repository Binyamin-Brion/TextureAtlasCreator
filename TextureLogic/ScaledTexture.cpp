//
// Created by BinyBrion on 2019-09-12.
//

#include "ScaledTexture.h"
#include <limits>

#include <QPainter>

namespace TextureLogic
{
    bool ScaledTexture::checkIntersection(const TextureLogic::ScaledTexture &scaledTexture)
    {
        return border.checkIntersection(scaledTexture.border);
    }

    const QImage& ScaledTexture::getImage() const
    {
        return image;
    }

    void ScaledTexture::initialize(const QString &textureLocation, TextureLogic::Zoom zoom)
    {
        if(!image.load(textureLocation))
        {
            throw std::runtime_error{"Unable to load texture: " + textureLocation.toStdString()};
        }

        auto zoomFactorValue = GetZoomValue(zoom);

        int newImageWidth = image.width() * zoomFactorValue;

        auto newImageHeight = image.height() * zoomFactorValue;

        image = image.scaled(newImageWidth, newImageHeight, Qt::KeepAspectRatio);

        border.initialize(newImageWidth, newImageHeight);
    }
}