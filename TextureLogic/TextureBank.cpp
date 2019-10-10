//
// Created by BinyBrion on 2019-09-12.
//

#include "TextureBank.h"

#include "GUI/Atlas/AtlasTabWidget.h"
#include "GUI/CurrentTexture/CurrentTextureTabWidget.h"
#include "GUI/TextureInformation/TextureInfoScrollArea.h"

namespace TextureLogic
{
    const std::vector<Texture>& TextureBank::getTextures(AccessRestriction::PassKey<GUI::Atlas::AtlasTabWidget>) const
    {
        return textures;
    }

    const std::vector<Texture>& TextureBank::getTexturesTextureInfo(AccessRestriction::PassKey<GUI::TextureInformation::TextureInfoScrollArea>)
    {
    return textures;
    }

    void TextureBank::removeTexture(const QString &textureLocation)
    {
        auto texturePosition = std::find_if(textures.begin(), textures.end(), [&textureLocation](const Texture &texture)
        {
             return textureLocation == texture.textureLocation();
        });

        if(texturePosition == textures.end())
        {
            QString errorMessage{"Unable to find requested texture to be deleted: " + textureLocation};

            Q_ASSERT_X(false, __PRETTY_FUNCTION__, errorMessage.toStdString().c_str());
        }

        textureSelected(nullptr);

        atlasTabWidget->removeTexture(&*texturePosition);

        freeSpotIndexes.push_back(std::distance(textures.begin(), texturePosition));
    }

    void TextureBank::selectedTextureChanged()
    {
        textureInfoScrollArea->selectedTextureModified();
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

    void TextureBank::setTextureInfoScrollAreaReference(GUI::TextureInformation::TextureInfoScrollArea *textureInfoScrollArea)
    {
        if(this->textureInfoScrollArea == nullptr)
        {
            this->textureInfoScrollArea = textureInfoScrollArea;
        }
    }

    void TextureBank::storeNewTexture(const QString &textureLocation, AccessRestriction::PassKey<GUI::LoadResults::TextureButtonArea>)
    {
        loadNewTexture(textureLocation);
    }

    void TextureBank::reuploadTexture(const QString &textureLocation, AccessRestriction::PassKey<GUI::TextureInformation::TextureInfoScrollArea>)
    {
        loadNewTexture(textureLocation);
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

        textureInfoScrollArea->setTexture(texture, {});
    }

    void TextureBank::loadNewTexture(const QString &textureLocation)
    {
        unsigned int loopCount = 0;

        for(const auto &i : textures)
        {
            if(std::find(freeSpotIndexes.begin(), freeSpotIndexes.end(), loopCount) != freeSpotIndexes.end())
            {
                continue;
            }

            if(i.textureLocation() == textureLocation)
            {
                return;
            }

            loopCount += 1;
        }

        if(freeSpotIndexes.empty())
        {
            textures.emplace_back(textureLocation);
        }
        else
        {
            textures[freeSpotIndexes.front()] = Texture{textureLocation};

            freeSpotIndexes.erase(freeSpotIndexes.begin());
        }

        resetTextureReference();
    }

    void TextureBank::resetTextureReference()
    {
        // Tell the texture atlas to reset its texture references as its references may now be invalid if the
        // textures vector reallocated memory

        atlasTabWidget->updateTextureReferences({});

        currentTextureTabWidget->setTexturesReference(textures);
    }
}
