//
// Created by BinyBrion on 2020-03-09.
//

#ifndef TEXTUREATLASCREATOR_PROJECTPARSER_H
#define TEXTUREATLASCREATOR_PROJECTPARSER_H

#include "TextureAtlasLoader.h"
#include "TextureButtonAreaLoader.h"

namespace Tests
{
    namespace GUI
    {
        class TestTextureButtonArea;
    }

    namespace TextureAtlas
    {
        class TestTextureAtlas;
    }
}

namespace ProjectLoader
{
    /**
     * Deals with parsing a project file and extracting the required information to reload a saved project.
     */

    class ProjectParser
    {
            friend class ::Tests::GUI::TestTextureButtonArea;
            friend class ::Tests::TextureAtlas::TestTextureAtlas;

        public:

            /**
             * Returns the atlases that were loaded from the project file.
             *
             * @return vector of atlases to reload
             */
            const std::vector<TextureAtlasLoader>& getTextureAtlases();

            /**
             * Returns tje texture button areas that were loaded from the project file.
             *
             * @return vector of button areas to reload
             */
            const std::vector<TextureButtonAreaLoader>& getTextureButtonAreas();

            /**
             * Parses the project file given to this function.
             *
             * @param projectFileLocation location on the file system of the project file to parse.
             * @return true if the parsing was successful
             */
            void parseFile(const QString &projectFileLocation);

        private:

            /**
             * Converts the source string into a number. If this could not be done, then an exception using
             * the baseErrorMessage is thrown. The offending source string is also included in the exception message.
             *
             * @param source string to convert to an int
             * @param baseErrorMessage string to serve as part of the thrown exception message
             * @return an integer equivalent of the source string, if an exception is not thrown
             */
            int checkConversionSuccess(const QString &source, const QString &baseErrorMessage);

            /**
             * Parses the input string if the current line of the project file being parsed specifies an atlas.
             *
             * @param qString the current line of the project file being parsed
             */
            void parseAtlas(const QString &qString);

            /**
           * Parses the input string if the current line of the project file being parsed specifies a texture button area.
           *
           * @param qString the current line of the project file being parsed
           */
            void parseButtonArea(const QString &qString);

            std::vector<std::string> readFile(const QString &projectFileLocation) const;

            std::vector<TextureAtlasLoader> textureAtlases;
            std::vector<TextureButtonAreaLoader> textureButtonAreas;

            // Variables used for parsing project file
            bool processingButtonArea = false;
            bool processingAtlas = false;

            // Keeps track what piece of atlas information is currently processed.
            int atlasInformationIndex = 0;

            TextureButtonAreaLoader textureButtonAreaLoader;
            TextureAtlasLoader textureAtlasLoader;
    };
}

#endif //TEXTUREATLASCREATOR_PROJECTPARSER_H
