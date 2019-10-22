//
// Created by BinyBrion on 2019-09-12.
//

#include "TextureBank.h"

#include "GUI/Atlas/AtlasTabWidget.h"
#include "GUI/CurrentTexture/CurrentTextureTabWidget.h"
#include "GUI/TextureInformation/TextureInfoScrollArea.h"

// For every type of format, there are two vectors associated with it: the first vector holds the
// textures of a given format; the other one holds free spots the new textures can be inserted to.

/*
 *  The purpose of the originalTextureUploadLocation is as follows. Each texture has its original format,
 *  such as when it was downloaded from internet. When converting the texture to a different format,
 *  it is converted from this original format. The reason for this is that it is unknown if converting
 *  to certain formats may lose information, resulting in erroneous conversions later if a conversion
 *  is based off of a different converted format.
 *
 *  To deal with this, whenever a new texture is upload, it is stored in the vector representing the
 *  texture's original format. Whenever the texture needs to be converted to a different format, it
 *  is done so from this version of the texture. The variable originalTextureUploadLocation keeps track
 *  of the vector of where this original texture is. The program must however iterate over that texture to
 *  find that texture though- there is no promise in the future that all textures uploaded into a vector will
 *  always have the same index.
 */

namespace TextureLogic
{
    TextureBank::TextureBank()
    {
        // The number of internal formats has a subtraction of 1 as the first internal format specified- Format_Invalid -
        // is not format that when a texture has, the texture of that format is stored. In that case, an error message
        // needs to be shown to the user to indicate that the texture could not be loaded.

        unsigned numberInternalFormats = GUI::TextureHelperFunctions::internalFormatPairRepresentations().size() - 1;

        // The vectors holding the textures of different formats have to be created first- otherwise a crash
        // will occur when trying to access them!

        for(int i = 0; i < numberInternalFormats; ++i)
        {
            textures.push_back(std::make_pair<std::vector<Texture>, std::vector<unsigned int>>({}, {}));
        }
    }

    const std::vector<std::pair<std::vector<Texture>, std::vector<unsigned int>>>& TextureBank::getTextures() const
    {
        return textures;
    }

    const std::vector<std::pair<std::vector<Texture>, std::vector<unsigned int>>>& TextureBank::getTexturesTextureInfo(AccessRestriction::PassKey<GUI::TextureInformation::TextureInfoScrollArea>)
    {
        return textures;
    }

    void TextureBank::removeTexture(const QString &textureLocation)
    {
        auto originalTextureLocation = originalTextureUploadLocation.find(textureLocation.toStdString());

        if(originalTextureLocation == originalTextureUploadLocation.end())
        {
            QString errorMessage{"Unable to find requested texture to be deleted: " + textureLocation};

            Q_ASSERT_X(false, __PRETTY_FUNCTION__, errorMessage.toStdString().c_str());
        }

        // Make sure that any GUI parts referencing the texture about to be deleted no longer reference them.
        // While in the current version of this program the texture is still in the vector, this may not always
        // be the case in the future and logically makes no sense to keep referencing that texture.

        textureSelected(nullptr);

        originalTextureLocation->second.second -= 1;

        if(originalTextureLocation->second.second != 0)
        {
            return;
        }

        // Iterate through all of the textures, including those of different formats, and remove that texture
        // from any of the texture atlases, and mark the texture's place in the vector as free

        for(auto i = textures.begin(); i != textures.end(); ++i)
        {
            for(auto j = i->first.cbegin(); j != i->first.cend(); ++j)
            {
                if(j->textureLocation() == textureLocation)
                {
                    atlasTabWidget->removeTexture(&*j);

                    i->second.push_back(std::distance(i->first.cbegin(), j));
                }
            }
        }
    }

    // This function is called when a selected texture is painted; when that occurs, certain information
    // about the texture that is shown, such as the texture name, is changed

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

    bool TextureBank::setIntersectionBorderWidth(Texture *texture, Zoom zoom, unsigned int newBorderWidth)
    {
       // unsigned int formatIndex = GUI::TextureHelperFunctions::indexFormatString(texture->textureFormat(), true);

        bool foundTexture = false;

        unsigned int previousIntersectionWidth = texture->getIntersectionBorderWidth(Zoom::Normal);

        texture->setIntersectionBorderWidth(newBorderWidth, zoom, {});

        if(atlasTabWidget->setIntersectionWidth(texture))
        {
            texture->setIntersectionBorderWidth(previousIntersectionWidth, Zoom::Normal, {});

            atlasTabWidget->setIntersectionWidth(texture);

            return true;
        }

        return false;
    }

    void TextureBank::setSelectionBorderWidth(TextureLogic::Texture *texture, TextureLogic::Zoom zoom, unsigned int newBorderWidth)
    {
        texture->setSelectionBorderWidth(newBorderWidth, zoom, {});

        atlasTabWidget->setIntersectionWidth(texture);
    }

    void TextureBank::setTextureInfoScrollAreaReference(GUI::TextureInformation::TextureInfoScrollArea *textureInfoScrollArea)
    {
        if(this->textureInfoScrollArea == nullptr)
        {
            this->textureInfoScrollArea = textureInfoScrollArea;
        }
    }

    void TextureBank::storeNewTexture(const QString &textureLocation, unsigned int intersectionBorderWidth, unsigned int selectionBorderWidth, AccessRestriction::PassKey<GUI::LoadResults::TextureButtonArea>)
    {
        loadNewTexture(textureLocation, intersectionBorderWidth, selectionBorderWidth);
    }

    // Note that fn reuploadTexture is called when a texture is saved. See the TextureInfo classes in GUI for more info

    void TextureBank::reuploadTexture(const QString &textureLocation, unsigned int intersectionBorderWidth, unsigned int selectionBorderWidth, AccessRestriction::PassKey<GUI::TextureInformation::TextureInfoScrollArea>)
    {
        loadNewTexture(textureLocation, intersectionBorderWidth, selectionBorderWidth);
    }

    void TextureBank::textureButtonPressed(const QString &textureLocation, unsigned int intersectionBorderWidth, unsigned int selectionBorderWidth, AccessRestriction::PassKey<GUI::LoadResults::TextureButtonArea>)
    {
        if(originalTextureUploadLocation.find(textureLocation.toStdString()) == originalTextureUploadLocation.end())
        {
            Q_ASSERT_X(false, __PRETTY_FUNCTION__, "Fatal internal error: requested texture has not been loaded into the texture bank first!");
        }

        // The format of the texture that is requested is dependent on the format of the texture atlas that is currently
        // being viewed. If the requested texture has already been loaded by the active texture atlas' format, simply
        // give a reference to that loaded texture. Otherwise the original requested texture needs to be converted into
        // the format of the active texture atlas, and then a reference to that newly created texture is given.

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

        // Requested texture with the format of the current atlas has not been found; time to create one.
        // The checks to see if there are free spots are the same in the fn loadNewTexture. See that function
        // for more details.

        if(!foundTexture)
        {
            if(textures[formatIndex].second.empty())
            {
                textures[formatIndex].first.push_back(Texture{textureLocation, intersectionBorderWidth, selectionBorderWidth});

                textures[formatIndex].first.back().convertToFormat(atlasTabWidget->getCurrentAtlasFormat());

                // Remember that the texture vectors has been added to at the back (its size increased).
                // A reseating of references to the vector is needed (see fn resetTextureReference for more details)

                resetTextureReference();

                atlasTabWidget->addTextureToCurrentAtlas(textures[formatIndex].first.back());
            }
            else
            {
                // Note: Since the size of the vector is not changed in this vector, fn resetTextureReference do not need to be called

                textures[formatIndex].first[textures[formatIndex].second.front()] = Texture{textureLocation, intersectionBorderWidth, selectionBorderWidth};

                textures[formatIndex].first[textures[formatIndex].second.front()].convertToFormat(atlasTabWidget->getCurrentAtlasFormat());

                textures[formatIndex].second.erase(textures[formatIndex].second.begin());

                atlasTabWidget->addTextureToCurrentAtlas(textures[formatIndex].first[textures[formatIndex].second.front()]);
            }
        }
    }

    void TextureBank::textureSelected(const Texture *texture)
    {
        // Notify all of the objects that change their state if a texture is selected that, well, a new texture has been selected.
        // As of this writing, these are GUI widgets such as a widget that displays the selected texture properites

        currentTextureTabWidget->setSelectedTexture(const_cast<Texture*>(texture), {});

        textureInfoScrollArea->setTexture(texture, {});
    }

    void TextureBank::loadNewTexture(const QString &textureLocation, unsigned int intersectionBorderWidth, unsigned int selectionBorderWidth)
    {
        // Stop a texture from being loaded twice- this wastes resources

        auto originalTextureLocation = originalTextureUploadLocation.find(textureLocation.toStdString());

        if(originalTextureLocation != originalTextureUploadLocation.end())
        {
            originalTextureLocation->second.second += 1;

            return;
        }

        // By default, it is assumed that a texture is of the format RGB32. The texture is loaded into the
        // RGB32 vector initially. When an instance of the texture loaded, it is checked if the texture is
        // actually of the RGB32 format. If it is not, it is removed from the RGB32 vector and placed into
        // the appropriate format vector.

        unsigned int RGB32_Index = GUI::TextureHelperFunctions::indexFormat(QImage::Format_RGB32, true);

        originalTextureUploadLocation.insert(std::make_pair(textureLocation.toStdString(), std::make_pair(RGB32_Index, 1)));

        // If there are no free places to insert a new texture, it is inserted at the back of a format texture

        if(textures[RGB32_Index].second.empty())
        {
            textures[RGB32_Index].first.push_back(Texture{textureLocation, intersectionBorderWidth, selectionBorderWidth});

            QImage::Format loadedTextureFormat = textures[RGB32_Index].first.back().getImage(Zoom::Normal).format();

            if(loadedTextureFormat != QImage::Format_RGB32)  // Check if the loaded texture actually has a initial format of RGB32
            {
                // Query the texture to see what format it actually has, and insert it into the appropriate format vector.
                // Afterwards, either delete the previously loaded texture if it was added at the back of the RGB32 vector,
                // or mark the texture in the RGB32 vector holding the previous loaded texture as free.
                // Note again the process of checking if there is a free spot in the appropriate texture format.

                unsigned int newTextureVectorIndex = GUI::TextureHelperFunctions::indexFormat(loadedTextureFormat, true);

                if(textures[newTextureVectorIndex].second.empty())
                {
                    textures[newTextureVectorIndex].first.push_back(Texture{textureLocation, intersectionBorderWidth, selectionBorderWidth});

                    textures[RGB32_Index].first.pop_back();
                }
                else
                {
                    textures[newTextureVectorIndex].first[textures[newTextureVectorIndex].second.front()] = Texture{textureLocation, intersectionBorderWidth, selectionBorderWidth};

                    textures[newTextureVectorIndex].second.erase(textures[newTextureVectorIndex].second.begin());
                }

                // Update where the original texture is being stored now that it is no longer in the RGB32 format vector

                originalTextureUploadLocation.erase(textureLocation.toStdString());

                originalTextureUploadLocation.insert(std::make_pair(textureLocation.toStdString(), std::make_pair(newTextureVectorIndex, 1)));
            }
        }
        else // Same logic as the above branch; the only difference is that the original texture is not loaded into the back of the RGB32 format vector
        {
            textures[RGB32_Index].first[textures[RGB32_Index].second.front()] = Texture{textureLocation, intersectionBorderWidth, selectionBorderWidth};

            QImage::Format loadedTextureFormat = textures[RGB32_Index].first[textures[RGB32_Index].second.front()].getImage(Zoom::Normal).format();

            if(loadedTextureFormat != QImage::Format_RGB32)
            {
                unsigned int newTextureVectorIndex = GUI::TextureHelperFunctions::indexFormat(loadedTextureFormat, true);

                if(textures[newTextureVectorIndex].second.empty())
                {
                    textures[newTextureVectorIndex].first.push_back(Texture{textureLocation, intersectionBorderWidth, selectionBorderWidth});
                }
                else
                {
                    textures[newTextureVectorIndex].first[textures[newTextureVectorIndex].second.front()] = Texture{textureLocation, intersectionBorderWidth, selectionBorderWidth};

                    textures[newTextureVectorIndex].second.erase(textures[newTextureVectorIndex].second.begin());
                }

                originalTextureUploadLocation.erase(textureLocation.toStdString());

                originalTextureUploadLocation.insert(std::make_pair(textureLocation.toStdString(), std::make_pair(newTextureVectorIndex, 1)));
            }
            else
            {
                textures[RGB32_Index].second.erase(textures[RGB32_Index].second.begin());
            }
        }

        // The vectors have been modified and the resources of the format vector may not be in the sample place in
        // memory as they were before. Any objects referring to those regions of memory have to be updated.

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
