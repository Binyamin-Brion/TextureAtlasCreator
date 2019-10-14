//
// Created by BinyBrion on 2019-09-12.
//

#include "ScaledTexture.h"
#include <limits>

#include <QPainter>

namespace TextureLogic
{
    void ScaledTexture::addPaintHistorySpecular(PaintFunctions::PaintHistoryCommand *paintHistoryCommand)
    {
        specularPaintHistory.push(paintHistoryCommand);
    }

    void ScaledTexture::addPaintHistoryTexture(PaintFunctions::PaintHistoryCommand *paintHistoryCommand)
    {
        texturePaintHistory.push(paintHistoryCommand);
    }

    void ScaledTexture::convertToFormat(QImage::Format newFormat)
    {
        image = image.convertToFormat(newFormat);
    }

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

    PaintFunctions::PaintHistoryCommand *ScaledTexture::removePaintHistorySpecular()
    {
        if(specularPaintHistory.empty())
        {
            return nullptr;
        }

        auto mostRecentHistory = specularPaintHistory.top();

        specularPaintHistory.pop();

        return mostRecentHistory;
    }

    PaintFunctions::PaintHistoryCommand *ScaledTexture::removePaintHistoryTexture()
    {
        if(texturePaintHistory.empty())
        {
            return nullptr;
        }

        auto mostRecentHistory = texturePaintHistory.top();

        texturePaintHistory.pop();

        return mostRecentHistory;
    }
}