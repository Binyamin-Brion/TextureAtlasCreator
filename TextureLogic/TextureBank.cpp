//
// Created by BinyBrion on 2019-09-12.
//

#include "TextureBank.h"

namespace TextureLogic
{
    void TextureBank::addImage(const QString &textureLocation)
    {
        textures.emplace_back(textureLocation);
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
}