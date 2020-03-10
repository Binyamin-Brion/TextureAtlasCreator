//
// Created by BinyBrion on 2020-03-09.
//

#include <QtCore/QFile>
#include "ProjectParser.h"

namespace ProjectLoader
{
    const std::vector<TextureAtlasLoader> &ProjectParser::getTextureAtlases()
    {
        return textureAtlases;
    }

    const std::vector<TextureButtonAreaLoader> &ProjectParser::getTextureButtonAreas()
    {
        return textureButtonAreas;
    }

    bool ProjectParser::parseFile(const QString &projectFileLocation)
    {
        QFile openProjectFile{projectFileLocation};

        if(!openProjectFile.open(QIODevice::ReadOnly))
        {
            return false;
        }

        QString projectFileContents{openProjectFile.readAll()};

        QStringList splitContents = projectFileContents.split('\n');

        textureAtlases.clear();
        textureButtonAreas.clear();

        bool processingButtonArea = false;
        bool processingAtlas = false;

        TextureAtlasLoader textureAtlasLoader;
        int atlasInformationIndex = 0;

        TextureButtonAreaLoader textureButtonAreaLoader;

        for(const auto &i : splitContents)
        {
            if(i.isEmpty())
            {
                continue;
            }

            if(i.contains('='))
            {
                processingAtlas = false;
                processingButtonArea = false;

                if(!textureButtonAreaLoader.textures.empty())
                {
                    textureButtonAreas.push_back(textureButtonAreaLoader);

                    textureButtonAreaLoader = TextureButtonAreaLoader{};
                }

                if(!textureAtlasLoader.textures.empty())
                {
                    textureAtlases.push_back(textureAtlasLoader);

                    textureAtlasLoader = TextureAtlasLoader{};

                    atlasInformationIndex = 0;
                }
            }

            // Handle Button Area

            if(i.contains("Texture Button Area"))
            {
                processingButtonArea = true;

                textureButtonAreaLoader.areaName = i.split(": ")[1];
            }
            else if(processingButtonArea)
            {
                textureButtonAreaLoader.textures.push_back(i);
            }

            // Handle Atlas

            if(i.contains("Atlas Name"))
            {
                processingAtlas = true;

                textureAtlasLoader.atlasName = i.split(": ")[1];

                atlasInformationIndex += 1;
            }
            else if(processingAtlas)
            {
                if(atlasInformationIndex == 1)
                {
                    QStringList dimensions = i.split(' ');

                    textureAtlasLoader.atlasSize = QSize{dimensions[1].toInt(), dimensions[3].toInt()};

                    atlasInformationIndex += 1;
                }
                else if(atlasInformationIndex == 2)
                {
                    textureAtlasLoader.format = i.split(": ")[1];

                    atlasInformationIndex += 1;
                }
                else
                {
                    QStringList textureInformation = i.split(' ' );

                    TexturePos texturePos;
                    texturePos.textureLocation = textureInformation[1];
                    texturePos.position = QPoint{textureInformation[4].toInt(), textureInformation[6].toInt()};

                    textureAtlasLoader.textures.push_back(texturePos);
                }
            }
        }
    }
}