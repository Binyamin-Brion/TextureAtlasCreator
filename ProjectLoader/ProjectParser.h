//
// Created by BinyBrion on 2020-03-09.
//

#ifndef TEXTUREATLASCREATOR_PROJECTPARSER_H
#define TEXTUREATLASCREATOR_PROJECTPARSER_H

#include "TextureAtlasLoader.h"
#include "TextureButtonAreaLoader.h"

namespace ProjectLoader
{
    class ProjectParser
    {
        public:
            const std::vector<TextureAtlasLoader>& getTextureAtlases();
            const std::vector<TextureButtonAreaLoader>& getTextureButtonAreas();
            bool parseFile(const QString &projectFileLocation);

        private:
            std::vector<TextureAtlasLoader> textureAtlases;
            std::vector<TextureButtonAreaLoader> textureButtonAreas;
    };
}

#endif //TEXTUREATLASCREATOR_PROJECTPARSER_H
