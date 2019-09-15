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
            void addImage(const QString &textureLocation);
            const std::vector<Texture>& WARN_UNUSED deleteImage(std::vector<Texture>::iterator texture, AccessRestriction::PassKey<Atlas::TextureAtlas>);
            const std::vector<Texture>& WARN_UNUSED getTextures(AccessRestriction::PassKey<Atlas::TextureAtlas>) const;

        private:
            std::vector<Texture> textures;

    };
}

#include "Attributes/UndefMacros.h"

#endif //TEXTUREATLAS_TEXTUREBANK_H
