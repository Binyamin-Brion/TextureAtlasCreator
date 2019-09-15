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

namespace TextureLogic
{
    class TextureBank
    {
        public:
            void addImage(const QString &textureLocation, AccessRestriction::PassKey<Atlas::TextureAtlas>);
            const std::vector<Texture>& WARN_UNUSED deleteImage(std::vector<Texture>::iterator texture, AccessRestriction::PassKey<Atlas::TextureAtlas>);
            const std::vector<Texture>& WARN_UNUSED getTextures(AccessRestriction::PassKey<Atlas::TextureAtlas>) const;
            void setTextureAtlasInstance(Atlas::TextureAtlas *textureAtlas, AccessRestriction::PassKey<Atlas::TextureAtlas>);
        private:
            Atlas::TextureAtlas *textureAtlas = nullptr;
            std::vector<Texture> textures;

    };
}

#include "Attributes/UndefMacros.h"

#endif //TEXTUREATLAS_TEXTUREBANK_H
