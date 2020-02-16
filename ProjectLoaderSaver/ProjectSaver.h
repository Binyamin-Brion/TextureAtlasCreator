//
// Created by BinyBrion on 2020-02-15.
//

#ifndef TEXTUREATLASCREATOR_PROJECTSAVER_H
#define TEXTUREATLASCREATOR_PROJECTSAVER_H

#include "TextureSaveInformation.h"

namespace ProjectLoaderSaver
{
    /**
     *  Object used to save a project to a file.
     *
     *  In order to use it, write all of the appropriate data that needs to be saved using
     *  the appropriate writeXXX functions. Then, call save().
     *
     *  Note that all of the data that has to be saved must be present when calling save();
     *  the data to be saved is erased from program memory after calling save. If a piece of data
     *  is forgotten to be included after calling save, then ALL of the data that has to be saved must
     *  be written back using the appropriate writeXXX functions.
     */

    class ProjectSaver
    {
        public:

            // All project files saved will have the extension represented by the projectExtension variable
            static const QString projectExtension;

            /**
             * Saves the file to the passed in destination folder using the passed in name.
             *
             * @param saveDestination the location in the local file system to save the project to
             * @param saveName the name of the file holding the saved project
             * @throws runtime_error if the file with the project name already exists or if a file could not be created
             *         in order to be written to
             */
            static void save(const QString &saveDestination, QString saveName);

            /**
             * Stores the drawing position of a texture in the given atlas.
             *
             * @param textureLocation the texture that the passed in coordinates are referring to
             * @param x the x-position of the drawing coordinates
             * @param y the y-position of the drawing coordinates
             */
            static void writeTextureLocations(const QString &textureLocation, int x, int y);

            /**
             * Stores the texture name of the passed in texture location (serves as an alias).
             *
             * @param textureLocation the texture location that the texture name is referring to
             * @param textureName the alias name for the texture location
             */
            static void writeTextureName(const QString &textureLocation, QString textureName);

            /**
             * Stores the intersection border width of the passed in texture location .
             *
             * @param textureLocation the texture the intersection border width is referring to
             * @param intersectionBorderWidth the intersection border width for texture location
             */
            static void writeIntersectionBorderWidth(const QString &textureLocation, int intersectionBorderWidth);

            /**
             * Stores the selection border width of the passed in texture location.
             *
             * @param textureLocation the texture the selection border width is referring to
             * @param selectionBorderWidth the selection border width for texture location
             */
            static void writeSelectionBorderWidth(const QString &textureLocation, int selectionBorderWidth);

            /**
             * Stores the description, if any, for the passed in texture location.
             *
             * @param textureLocation the texture the description is associated with
             * @param description the description for texture location
             */
            static void writeDescription(const QString &textureLocation, QString description);

        private:

            /**
             * Finds the index into textureLocations for the given texture location. Used to reduce code duplication.
             *
             * @param textureSaveInformation the texture that the index should refer to
             * @return the index of the texture location passed in
             */
            static int findTextureSaveIndex(const QString &textureSaveInformation);

            // Holds all of the texture-related information of all the texture locations referred to by the writeXXX functions
            static std::vector<TextureSaveInformation> textureLocations;
    };
}

#endif //TEXTUREATLASCREATOR_PROJECTSAVER_H
