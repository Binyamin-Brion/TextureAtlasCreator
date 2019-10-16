//
// Created by BinyBrion on 2019-09-12.
//

#ifndef TEXTUREATLAS_TEXTURE_H
#define TEXTUREATLAS_TEXTURE_H

#include "TextureLogic/ScaledTexture.h"
#include "TextureLogic/Zoom.h"
#include <array>
#include <stack>

namespace GUI
{
    namespace CurrentTexture
    {
        class RenderArea;
    }

    namespace TextureInformation
    {
        class SelectedTextureInformation;
    }
}


class QPainter;

namespace TextureLogic
{
    class Texture
    {
        using TextureImages = std::array<ScaledTexture, NumberZoomElements()>;

        public:
            explicit Texture(const QString &texturePath);
            void addPaintHistorySpecular(Zoom zoom, PaintFunctions::PaintHistoryCommand *paintHistoryCommand, AccessRestriction::PassKey<GUI::CurrentTexture::RenderArea>);
            void addPaintHistoryTexture(Zoom zoom, PaintFunctions::PaintHistoryCommand *paintHistoryCommand, AccessRestriction::PassKey<GUI::CurrentTexture::RenderArea>);
            void convertToFormat(QImage::Format newFormat);
            const QImage& getImage(TextureLogic::Zoom zoom) const;
            QImage& getSpecularTexture(TextureLogic::Zoom zoom, AccessRestriction::PassKey<GUI::CurrentTexture::RenderArea>);
            const QString& getTextureDescription(AccessRestriction::PassKey<GUI::TextureInformation::SelectedTextureInformation>) const;
            PaintFunctions::PaintHistoryCommand* removeRecentPaintHistorySpecular(Zoom zoom, AccessRestriction::PassKey<GUI::CurrentTexture::RenderArea>);
            PaintFunctions::PaintHistoryCommand* removeRecentPaintHistoryTexture(Zoom zoom, AccessRestriction::PassKey<GUI::CurrentTexture::RenderArea>);
            void setTextureDescription(const QString &description, AccessRestriction::PassKey<GUI::TextureInformation::SelectedTextureInformation>);
            void setTextureLocation(const QString &textureLocation, AccessRestriction::PassKey<GUI::TextureInformation::SelectedTextureInformation>);
            void setTextureName(const QString &textureName, AccessRestriction::PassKey<GUI::TextureInformation::SelectedTextureInformation>);
            const QString& textureFormat() const;
            TextureImages& textureImages();
            const QString& textureLocation() const;
            const QString& textureName() const;

        private:

            QString textureDescription;
            QString _textureFormat;
            QString _textureLocation;
            QString _textureName;

            TextureImages _texture;
    };
}

#endif //TEXTUREATLAS_TEXTURE_H
