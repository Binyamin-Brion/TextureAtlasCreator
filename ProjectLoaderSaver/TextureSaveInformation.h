//
// Created by BinyBrion on 2020-02-15.
//

#ifndef TEXTUREATLASCREATOR_TEXTURESAVEINFORMATION_H
#define TEXTUREATLASCREATOR_TEXTURESAVEINFORMATION_H

#include <QString>

namespace ProjectLoaderSaver
{
    /*
     *  Holds all of the information associated with a texture, and using
     *  the information stored here, the texture can be restored as it was
     *  when the project saved.
     */

    struct TextureSaveInformation
    {
        QString textureLocation;
        QString textureName;

        // Following two members to specify drawing positions
        int xPos;
        int yPos;

        int selectionBorderWidth;
        int intersectionBorderWidth;

        QString description;

        // Note that for the two following members, vectors are needed as a texture
        // can be a part of more than one tab

        // Refers to the tabs where a user can click on textures to add to them to atlases
        std::vector<QString> textureTabs;

        // Refers to the tabs holding the atlases
        std::vector<QString> atlasTabs;
    };
}

#endif //TEXTUREATLASCREATOR_TEXTURESAVEINFORMATION_H
