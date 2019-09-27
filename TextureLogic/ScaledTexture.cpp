//
// Created by BinyBrion on 2019-09-12.
//

#include "ScaledTexture.h"
#include <limits>

#include <QPainter>

namespace TextureLogic
{

    const QImage& ScaledTexture::getImage() const
    {
        return image;
    }

    QImage& ScaledTexture::getSpecularTexture()
    {
        return specularTexture;
    }

    void ScaledTexture::initialize(const QString &textureLocation, TextureLogic::Zoom zoom)
    {
        if(!image.load(textureLocation))
        {
            throw std::runtime_error{"Unable to load texture: " + textureLocation.toStdString()};
        }

        auto zoomFactorValue = GetZoomValue(zoom);

        int newImageWidth = image.width() * zoomFactorValue;

        int newImageHeight = image.height() * zoomFactorValue;

        image = image.scaled(newImageWidth, newImageHeight, Qt::KeepAspectRatio);

        specularTexture = QImage{newImageWidth, newImageHeight, image.format()};
        specularTexture.fill(QColor{0, 0, 0});
    }
}