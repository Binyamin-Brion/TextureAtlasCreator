//
// Created by BinyBrion on 2019-09-12.
//

#ifndef TEXTUREATLAS_TEXTUREBANK_H
#define TEXTUREATLAS_TEXTUREBANK_H

#include "TextureLogic/Texture.h"

#include "Attributes/AttributeMacros.h"
#include "AccessRestriction/PassKey.h"

namespace Atlas
{
    class TextureAtlas;
}

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

    namespace TextureInformation
    {
        class TextureInfoScrollArea;
    }
}

namespace TextureLogic
{
    class TextureBank
    {
        public:

            const std::vector<Texture>& WARN_UNUSED getTextures(AccessRestriction::PassKey<GUI::Atlas::AtlasTabWidget>) const;
            const std::vector<Texture>& WARN_UNUSED getTexturesTextureInfo(AccessRestriction::PassKey<GUI::TextureInformation::TextureInfoScrollArea>);
            void removeTexture(const QString &textureLocation);
            void selectedTextureChanged();
            void setAtlasTabWidgetReference(GUI::Atlas::AtlasTabWidget *atlasTabWidget);
            void setCurrentTextureTabWidgetReference(GUI::CurrentTexture::CurrentTextureTabWidget *currentTextureTabWidget);
            void setTextureInfoScrollAreaReference(GUI::TextureInformation::TextureInfoScrollArea *textureInfoScrollArea);
            void storeNewTexture(const QString &textureLocation, AccessRestriction::PassKey<GUI::LoadResults::TextureButtonArea>);
            void reuploadTexture(const QString &textureLocation, AccessRestriction::PassKey<GUI::TextureInformation::TextureInfoScrollArea>);
            void textureButtonPressed(const QString &textureLocation, AccessRestriction::PassKey<GUI::LoadResults::TextureButtonArea>);
            void textureSelected(const Texture *texture);

        private:
            void loadNewTexture(const QString &textureLocation);
            void resetTextureReference();

            GUI::Atlas::AtlasTabWidget *atlasTabWidget = nullptr;
            GUI::CurrentTexture::CurrentTextureTabWidget *currentTextureTabWidget = nullptr;
            GUI::TextureInformation::TextureInfoScrollArea *textureInfoScrollArea = nullptr;
            std::vector<Texture> textures;

            std::vector<unsigned int> freeSpotIndexes;
    };
}

#include "Attributes/UndefMacros.h"

#endif //TEXTUREATLAS_TEXTUREBANK_H
