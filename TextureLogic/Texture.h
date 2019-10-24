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
    class TextureBank;

    /*
     * Represents an image. It stores all of scaled images that are the result of zoom operations.
     * These functions- there are many- are not complicated. They are either getters or simple setters either to
     * variables stored in this class or in ScaledTexture.
     */

    class Texture
    {
        using TextureImages = std::array<ScaledTexture, NumberZoomElements()>;

        public:
            Texture(const QString &texturePath, unsigned int intersectionBorderWidth, unsigned int selectionBorderWidth);
            void addPaintHistorySpecular(Zoom zoom, PaintFunctions::PaintHistoryCommand *paintHistoryCommand, AccessRestriction::PassKey<GUI::CurrentTexture::RenderArea>);
            void addPaintHistoryTexture(Zoom zoom, PaintFunctions::PaintHistoryCommand *paintHistoryCommand, AccessRestriction::PassKey<GUI::CurrentTexture::RenderArea>);
            void convertToFormat(QImage::Format newFormat);
            const QImage& getImage(TextureLogic::Zoom zoom) const;
            unsigned int getIntersectionBorderWidth(Zoom zoom) const;
            unsigned int getSelectedBorderWidth(Zoom zoom) const;
            QImage& getSpecularTexture(TextureLogic::Zoom zoom, AccessRestriction::PassKey<GUI::CurrentTexture::RenderArea>);
            const QString& getTextureDescription(AccessRestriction::PassKey<GUI::TextureInformation::SelectedTextureInformation>) const;
            PaintFunctions::PaintHistoryCommand* removeRecentPaintHistorySpecular(Zoom zoom, AccessRestriction::PassKey<GUI::CurrentTexture::RenderArea>);
            PaintFunctions::PaintHistoryCommand* removeRecentPaintHistoryTexture(Zoom zoom, AccessRestriction::PassKey<GUI::CurrentTexture::RenderArea>);
            void setIntersectionBorderWidth(unsigned int newWidth, Zoom zoom, AccessRestriction::PassKey<TextureBank>);
            void setSelectionBorderWidth(unsigned int newWidth, Zoom zoom, AccessRestriction::PassKey<TextureBank>);
            void setTextureDescription(const QString &description, AccessRestriction::PassKey<GUI::TextureInformation::SelectedTextureInformation>);
            void setTextureLocation(const QString &textureLocation, AccessRestriction::PassKey<GUI::TextureInformation::SelectedTextureInformation>);
            void setTextureName(const QString &textureName, AccessRestriction::PassKey<GUI::TextureInformation::SelectedTextureInformation>);
            const QString& textureFormat() const;
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
