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
            void storeNewTexture(const QString &textureLocation, AccessRestriction::PassKey<GUI::LoadResults::TextureButtonArea>);
            void textureButtonPressed(const QString &textureLocation, AccessRestriction::PassKey<GUI::LoadResults::TextureButtonArea>);

        private:
            GUI::Atlas::AtlasTabWidget *atlasTabWidget = nullptr;
            std::vector<Texture> textures;

    };
}

#include "Attributes/UndefMacros.h"

#endif //TEXTUREATLAS_TEXTUREBANK_H
