//
// Created by BinyBrion on 10/10/2019.
//

#ifndef TEXTUREATLASCREATOR_OPTIONSMENU_H
#define TEXTUREATLASCREATOR_OPTIONSMENU_H

#include <QtWidgets/QMenu>

namespace GUI
{
    namespace LoadResults
    {
        /**
         *  Provides a menu of options that can be performed on the current TextureButtonArea if it is right-clicked.
         *  Note that the LoadedTextures Widget also used this Menu in order to provide allow user to perform common
         *  functionality regardless if user clicks actual tab widget or tab contents. Both TextureButtonArea and LoadTextures Widget
         *  pass in the appropriate bool-s to the constructor so that only valid actions are presented depending if the user
         *  clicks the button area or the tab widget holding those areas.
         */

        class OptionsMenu : public QMenu
        {
                Q_OBJECT

            signals:

                /**
                 * Called when the action to delete a texture button when it is clicked.
                 */
                void deleteTextureButtonTriggered();

                /**
                 * Called when the action to delete a texture tab when it is clicked.
                 */
                void deleteTabTriggered();

                /**
                 *  Called when the action to add a tab when it is clicked.
                 */
                void addTabActionTriggered();

                /**
                 *  Called when the action to rename a tab when it is clicked.
                 */
                void renameTabActionTriggered();

                /**
                 * Called when the action to move a tab to the left when it is clicked.
                 */
                void moveTabLeftTriggered();

                /**
                 * Called when the action to move a tab to the right when it is clicked.
                 */
                void moveTabRightTriggered();

            public:

                /**
                 * Initializes the menu to display only the required actions along with the parent passed in.
                 *
                 * @param showDeleteTextureButton true if the action to delete a texture button should appear
                 * @param showDeleteTextureButtonArea true if the action to delete a texture button area should appear
                 * @param parent widget that has ownership over this object
                 */
                OptionsMenu(bool showDeleteTextureButton, bool showDeleteTextureButtonArea, QWidget *parent = nullptr);

                /**
                 * Called by the TextureButtonArea to dynamically choose if the action to delete a texture button should appear.
                 * While the TextureButtonArea initializes its instance of this object to show this action, it may want to hide
                 * it in certain cases. See TextureButtonArea for more details.
                 *
                 * @param value true if the action should be hidden
                 */
                void showTextureButtonDeleteAction(bool value);

            private:

                // Actions for the context menu, in the order appear in the menu
                QAction addTab;
                QAction renameTab;
                QAction _moveTabLeft;
                QAction _moveTabRight;
                QAction deleteTab;
                QAction deleteTextureButton;
        };
    }
}

#endif //TEXTUREATLASCREATOR_OPTIONSMENU_H
