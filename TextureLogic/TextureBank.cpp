//
// Created by BinyBrion on 2019-09-12.
//

#include "TextureBank.h"

#include "GUI/Atlas/AtlasTabWidget.h"
#include "GUI/CurrentTexture/CurrentTextureTabWidget.h"

namespace TextureLogic
{

    const std::vector<Texture>& TextureBank::deleteImage(std::vector<Texture>::iterator texture, AccessRestriction::PassKey<GUI::Atlas::AtlasTabWidget>)
    {
        textures.erase(texture);

        return textures;
    }

    const std::vector<Texture>& TextureBank::getTextures(AccessRestriction::PassKey<GUI::Atlas::AtlasTabWidget>) const
    {
        return textures;
    }

    void TextureBank::setAtlasTabWidgetReference(GUI::Atlas::AtlasTabWidget *atlasTabWidget)
    {
        if(this->atlasTabWidget == nullptr)
        {
            this->atlasTabWidget = atlasTabWidget;
        }
    }

    void TextureBank::setCurrentTextureTabWidgetReference(GUI::CurrentTexture::CurrentTextureTabWidget *currentTextureTabWidget)
    {
        if(this->currentTextureTabWidget == nullptr)
        {
            this->currentTextureTabWidget = currentTextureTabWidget;
        }
    }

    void TextureBank::storeNewTexture(const QString &textureLocation, AccessRestriction::PassKey<GUI::LoadResults::TextureButtonArea>)
    {
        for(const auto &i : textures)
        {
            if(i.textureLocation() == textureLocation)
            {
                return;
            }
        }

        textures.emplace_back(textureLocation);

        // Tell the texture atlas to reset its texture references as its references may now be invalid if the
        // textures vector reallocated memory

        atlasTabWidget->updateTextureReferences({});

        currentTextureTabWidget->setTexturesReference(textures);
    }

    void TextureBank::textureButtonPressed(const QString &textureLocation, AccessRestriction::PassKey<GUI::LoadResults::TextureButtonArea>)
    {
        bool foundTexture = false;

        for(const auto &i : textures)
        {
            if(i.textureLocation() == textureLocation)
            {
                foundTexture = true;

                atlasTabWidget->addTextureToCurrentAtlas(i);

                break;
            }
        }

        if(!foundTexture)
        {
            Q_ASSERT_X(false, __PRETTY_FUNCTION__, "Fatal internal error: requested texture has not been loaded into the texture bank first!");
        }
    }

    void TextureBank::textureSelected(const Texture *texture)
    {
        currentTextureTabWidget->setSelectedTexture(const_cast<Texture*>(texture), {});
    }
}