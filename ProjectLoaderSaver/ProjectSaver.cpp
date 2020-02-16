//
// Created by BinyBrion on 2020-02-15.
//

#include <fstream>
#include <QtCore/QFileInfo>
#include "ProjectSaver.h"

namespace ProjectLoaderSaver
{
    const QString ProjectSaver::projectExtension = ".texat";
    std::vector<TextureSaveInformation> ProjectSaver::textureLocations;

    // Beginning of public functions

    void ProjectSaver::save(const QString &saveDestination, QString saveName)
    {
        // In case the passed in name does not include the project extension, it should be added now so that when a
        // file is created to hold the project contents, it will have the extension. The project extension should not
        // be appended to saveName when calling this function, but just in case it is, a check is executed to ensure
        // that the extension does not appear twice in the saved file name.

        if(!saveName.contains(projectExtension))
        {
            saveName += projectExtension;
        }

        // From now on, only the absolute path including the to-be file name is worked with, as only then can
        // a file be created at the proper location and checks to see if the file can be created in the first place

        QString fullDestinationPath = saveDestination + "/" + saveName;

        QFileInfo fileInformation{fullDestinationPath};

        // This class does not have the authority to overwrite existing files; the caller of this function must
        // notify the user of this so that they can take appropriate action.

        if(fileInformation.exists())
        {
            throw std::runtime_error{"A project with this name already exists!\n"};
        }

        std::ofstream writeProject{fullDestinationPath.toStdString()};

        if(!writeProject.is_open())
        {
            throw std::runtime_error{"Unable to create the file to save to!"};
        }

        // When saving a texture piece of information, there is only whitespace separating the label of what is being
        // written and the actual information so that it is easy to read back when opening a project

        for(const auto &i : textureLocations)
        {
            writeProject << "BEGIN TEXTURE\n\n";
            writeProject << "Texture Location: " << i.textureLocation.toStdString() << '\n';
            writeProject << "Texture Name: " << i.textureName.toStdString() << '\n';
            writeProject << "Drawing Positions: " << i.xPos << " , " << i.yPos << '\n';
            writeProject << "Selection Border Width: " << i.selectionBorderWidth << '\n';
            writeProject << "Intersection Border Width: " << i.intersectionBorderWidth << '\n';
            writeProject << "Description: " << i.description.toStdString() << '\n';

            for(const auto &textureTab : i.textureTabs)
            {
                writeProject << "Texture Tab: " << textureTab.toStdString() << '\n';
            }

            for(const auto &atlasTab : i.atlasTabs)
            {
                writeProject << "Atlas Tab: " << atlasTab.toStdString() << '\n';
            }

            writeProject << "\nEnd Texture\n\n";
        }

        textureLocations.clear();
    }

    void ProjectSaver::writeTextureLocations(const QString &textureLocation, int x, int y)
    {
        textureLocations[findTextureSaveIndex(textureLocation)].xPos = x;
        textureLocations[findTextureSaveIndex(textureLocation)].yPos = y;
    }

    void ProjectSaver::writeTextureName(const QString &textureLocation, QString textureName)
    {
        textureLocations[findTextureSaveIndex(textureLocation)].textureName = textureName;
    }

    void ProjectSaver::writeIntersectionBorderWidth(const QString &textureLocation, int intersectionBorderWidth)
    {
        textureLocations[findTextureSaveIndex(textureLocation)].intersectionBorderWidth = intersectionBorderWidth;
    }

    void ProjectSaver::writeSelectionBorderWidth(const QString &textureLocation, int selectionBorderWidth)
    {
        textureLocations[findTextureSaveIndex(textureLocation)].selectionBorderWidth = selectionBorderWidth;
    }

    void ProjectSaver::writeDescription(const QString &textureLocation, QString description)
    {
        textureLocations[findTextureSaveIndex(textureLocation)].description = description;
    }

    // Beginning of private functions

    int ProjectSaver::findTextureSaveIndex(const QString &textureLocation)
    {
        // If the texture location could not be found in textureLocations, then it means that an additional texture
        // will be saved. To account for this, append an additional elements to textureLocations where information
        // regarding this new texture will be stored. As a result, the return value of this function is always valid.

        for(int i = 0; i < textureLocations.size(); ++i)
        {
            if(textureLocations[i].textureLocation == textureLocation)
            {
                return i;
            }
        }

        textureLocations.emplace_back();

        return textureLocations.size() - 1;
    }
}