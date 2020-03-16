//
// Created by BinyBrion on 2019-09-12.
//

#ifndef TEXTUREATLAS_TEXTUREBANK_H
#define TEXTUREATLAS_TEXTUREBANK_H

#include "TextureLogic/Texture.h"

#include "HelperUtilities/PassKey.h"
#include "GUI/TextureHelperFunctions/HelperFunctions.h"

#include <unordered_map>

namespace Atlas
{
    class TextureAtlas;
}

namespace GUI
{
    class MainWindow;

    namespace Atlas
    {
        class AtlasTabWidget;
    }

    namespace CurrentTexture
    {
        namespace SpecularTexture
        {
            class SpecularTextureTabWidget;
        }

        class CurrentTextureTabWidget;
    }

    namespace LoadResults
    {
        class LoadedTextures;
        class TextureButtonArea;
    }

    namespace TextureInformation
    {
        class TextureInfoScrollArea;
    }
}

namespace TextureLogic
{
    class TextureBank
    {
            using FormatIndex = unsigned int;
            using UnusedSpotIndexes = unsigned int;

        public:

            /**
             * Initializes required internal variables.
             */
            TextureBank();

            /**
            * Gets the width of the selection border for the texture that the input location represents.
            *
            * @param textureLocation location of the texture on the file system being queried when it was loaded into the program
            * @return selection border width
            */
            unsigned int getIntersectionWidth(const QString &textureLocation);

            /**
             * Gets the width of the selection border for the texture that the input location represents.
             *
             * @param textureLocation location of the texture on the file system being queried when it was loaded into the program
             * @return selection border width
             */
            unsigned int getSelectionWidth(const QString &textureLocation);

            /**
             * Gets the textures that have been loaded thus far.
             *
             * @return vector containing the textures loaded and free vector spots for each format.
             */
            const std::vector<std::pair<std::vector<Texture>, std::vector<UnusedSpotIndexes>>>& getTextures() const;

            /**
             *  Erases a loaded texture from the program; afterwards, that texture is no longer stored in memory.
             *
             * @param textureLocation the location of the texture to be deleted, which was used to load the texture originally
             */
            void removeTexture(const QString &textureLocation);

            /**
             *  Called when the currently selected texture has been painted.
             */
            void selectedTextureChanged();

            /**
             * Sets the reference to the atlas tab widget holding all of the texture atlases.
             *
             * @param atlasTabWidget reference to the atlas tab widget
             */
            void setAtlasTabWidgetReference(GUI::Atlas::AtlasTabWidget *atlasTabWidget);

            /**
             * Sets the reference to the widget holding the texture buttons.
             *
             * @param loadedTexturesWidget
             */
            void setLoadedTextures(GUI::LoadResults::LoadedTextures *loadedTexturesWidget);

            /**
             * Sets the reference to the tab widget holding the currently selected reference.
             *
             * @param currentTextureTabWidget reference to the current texture tab widget
             */
            void setSelectedTextureTabWidgetReference(GUI::CurrentTexture::CurrentTextureTabWidget *currentTextureTabWidget);

            /**
             * Sets the reference to the widget holding the specular texture for the selected texture.
             *
             * @param specularTextureTabWidget
             */
            void setSpecularTabWidgetReference(GUI::CurrentTexture::SpecularTexture::SpecularTextureTabWidget *specularTextureTabWidget);

            /**
             * Updates the intersection width of the passed in texture.
             *
             * @param texture whose intersection width should change
             * @param zoom at which the new border width is being specified
             * @param newBorderWidth that the texture should have for the intersection border width
             * @return true if the change in intersection border width could be applied
             */
            bool setIntersectionBorderWidth(Texture *texture, Zoom zoom, unsigned int newBorderWidth);

            /**
             * Updates the selection border width of the passed in texture.
             *
             * @param texture whose selection width should change
             * @param zoom at which the new border width is being specified
             * @param newBorderWidth that the texture should have for the selection border width
             */
            void setSelectionBorderWidth(Texture *texture, Zoom zoom, unsigned int newBorderWidth);

            /**
             * Sets the reference to the scroll area holding all of the information about the currently selected texture.
             *
             * @param textureInfoScrollArea reference to the texture content scroll area widget
             */
            void setTextureInfoScrollAreaReference(GUI::TextureInformation::TextureInfoScrollArea *textureInfoScrollArea);

            /**
             * Stores a new texture into the program; afterwards it is available to be placed in atlases and modified.
             *
             * @param textureLocation location of the texture to be loaded on the file system
             * @param intersectionBorderWidth that the texture should have at a 100% (ie Normal) zoom
             * @param selectionBorderWidth that the texture should have at a 100% (ie Normal) zoom
             */
            void storeNewTexture(const QString &textureLocation, unsigned int intersectionBorderWidth, unsigned int selectionBorderWidth, AccessRestriction::PassKey<GUI::LoadResults::TextureButtonArea>);

            /**
             * Called when a texture is saved from within the program. See the TextureInfo classes in GUI for more info
             *
             * @param textureLocation location of the texture to be loaded on the file system
             * @param intersectionBorderWidth that the texture should have at a 100% (ie Normal) zoom
             * @param selectionBorderWidth that the texture should have at a 100% (ie Normal) zoom
             */
            void reuploadTexture(const QString &textureLocation, unsigned int intersectionBorderWidth, unsigned int selectionBorderWidth, AccessRestriction::PassKey<GUI::TextureInformation::TextureInfoScrollArea>);

            /**
             * Adds the specified texture to the currently opened atlas. If the texture has not been loaded in the format
             * of the currently opened atlas, it is first loaded as that format and then added to the atlas.
             *
             * @param textureLocation location of the texture to be loaded on the file system
             * @param intersectionBorderWidth that the texture should have at a 100% (ie Normal) zoom
             * @param selectionBorderWidth that the texture should have at a 100% (ie Normal) zoom
             */
            void textureButtonPressedButtonArea(const QString &textureLocation, unsigned int intersectionBorderWidth, unsigned int selectionBorderWidth, AccessRestriction::PassKey<GUI::LoadResults::TextureButtonArea>);

            /**
             * Adds the specified texture to the currently opened atlas. If the texture has not been loaded in the format
             * of the currently opened atlas, it is first loaded as that format and then added to the atlas. Called when loading
             * from a project file.
             *
             * @param atlasName name of the atlas to add the texture to
             * @param textureLocation location on the file system of the texture being added
             * @param intersectionBorderWidth that the texture should have at a 100% (ie Normal) zoom
             * @param selectionBorderWidth that the texture should have at a 100% (ie Normal) zoom
             * @param texturePosition location of the loaded texture within the atlas  (QSize == {-1, -1} if no position is known)
             */
            void textureButtonPressedMainWindow(const QString &atlasName, const QString &textureLocation, unsigned int intersectionBorderWidth, unsigned int selectionBorderWidth, QPoint texturePosition, AccessRestriction::PassKey<GUI::MainWindow>);

            /**
             * Informs other GUI widgets that the currently selected texture has changed.
             *
             * @param texture that is now selected
             */
            void textureSelected(const Texture *texture);

            /**
             * Forwards request to all texture button areas that represent the modified texture to now represent
             * the saved modified texture.
             *
             * @param previousLocation location on the file system that the texture buttons had before the modified texture was saved
             * @param newLocation new location on the file system of the saved modified texture
             */
            void updateTextureButtonLocation(const QString &previousLocation, const QString &newLocation);

        private:

            /**
             * Get the reference to the texture given its location on the file system.
             *
             * @param textureLocation location on the file system of the desired texture when it was loaded into the program
             * @return reference to the equivalent texture object. If no texture was found, a nullptr is returned
             */
            const Texture* findTexture(const QString &textureLocation) const;

            /**
             * Executes the logic required when adding a texture as a result of a texture button click.
             *
             * @param atlasName name of the atlas to add the texture to
             * @param textureLocation location on the file system of the texture being added
             * @param intersectionBorderWidth that the texture should have at a 100% (ie Normal) zoom
             * @param selectionBorderWidth that the texture should have at a 100% (ie Normal) zoom
             * @param texturePosition location of the loaded texture within the atlas
             */
            void handleTextureButtonPressed(const QString &atlasName, const QString &textureLocation, unsigned int intersectionBorderWidth, unsigned int selectionBorderWidth, QPoint texturePosition);

            /**
             * Actually does the work of loading a new texture into the program. The loading logic is put in a separate
             * function to reduce code duplication as this function is called by both storeNewTexture() and reuploadTexture().
             *
             * @param textureLocation location of the texture to be loaded on the file system
             * @param intersectionBorderWidth that the texture should have at a 100% (ie Normal) zoom
             * @param selectionBorderWidth that the texture should have at a 100% (ie Normal) zoom
             */
            void loadNewTexture(const QString &textureLocation, unsigned int intersectionBorderWidth, unsigned int selectionBorderWidth);

            /**
             * Ensures that the widgets referencing textures are pointing to a valid memory location. The previous location
             * may be invalid if loading a new texture caused the contents of the texture vector to be moved in memory.
             */
            void resetTextureReference();

            GUI::Atlas::AtlasTabWidget *atlasTabWidget = nullptr;
            GUI::CurrentTexture::CurrentTextureTabWidget *currentTextureTabWidget = nullptr;
            GUI::CurrentTexture::SpecularTexture::SpecularTextureTabWidget *specularTextureTabWidget  = nullptr;
            GUI::TextureInformation::TextureInfoScrollArea *textureInfoScrollArea = nullptr;
            GUI::LoadResults::LoadedTextures *loadedTexturesWidget = nullptr;

            // See TextureBank.cpp for more details of these two variables
            std::vector<std::pair<std::vector<Texture>, std::vector<UnusedSpotIndexes>>> textures;
            std::unordered_map<std::string, FormatIndex> originalTextureUploadLocation;
    };
}

#endif //TEXTUREATLAS_TEXTUREBANK_H
