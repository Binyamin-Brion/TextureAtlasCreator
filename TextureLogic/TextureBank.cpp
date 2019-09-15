//
// Created by BinyBrion on 2019-09-12.
//

#include "TextureBank.h"
#include "Atlas/TextureAtlas.h"

namespace TextureLogic
{
    void TextureBank::addImage(const QString &textureLocation, AccessRestriction::PassKey<Atlas::TextureAtlas>)
    {
        textures.emplace_back(textureLocation);

        // Tell the texture atlas to reset its texture references as its references may now be invalid if the
        // textures vector reallocated memory

        textureAtlas->textureLoaded({});
    }

    const std::vector<Texture>& TextureBank::deleteImage(std::vector<Texture>::iterator texture, AccessRestriction::PassKey<Atlas::TextureAtlas>)
    {
        textures.erase(texture);

        return textures;
    }

    const std::vector<Texture>& TextureBank::getTextures(AccessRestriction::PassKey<Atlas::TextureAtlas>) const
    {
        return textures;
    }

    void TextureBank::setTextureAtlasInstance(Atlas::TextureAtlas *textureAtlas, AccessRestriction::PassKey<Atlas::TextureAtlas>)
    {
        this->textureAtlas = textureAtlas;
    }
}