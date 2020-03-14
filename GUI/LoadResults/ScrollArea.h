//
// Created by BinyBrion on 2019-09-15.
//

#ifndef TEXTUREATLASCREATOR_SCROLLAREA_H
#define TEXTUREATLASCREATOR_SCROLLAREA_H


#include <QtWidgets/QScrollArea>

namespace TextureLogic
{
    class Texture;
    class TextureBank;
}

namespace GUI
{
    namespace LoadResults
    {
        class TextureButtonArea;

        /**
         *  Provides a scroll area to a TextureButtonArea. Since a TextureButtonArea can hold many buttons that may
         *  not fit in the given space allocated to it onscreen, a scrollable view is required to be able to view
         *  all of the buttons that TextureButtonArea holds.
         */

        class ScrollArea : public QScrollArea
        {
            public:

                /**
                 * Initializes this object with the parent passed in.
                 *
                 * @param parent widget that has ownership over this object
                 */
                explicit ScrollArea(QWidget *parent = nullptr);

                /**
                 * Adds a texture button to the texture button area this scroll are holds. Forwards arguments to the texture button area.
                 *
                 * @param textureLocation location of the texture the texture button should associate with
                 * @param intersectionBorderWidth of the texture to be loaded
                 * @param selectionBorderWidth of the texture to be loaded
                 * @param loadingProject true if this function is called when loading a project
                 */
                void addTextureButton(const QString &textureLocation, unsigned int intersectionBorderWidth, unsigned int selectionBorderWidth, bool loadingProject);

                /**
                 *  Removes all of the texture buttons. Used when a texture button is to be deleted. Forwards call to the texture button area.
                 *
                 * @param whether or not the signal is being emitted as a result of the current project closing
                 */
                void deleteTextureButtons(bool closingProgram);

                /**
                 * Gets the texture button area this objects holds.
                 *
                 * @return reference to the texture button area held in this object.
                 */
                const TextureButtonArea* getTextureArea() const;

                /**
              * Checks if there are any unsaved changes in the texture button area.
              *
              * @return true if there are unsaved changes
              */
                bool getUnsavedChanges() const;

                /**
                 * Called when the selected texture has been painted, and the appropriate texture buttons have to be updated
                 * to reflect this visual change.
                 *
                 * @param texture reference to the texture that was changed
                 */
                void paintTextureButton(const ::TextureLogic::Texture *texture);

                /**
                 * Forwards request to save the loaded textures to the texture button area.
                 *
                 * @param textureButtonAreaName name of the texture button area holding the texture buttons
                 * @param saveLocation location to save the texture locations to
                 */
                void saveLoadedTextures(const QString &textureButtonAreaName, const QString &saveLocation) const;

                /**
                 * Set the reference to the texture bank area. Forwards argument to the texture button area.
                 *
                 * @param textureBank reference to the texture bank that holds all textures
                 */
                void setTextureBankReference(TextureLogic::TextureBank *textureBank);

                /**
                 * Forwards request to all texture button areas that represent the modified texture to now represent
                 * the saved modified texture.
                 *
                 * @param previousLocation location on the file system that the texture buttons had before the modified texture was saved
                 * @param newLocation new location on the file system of the saved modified texture
                 */
                void updateTextureButtonsLocation(const QString &previousLocation, const QString &newLocation);

            private:

                // The actual resource this object holds
                TextureButtonArea *textureButtonArea = nullptr;
        };
    }
}

#endif //TEXTUREATLASCREATOR_SCROLLAREA_H
