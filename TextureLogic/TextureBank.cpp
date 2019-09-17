//
// Created by BinyBrion on 2019-09-12.
//

#include "TextureBank.h"

#include "GUI/Atlas/AtlasTabWidget.h"

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
        if(this->atlasTabWidget != nullptr)
        {
            return;
        }

        this->atlasTabWidget = atlasTabWidget;
    }

    void TextureBank::storeNewTexture(const QString &textureLocation, AccessRestriction::PassKey<GUI::LoadResults::TextureButtonArea>)
    {
        textures.emplace_back(textureLocation);

        // Tell the texture atlas to reset its texture references as its references may now be invalid if the
        // textures vector reallocated memory

        atlasTabWidget->updateTextureReferences({});
    }

    void TextureBank::textureButtonPressed(const QString &textureLocation, AccessRestriction::PassKey<GUI::LoadResults::TextureButtonArea>)
    {
        bool foundTexture = false;

        for(const auto &i : textures)
        {
            if(i.textureLocation() == textureLocation)
            {
                foundTexture = true;

                break;
            }
        }

        if(!foundTexture)
        {
            Q_ASSERT_X(false, __PRETTY_FUNCTION__, "Fatal internal error: requested texture has not been loaded into the texture bank first!");
        }
    }
}