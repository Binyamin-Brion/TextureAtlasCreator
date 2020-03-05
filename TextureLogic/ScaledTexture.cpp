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

    unsigned int ScaledTexture::getIntersectionBorderWidth() const
    {
        return intersectionBorderWidth;
    }

    unsigned int ScaledTexture::getSelectionBorderWidth() const
    {
        return selectionBorderWidth;
    }

    QImage& ScaledTexture::getSpecularTexture()
    {
        return specularTexture;
    }

    void ScaledTexture::initialize(const QString &textureLocation, TextureLogic::Zoom zoom, unsigned int intersectionBorderWidth, unsigned int selectionBorderWidth)
    {
        // See Texture.cpp for why there is no constructor in this class.

        if(!image.load(textureLocation))
        {
            throw std::runtime_error{"Unable to load texture: " + textureLocation.toStdString()};
        }

        // The scaling happens using the original texture dimensions and the required zoom this object instance represents,
        // as passing the scaled dimensions directly to this function would increase chances for mistakes to happen
        auto zoomFactorValue = GetZoomValue(zoom);

        int newImageWidth = image.width() * zoomFactorValue;

        int newImageHeight = image.height() * zoomFactorValue;

        image = image.scaled(newImageWidth, newImageHeight, Qt::KeepAspectRatio);

        specularTexture = QImage{newImageWidth, newImageHeight, image.format()};

        // By default, the specular texture is completely black as there are no reflections in diffuse map unless otherwise specified
        specularTexture.fill(QColor{0, 0, 0});

        this->intersectionBorderWidth = intersectionBorderWidth * zoomFactorValue;

        this->selectionBorderWidth = selectionBorderWidth * zoomFactorValue;
    }

    PaintFunctions::PaintHistoryCommand *ScaledTexture::removePaintHistorySpecular()
    {
        if(specularPaintHistory.empty())
        {
            return nullptr;
        }

        // Temporary variable is needed to store top element as top() does not remove element from container, and pop()
        // does not return the last variable. To ensure top element is removed and can be returned from this function,
        // this temporary variable is needed.
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

        // Same logic for removePaintHistorySpecular() for as to why this temporary variable is needed
        auto mostRecentHistory = texturePaintHistory.top();

        texturePaintHistory.pop();

        return mostRecentHistory;
    }

    void ScaledTexture::setIntersectionWidth(unsigned int borderWidth)
    {
        intersectionBorderWidth = borderWidth;
    }

    void ScaledTexture::setSelectionBorderWidth(unsigned int borderWidth)
    {
        selectionBorderWidth = borderWidth;
    }
}