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
        class PaintArea;
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

    /**
     * Represents an image. It stores all of scaled images that are the result of zoom operations, and simply forwards
     * arguments and/or creates valid arguments from the function parameters to them. Additionally stores any other relevant
     * details about a texture as a whole, such as texture location and description.
     */

    class Texture
    {
        using TextureImages = std::array<ScaledTexture, NumberZoomElements()>;

        public:

            /**
             * Constructor that initializes all of the required internal variables, for example the scaled versions of
             * the texture associated with this object.
             *
             * @param texturePath location of the texture on the file system to be loaded
             * @param intersectionBorderWidth of the passed texture at the 100% (Normal) zoom level
             * @param selectionBorderWidth of the passed in texture at the 199% (Normal) zoom level
             */
            Texture(const QString &texturePath, unsigned int intersectionBorderWidth, unsigned int selectionBorderWidth);

            /**
             * Add a paint operation done on the specular map for this texture at the given zoom to the history stack.
             *
             * @param zoom at which the paint operation occurred
             * @param paintHistoryCommand history of the paint operation
             */
            void addPaintHistorySpecular(Zoom zoom, PaintFunctions::PaintHistoryCommand *paintHistoryCommand, AccessRestriction::PassKey<GUI::CurrentTexture::PaintArea>);

            /**
             * Add a paint operation done on the diffuse map for this texture at the given zoom to the history stack.
             *
             * @param zoom at which the paint operation occurred
             * @param paintHistoryCommand history of the paint operation
             */
            void addPaintHistoryTexture(Zoom zoom, PaintFunctions::PaintHistoryCommand *paintHistoryCommand, AccessRestriction::PassKey<GUI::CurrentTexture::PaintArea>);

            /**
             * Converts the texture (and as a result all of the scaled textures) to the passed in format.
             *
             * @param newFormat that the texture should now have
             */
            void convertToFormat(QImage::Format newFormat);

            /**
             * Get the texture at the specified zoom.
             *
             * @param zoom that the return texture should be in
             * @return reference to texture resource at the passed in zoo,
             */
            const QImage& getImage(TextureLogic::Zoom zoom) const;

            /**
             * Get the intersection width of the texture at the specified zoom.
             *
             * @param zoom specifies at what zoom the intersection width should be given
             * @return intersection width for the texture at the passed in zoom
             */
            unsigned int getIntersectionBorderWidth(Zoom zoom) const;

            /**
             * Get the selection width of the texture at the specified zoom.
             *
             * @param zoom specifies at what zoom the selection width should be given
             * @return selection width for the texture at the passed in zoom
             */
            unsigned int getSelectedBorderWidth(Zoom zoom) const;

            /**
             * Get the specular map for the texture at the given zoom.
             *
             * @param zoom at which the specular map should be at
             * @return reference to the specular map resource at the given zoom
             */
            QImage& getSpecularTexture(TextureLogic::Zoom zoom, AccessRestriction::PassKey<GUI::CurrentTexture::PaintArea>);

            /**
             * Get the description associated with this texture.
             *
             * @return string of the texture description
             */
            const QString& getTextureDescription() const;

            /**
             * Returns the most recent paint operation for the specular map at the given zoom.
             *
             * @param zoom at which the paint operation was applied
             * @return history of paint operation at the given zoom
             */
            PaintFunctions::PaintHistoryCommand* removeRecentPaintHistorySpecular(Zoom zoom, AccessRestriction::PassKey<GUI::CurrentTexture::PaintArea>);

            /**
             * Returns the most recent paint operation for the diffuse map at the given zoom.
             *
             * @param zoom at which the paint operation was applied
             * @return history of paint operation at the given zoom
             */
            PaintFunctions::PaintHistoryCommand* removeRecentPaintHistoryTexture(Zoom zoom, AccessRestriction::PassKey<GUI::CurrentTexture::PaintArea>);

            /**
             * Updates the texture's intersection width at the given zoom. This will update the intersection width of the
             * texture at the other zooms using the passed in zoom as a reference point (scaling the width at the other zoom
             * as required).
             *
             * @param newWidth that the texture's intersection border width should be
             * @param zoom at which the new width is meant to represent
             */
            void setIntersectionBorderWidth(unsigned int newWidth, Zoom zoom, AccessRestriction::PassKey<TextureBank>);

            /**
             * Updates the texture's selection width at the given zoom. This will update the intersection width of the
             * texture at the other zooms using the passed in zoom as a reference point (scaling the width at the other zoom
             * as required).
             *
             * @param newWidth that the texture's selection border width should be
             * @param zoom at which the new width is meant to represent
             */
            void setSelectionBorderWidth(unsigned int newWidth, Zoom zoom, AccessRestriction::PassKey<TextureBank>);

            /**
             * Updates the texture description to the description that is passed in.
             *
             * @param description the new description the texture should have
             */
            void setTextureDescription(const QString &description, AccessRestriction::PassKey<GUI::TextureInformation::SelectedTextureInformation>);

            /**
             * Updates the location of the texture that this object represents. Called when a modified texture was saved.
             * This does not modify the texture on the file system.
             *
             * @param textureLocation that the texture this object represents now has
             */
            void setTextureLocation(const QString &textureLocation, AccessRestriction::PassKey<GUI::TextureInformation::SelectedTextureInformation>);

            /**
             * Updates the texture name. This does not modify the texture on the file system.
             *
             * @param textureName that the texture should now have.
             */
            void setTextureName(const QString &textureName, AccessRestriction::PassKey<GUI::TextureInformation::SelectedTextureInformation>);

            /**
             * Get the string representation of the format this texture has.
             *
             * @return format of this texture as a string
             */
            const QString& textureFormat() const;

            /**
             * Get the location of this texture on the file system.
             *
             * @return location of the texture this object represents
             */
            const QString& textureLocation() const;

            /**
             * Get the name of this texture.
             *
             * @return name of the texture this object represents
             */
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
