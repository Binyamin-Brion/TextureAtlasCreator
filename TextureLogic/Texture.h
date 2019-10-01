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
}


class QPainter;

namespace TextureLogic
{
    class Texture
    {
        using TextureImages = std::array<ScaledTexture, NumberZoomElements()>;

        public:
            explicit Texture(const QString &texturePath);
            void addPaintHistorySpecular(PaintFunctions::PaintHistoryCommand *paintHistoryCommand, AccessRestriction::PassKey<GUI::CurrentTexture::RenderArea>);
            void addPaintHistoryTexture(PaintFunctions::PaintHistoryCommand *paintHistoryCommand, AccessRestriction::PassKey<GUI::CurrentTexture::RenderArea>);
            const QImage& getImage(TextureLogic::Zoom zoom) const;
            QImage& getSpecularTexture(TextureLogic::Zoom zoom, AccessRestriction::PassKey<GUI::CurrentTexture::RenderArea>);
            PaintFunctions::PaintHistoryCommand* removeRecentPaintHistorySpecular(AccessRestriction::PassKey<GUI::CurrentTexture::RenderArea>);
            PaintFunctions::PaintHistoryCommand* removeRecentPaintHistoryTexture(AccessRestriction::PassKey<GUI::CurrentTexture::RenderArea>);
            TextureImages& textureImages();
            const QString& textureLocation() const;
            const QString& textureName() const;

        private:

            QString _textureLocation;
            QString _textureName;

            TextureImages _texture;
    };
}

#endif //TEXTUREATLAS_TEXTURE_H
