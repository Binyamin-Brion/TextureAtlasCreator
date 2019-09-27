//
// Created by BinyBrion on 2019-09-12.
//

#ifndef TEXTUREATLAS_TEXTUREBANK_H
#define TEXTUREATLAS_TEXTUREBANK_H

#include "TextureLogic/Texture.h"

#include "Attributes/AttributeMacros.h"
#include "AccessRestriction/PassKey.h"


namespace GUI
{
    namespace Atlas
    {
        class AtlasTabWidget;
    }

    namespace CurrentTexture
    {
        class CurrentTextureTabWidget;
    }

    namespace LoadResults
    {
        class TextureButtonArea;
    }
}

namespace TextureLogic
{
    class TextureBank
    {
        public:
           
            const std::vector<Texture>& WARN_UNUSED deleteImage(std::vector<Texture>::iterator texture, AccessRestriction::PassKey<GUI::Atlas::AtlasTabWidget>);
            const std::vector<Texture>& WARN_UNUSED getTextures(AccessRestriction::PassKey<GUI::Atlas::AtlasTabWidget>) const;
            void setAtlasTabWidgetReference(GUI::Atlas::AtlasTabWidget *atlasTabWidget);
            void setCurrentTextureTabWidgetReference(GUI::CurrentTexture::CurrentTextureTabWidget *currentTextureTabWidget);
            void storeNewTexture(const QString &textureLocation, AccessRestriction::PassKey<GUI::LoadResults::TextureButtonArea>);
            void textureButtonPressed(const QString &textureLocation, AccessRestriction::PassKey<GUI::LoadResults::TextureButtonArea>);
            void textureSelected(const Texture *texture);

        private:
            GUI::Atlas::AtlasTabWidget *atlasTabWidget = nullptr;
            GUI::CurrentTexture::CurrentTextureTabWidget *currentTextureTabWidget = nullptr;
            std::vector<Texture> textures;

    };
}

#include "Attributes/UndefMacros.h"

#endif //TEXTUREATLAS_TEXTUREBANK_H
