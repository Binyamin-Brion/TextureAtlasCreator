//
// Created by BinyBrion on 2019-09-12.
//

#ifndef TEXTUREATLAS_TEXTUREBANK_H
#define TEXTUREATLAS_TEXTUREBANK_H

#include "TextureLogic/Texture.h"

#include "AccessRestriction/PassKey.h"
#include "GUI/TextureHelperFunctions/TextureFormats.h"

#include <unordered_map>

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

            TextureBank();
            const std::vector<std::pair<std::vector<Texture>, std::vector<unsigned int>>>& getTextures() const;
            const std::vector<std::pair<std::vector<Texture>, std::vector<unsigned int>>>& getTexturesTextureInfo(AccessRestriction::PassKey<GUI::TextureInformation::TextureInfoScrollArea>);
            void removeTexture(const QString &textureLocation);
            void selectedTextureChanged();
            void setAtlasTabWidgetReference(GUI::Atlas::AtlasTabWidget *atlasTabWidget);
            void setCurrentTextureTabWidgetReference(GUI::CurrentTexture::CurrentTextureTabWidget *currentTextureTabWidget);
            bool setIntersectionBorderWidth(Texture *texture, Zoom zoom, unsigned int newBorderWidth);
            void setSelectionBorderWidth(Texture *texture, Zoom zoom, unsigned int newBorderWidth);
            void setTextureInfoScrollAreaReference(GUI::TextureInformation::TextureInfoScrollArea *textureInfoScrollArea);
            void storeNewTexture(const QString &textureLocation, unsigned int intersectionBorderWidth, unsigned int selectionBorderWidth, AccessRestriction::PassKey<GUI::LoadResults::TextureButtonArea>);
            void reuploadTexture(const QString &textureLocation, unsigned int intersectionBorderWidth, unsigned int selectionBorderWidth, AccessRestriction::PassKey<GUI::TextureInformation::TextureInfoScrollArea>);
            void textureButtonPressed(const QString &textureLocation, unsigned int intersectionBorderWidth, unsigned int selectionBorderWidth, AccessRestriction::PassKey<GUI::LoadResults::TextureButtonArea>);
            void textureSelected(const Texture *texture);

        private:
            void loadNewTexture(const QString &textureLocation, unsigned int intersectionBorderWidth, unsigned int selectionBorderWidth);
            void resetTextureReference();

            GUI::Atlas::AtlasTabWidget *atlasTabWidget = nullptr;
            GUI::CurrentTexture::CurrentTextureTabWidget *currentTextureTabWidget = nullptr;
            GUI::TextureInformation::TextureInfoScrollArea *textureInfoScrollArea = nullptr;

            // See TextureBank.cpp for more details of these two variables
            std::vector<std::pair<std::vector<Texture>, std::vector<unsigned int>>> textures;
            std::unordered_map<std::string, std::pair<unsigned int, unsigned int>> originalTextureUploadLocation;
    };
}

#endif //TEXTUREATLAS_TEXTUREBANK_H
