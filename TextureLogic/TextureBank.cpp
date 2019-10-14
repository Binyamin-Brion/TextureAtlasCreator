//
// Created by BinyBrion on 2019-09-12.
//

#include "TextureBank.h"

#include "GUI/Atlas/AtlasTabWidget.h"
#include "GUI/CurrentTexture/CurrentTextureTabWidget.h"
#include "GUI/TextureInformation/TextureInfoScrollArea.h"

namespace TextureLogic
{
    TextureBank::TextureBank()
    {
        internalFormats = GUI::TextureHelperFunctions::internalFormatPairRepresentations();

        internalFormats.erase(internalFormats.begin());

        for(const auto &i : internalFormats)
        {
            textures.push_back(std::make_pair<std::vector<Texture>, std::vector<unsigned int>>({}, {}));
        }
    }

    const std::vector<std::pair<std::vector<Texture>, std::vector<unsigned int>>>& TextureBank::getTextures(AccessRestriction::PassKey<GUI::Atlas::AtlasTabWidget>) const
    {
        return textures;
    }

    const std::vector<std::pair<std::vector<Texture>, std::vector<unsigned int>>>& TextureBank::getTexturesTextureInfo(AccessRestriction::PassKey<GUI::TextureInformation::TextureInfoScrollArea>)
    {
    return textures;
    }

    void TextureBank::removeTexture(const QString &textureLocation)
    {
        if(originalTextureUploadLocation.find(textureLocation.toStdString()) == originalTextureUploadLocation.end())
        {
            QString errorMessage{"Unable to find requested texture to be deleted: " + textureLocation};

            Q_ASSERT_X(false, __PRETTY_FUNCTION__, errorMessage.toStdString().c_str());
        }

        textureSelected(nullptr);

        unsigned int formatIndex = GUI::TextureHelperFunctions::indexFormat(atlasTabWidget->getCurrentAtlasFormat(), true);

        for(auto i = textures.cbegin(); i != textures.cend(); ++i)
        {
            for(auto j = i->first.cbegin(); j != i->first.cend(); ++j)
            {
                if(j->textureLocation() == textureLocation)
                {
                    atlasTabWidget->removeTexture(&*j);

                    textures[formatIndex].second.push_back(std::distance(i->first.cbegin(), j));
                }
            }
        }
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
        if(originalTextureUploadLocation.find(textureLocation.toStdString()) == originalTextureUploadLocation.end())
        {
            Q_ASSERT_X(false, __PRETTY_FUNCTION__, "Fatal internal error: requested texture has not been loaded into the texture bank first!");
        }

        unsigned formatIndex = GUI::TextureHelperFunctions::indexFormat(atlasTabWidget->getCurrentAtlasFormat(), true);

        bool foundTexture = false;

        for(const auto &i : textures[formatIndex].first)
        {
            if(i.textureLocation() == textureLocation)
            {
                atlasTabWidget->addTextureToCurrentAtlas(i);

                foundTexture = true;

                break;
            }
        }

        if(!foundTexture)
        {
            if(textures[formatIndex].second.empty())
            {
                textures[formatIndex].first.push_back(Texture{textureLocation});

                textures[formatIndex].first.back().convertToFormat(atlasTabWidget->getCurrentAtlasFormat());

                resetTextureReference();

                atlasTabWidget->addTextureToCurrentAtlas(textures[formatIndex].first.back());
            }
            else
            {
                textures[formatIndex].first[textures[formatIndex].second.front()] = Texture{textureLocation};

                textures[formatIndex].first[textures[formatIndex].second.front()].convertToFormat(atlasTabWidget->getCurrentAtlasFormat());

                textures[formatIndex].second.erase(textures[formatIndex].second.begin());

                resetTextureReference();

                atlasTabWidget->addTextureToCurrentAtlas(textures[formatIndex].first[textures[formatIndex].second.front()]);
            }
        }
    }

    void TextureBank::textureSelected(const Texture *texture)
    {
        currentTextureTabWidget->setSelectedTexture(const_cast<Texture*>(texture), {});

        textureInfoScrollArea->setTexture(texture, {});
    }

    void TextureBank::loadNewTexture(const QString &textureLocation)
    {
        if(originalTextureUploadLocation.find(textureLocation.toStdString()) != originalTextureUploadLocation.end())
        {
            return;
        }

        unsigned int RGB32_Index = GUI::TextureHelperFunctions::indexFormat(QImage::Format_RGB32, true);

        originalTextureUploadLocation.insert(std::make_pair(textureLocation.toStdString(), RGB32_Index));

        if(textures[RGB32_Index].second.empty())
        {
            textures[RGB32_Index].first.push_back(Texture{textureLocation});

            QImage::Format loadedTextureFormat = textures[RGB32_Index].first.back().getImage(Zoom::Normal).format();

            if(loadedTextureFormat != QImage::Format_RGB32)
            {
                unsigned int newTextureVectorIndex = GUI::TextureHelperFunctions::indexFormat(loadedTextureFormat, true);

                if(textures[newTextureVectorIndex].second.empty())
                {
                    textures[newTextureVectorIndex].first.push_back(Texture{textureLocation});

                    textures[RGB32_Index].first.pop_back();
                }
                else
                {
                    textures[newTextureVectorIndex].first[textures[newTextureVectorIndex].second.front()] = Texture{textureLocation};

                    textures[newTextureVectorIndex].second.erase(textures[newTextureVectorIndex].second.begin());
                }

                originalTextureUploadLocation.erase(textureLocation.toStdString());

                originalTextureUploadLocation.insert(std::make_pair(textureLocation.toStdString(), newTextureVectorIndex));
            }
        }
        else
        {
            textures[RGB32_Index].first[textures[RGB32_Index].second.front()] = Texture{textureLocation};

            QImage::Format loadedTextureFormat = textures[RGB32_Index].first[textures[RGB32_Index].second.front()].getImage(Zoom::Normal).format();

            if(loadedTextureFormat != QImage::Format_RGB32)
            {
                unsigned int newTextureVectorIndex = GUI::TextureHelperFunctions::indexFormat(loadedTextureFormat, true);

                if(textures[newTextureVectorIndex].second.empty())
                {
                    textures[newTextureVectorIndex].first.push_back(Texture{textureLocation});
                }
                else
                {
                    textures[newTextureVectorIndex].first[textures[newTextureVectorIndex].second.front()] = Texture{textureLocation};

                    textures[newTextureVectorIndex].second.erase(textures[newTextureVectorIndex].second.begin());
                }

                originalTextureUploadLocation.erase(textureLocation.toStdString());

                originalTextureUploadLocation.insert(std::make_pair(textureLocation.toStdString(), newTextureVectorIndex));
            }
            else
            {
                textures[RGB32_Index].second.erase(textures[RGB32_Index].second.begin());
            }
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
