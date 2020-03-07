//
// Created by BinyBrion on 2019-09-15.
//

#ifndef TEXTUREATLASCREATOR_LOADEDTEXTURES_H
#define TEXTUREATLASCREATOR_LOADEDTEXTURES_H

#include <QtWidgets/QTabWidget>
#include "ScrollArea.h"

namespace TextureLogic
{
    class TextureBank;
}

namespace GUI
{
    namespace Dialogs
    {
        class AddNewTab;
        class ChooseTexture;
    }

    namespace LoadResults
    {
        class OptionsMenu;
        class ScrollArea;

        /**
         *  Holds all of the TextureButtonsArea. In doing so, it holds all of the textures ever loaded into program.
         *  The user can find a loaded texture by navigating to the tab in which the texture was specified to be loaded
         *  into in the ChooseTexture dialog.
         */

        class LoadedTextures : public QTabWidget
        {
                Q_OBJECT

            public:

                /**
                 * Initializes this object with the parent passed in.
                 *
                 * @param parent widget that has ownership over this object.
                 */
                explicit LoadedTextures(QWidget *parent = nullptr);

                /**
                 * Adds a new area in which texture buttons can be placed after loading a texture.
                 *
                 * @param tabName that the new tab should have
                 */
                void addTextureButtonArea(const QString &tabName);

                /**
                 *  Goes through every texture button area, deletes all of the textures within them, and then deletes
                 *  all of the tabs.
                 */
                void closeAllTabs();

                /**
                 * Sets the reference to the texture bank. The reference is passed to the areas that will buttons of the
                 * loaded textures so that when a texture button is pressed, the texture bank can be notified of this
                 * action and add the clicked texture to the currently opened atlas.
                 *
                 * @param textureBank reference to the texture bank storing all of the loaded textures
                 */
                void setTextureBankReference(TextureLogic::TextureBank *textureBank);

            public slots:

                /**
                 *  Called to open the dialog used to add a new tab to this widget.
                 */
                void showAddTabDialog();

                /**
                 * Shows the context menu when the user right clicks the current atlas.
                 *
                 * @param pos the position of the cursor when the right-click event occurred
                 */
                void showContextMenu(const QPoint &pos);

                /**
                 *  Opens the dialog to load a texture. This is called from the mainwindow class as a keyboard shortcut to
                 *  open the dialog is defined there.
                 */
                void showLoadTextureDialog();

                /**
                 *  Called to open the dialog used to rename a tab held by this widget.
                 */
                void showRenameTabDialog();

            private slots:

                /**
                 *  Removes and deletes a tab along with the atlas inside. After this operation, the texture buttons
                 *  to load a texture into an atlas are gone unless they existed in a different tab.
                 *
                 *  Note: If the current tab   contains the last button for a texture, then the associated texture is
                 *        deleted from the texture bank AND all opened textures that have that texture. A warning will
                 *        be given in such a scenario.
                 */
                void deleteCurrentTab();

                /**
                 *  Moves the current tab to the left in the list of open tabs.
                 *  Internal note: See LoadedTextures.cpp
                 */
                void moveTabLeft();

                /**
                 *  Moves the current tab to the right in the list of open tabs.
                 *  Internal note: See LoadedTextures.cpp
                 */
                void moveTabRight();

                /**
                 * Handles the result of the addNewTab dialog, which may add a new tab or rename a tab depending on the
                 * context action used to show the dialog.
                 *
                 * @param newTabName result of the addNewTab dialog
                 */
                void newTabNameChosen(QString newTabName);

                /**
                 * Forwards the information specified in the ChooseTexture dialog to the the TextureButtonArea in the
                 * currently opened tab.
                 *
                 * @param textureLocation
                 * @param intersectionBorderWidth
                 * @param selectionBorderWidth
                 */
                void openTexture(QString textureButtonAreaName, QString textureLocation, unsigned int intersectionBorderWidth, unsigned int selectionBorderWidth);

            private:

                // See internal note for the variable currentTabs in LoadedTextures.cpp
                std::vector<std::pair<GUI::LoadResults::ScrollArea*, QString>> currentTabs;

                // Reference to the texture bank
                TextureLogic::TextureBank *textureBank = nullptr;

                // Keep track if the result of the addNewTab dialog is to add a new tab or rename an existing tab. This
                // works because the only thing the addNewTab gives back once a user is done is a name for a tab.
                // The state of this variable depends on what action was clicked in the context menu.
                bool renameTab = false;

                // References to dialog and context menu
                Dialogs::AddNewTab *addNewTab = nullptr;

                // This dialog is kept here, rather than in mainwindow where the shortcut to open it is, as logically
                // this dialog interacts with this class more so than the place from where it is opened
                Dialogs::ChooseTexture *chooseTexture = nullptr;
                OptionsMenu *optionsMenu = nullptr;
        };
    }
}

#endif //TEXTUREATLASCREATOR_LOADEDTEXTURES_H
