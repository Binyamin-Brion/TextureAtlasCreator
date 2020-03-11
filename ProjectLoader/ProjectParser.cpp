//
// Created by BinyBrion on 2020-03-09.
//

#include <QtCore/QFile>
#include <c++/fstream>
#include <c++/sstream>
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
        // Reading and processing of file contents are done using std::strings instead of QStrings as using QStrings
        // causes this function to fail in tests on certain computers.
        std::ifstream fileReader{projectFileLocation.toStdString()};

        if(!fileReader.is_open())
        {
            return false;
        }

        std::stringstream fileContentsBuffer;

        fileContentsBuffer << fileReader.rdbuf();

        std::vector<std::string> splitContents;

        std::string fileSegment;

        while(std::getline(fileContentsBuffer, fileSegment, '\n'))
        {
            splitContents.push_back(fileSegment);
        }

        textureAtlases.clear();
        textureButtonAreas.clear();

        bool processingButtonArea = false;
        bool processingAtlas = false;

        TextureAtlasLoader textureAtlasLoader;
        int atlasInformationIndex = 0;

        TextureButtonAreaLoader textureButtonAreaLoader;

        for(const auto &i : splitContents)
        {
            QString qString = QString::fromStdString(i);

            if(qString.isEmpty())
            {
                continue;
            }

            if(qString.contains('='))
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

            if(qString.contains("Texture Button Area"))
            {
                processingButtonArea = true;

                textureButtonAreaLoader.areaName = qString.split(": ")[1];
            }
            else if(processingButtonArea)
            {
                textureButtonAreaLoader.textures.push_back(qString);
            }

            // Handle Atlas

            if(qString.contains("Atlas Name"))
            {
                processingAtlas = true;

                textureAtlasLoader.atlasName = qString.split(": ")[1];

                atlasInformationIndex += 1;
            }
            else if(processingAtlas)
            {
                if(atlasInformationIndex == 1)
                {
                    QStringList dimensions = qString.split(' ');

                    textureAtlasLoader.atlasSize = QSize{dimensions[1].toInt(), dimensions[3].toInt()};

                    atlasInformationIndex += 1;
                }
                else if(atlasInformationIndex == 2)
                {
                    textureAtlasLoader.format = qString.split(": ")[1];

                    atlasInformationIndex += 1;
                }
                else
                {
                    QStringList textureInformation = qString.split(' ' );

                    TexturePos texturePos;
                    texturePos.textureLocation = textureInformation[1];
                    texturePos.position = QPoint{textureInformation[4].toInt(), textureInformation[6].toInt()};

                    textureAtlasLoader.textures.push_back(texturePos);
                }
            }
        }
    }
}