//
// Created by BinyBrion on 2020-03-09.
//

#include <QtCore/QFile>
#include <fstream>
#include <sstream>
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

    void ProjectParser::parseFile(const QString &projectFileLocation)
    {
        std::vector<std::string> splitContents = readFile(projectFileLocation);

        // Ensure that repeated parsing of project files does not include result of previous project loading.
        textureAtlases.clear();
        textureButtonAreas.clear();

        for(const auto &i : splitContents)
        {
            QString qString = QString::fromStdString(i);

            // Signals the end of a section within the project file
            if(qString.contains('='))
            {
                // Condition is true if a texture button area was the last thing processed
                if(processingButtonArea)
                {
                    textureButtonAreas.push_back(textureButtonAreaLoader);

                    textureButtonAreaLoader = TextureButtonAreaLoader{};
                }

                // Condition is true if a texture atlas was the last thing processed
                if(processingAtlas)
                {
                    textureAtlases.push_back(textureAtlasLoader);

                    textureAtlasLoader = TextureAtlasLoader{};

                    atlasInformationIndex = 0;
                }

                processingAtlas = false;
                processingButtonArea = false;
            }

            // If a line results in parsing a button area, then parseArea() will not do anything and this is true the other way around

            parseButtonArea(qString);

            parseAtlas(qString);

        }
    }

    int ProjectParser::checkConversionSuccess(const QString &source, const QString &baseErrorMessage)
    {
        bool ok = true;

        int convertedResult = source.toInt(&ok);

        if(!ok)
        {
            throw std::runtime_error{(baseErrorMessage + ". The text : " + source + " is not a valid number.").toStdString()};
        }

        return convertedResult;
    }

    void ProjectParser::parseAtlas(const QString &qString)
    {std::string fuckyou = qString.toStdString();
        if(qString.contains("Atlas Name"))
        {
            // Line format: Atlas Name: atlasName
            processingAtlas = true;

            QStringList atlasNameInformation = qString.split(QRegExp{":\\s+"});

            if(atlasNameInformation.size() != 2) // Index 1 must be valid
            {
                throw std::runtime_error{"Error reading an atlas name- it does not match the expected format."};
            }

            textureAtlasLoader.atlasName = atlasNameInformation[1];

            atlasInformationIndex += 1;
        }
        else if(processingAtlas)
        {
            if(atlasInformationIndex == 1)
            {
                const QString errorMessage = "Error reading dimensions for atlas: " + textureAtlasLoader.atlasName;

                // Line format: Dimensions: width , height
                QStringList dimensions = qString.split(QRegExp{"\\s+"});

                if(dimensions.size() != 4) // Index 3 must be valid
                {
                    throw std::runtime_error{(errorMessage + ". The format was not as expected").toStdString()};
                }

                textureAtlasLoader.atlasSize = QSize{checkConversionSuccess(dimensions[1], errorMessage), checkConversionSuccess(dimensions[3], errorMessage)};

                atlasInformationIndex += 1;
            }
            else if(atlasInformationIndex == 2)
            {
                QStringList formatInformation = qString.split(QRegExp{":\\s+"});

                if(formatInformation.size() != 2) // Index 1 must be valid
                {
                    throw std::runtime_error{("Error atlas format for atlas: " + textureAtlasLoader.atlasName + ". The format was not as expected.").toStdString()};
                }

                // Line format: Format: format
                textureAtlasLoader.format = formatInformation[1];

                atlasInformationIndex += 1;
            }
            else
            {
                const QString errorMessage = "Error information about texture within atlas: " + textureAtlasLoader.atlasName;

                // Line format: Texture: textureLocation -> Position: width , height
                QStringList textureInformation = qString.split(QRegExp{"\\s+"} );

                if(textureInformation.size() != 7) // Index 7 must be valid
                {
                    throw std::runtime_error{(errorMessage + ". The format was not as expected.").toStdString()};
                }

                TexturePos texturePos;
                texturePos.textureLocation = textureInformation[1];
                texturePos.position = QPoint{checkConversionSuccess(textureInformation[4], errorMessage), checkConversionSuccess(textureInformation[6], errorMessage)};

                textureAtlasLoader.textures.push_back(texturePos);
            }
        }
    }

    void ProjectParser::parseButtonArea(const QString &qString)
    {
        if(qString.contains("Texture Button Area"))
        {
            processingButtonArea = true;

            QStringList buttonAreaName = qString.split(QRegExp{":\\s+"});

            if(buttonAreaName.size() != 2) // Index 1 must be valid
            {
                throw std::runtime_error{"Error reading a button area name- it does not match the expected format."};
            }

            // Line format: Texture Button Area Name: AreName
            textureButtonAreaLoader.areaName = buttonAreaName[1];
        }
        else if(processingButtonArea)
        {
            const QString errorMessage = "Error reading button area texture for button area: " + textureButtonAreaLoader.areaName;

            // Line format: textureLocation -> selectionBorderWidth , intersectionBorderWidth
            QStringList textureInformation = qString.split(QRegExp{"\\s+"});

            if(textureInformation.size() != 5) // Index 4 must be valid
            {
                throw std::runtime_error{(errorMessage + ". The format does not match what was expected.").toStdString()};
            }

            TextureData textureData;

            textureData.textureLocation = textureInformation[0];
            textureData.intersectionWidth = checkConversionSuccess(textureInformation[4], errorMessage);
            textureData.selectionWidth = checkConversionSuccess(textureInformation[2], errorMessage);

            textureButtonAreaLoader.textures.push_back(textureData);
        }
    }

    std::vector<std::string> ProjectParser::readFile(const QString &projectFileLocation) const
    {
        // Reading and processing of file contents are done using std::strings instead of QStrings as using QStrings
        // causes this function to fail in tests on certain computers.
        std::ifstream fileReader{projectFileLocation.toStdString()};

        if(!fileReader.is_open())
        {
            throw std::runtime_error{"Unable to open the file: " + projectFileLocation.toStdString()};
        }

        std::stringstream fileContentsBuffer;

        fileContentsBuffer << fileReader.rdbuf();

        std::vector<std::string> splitContents;

        std::string fileSegment;

        while(std::getline(fileContentsBuffer, fileSegment, '\n'))
        {
            if(fileSegment.empty())
            {
                continue;
            }

            splitContents.push_back(fileSegment);
        }

        return splitContents;
    }
}