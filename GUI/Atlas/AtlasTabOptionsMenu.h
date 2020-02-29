//
// Created by BinyBrion on 10/11/2019.
//

#ifndef TEXTUREATLASCREATOR_ATLASTABOPTIONSMENU_H
#define TEXTUREATLASCREATOR_ATLASTABOPTIONSMENU_H

#include <QtWidgets/QMenu>

namespace GUI
{
    namespace Atlas
    {
        /**
         * Holds the options that a user can perform on a TextureAtlas when right clicking it.
         * Whenever an action is pressed, program control goes to the appropriate SLOT in the
         * AtlasTabWidget class.
         */

        class AtlasTabOptionsMenu : public QMenu
        {
                Q_OBJECT

            signals:

                /**
                 *  Emitted when user right clicks and selects option to add new tab.
                 */
                void addTabActionTriggered();

                /**
                 *  Emitted when the user requests to move the currently viewed tab to the left.
                 */
                void moveTabLeftTriggered();

                /**
                 *  Emitted when the user requests to move the currently viewed tab to the right.
                 */
                void moveTabRightTriggered();

                /**
                 *  Emitted wen user right clicks and selects option to delete the current tab.
                 */
                void deleteTabActionTriggered();

                /**
                 * Emitted when user right clicks and selects option to rename the current tab.
                 */
                void renameTabActionTriggered();

            public:

                /**
                 * Initializes this object with the parent passed in.
                 *
                 * @param parent widget that has ownership over this object.
                 */
                explicit AtlasTabOptionsMenu(QWidget *parent = nullptr);

            private:

                // The below actions appear in a popup context menu when the user right clicks, and invoke
                // the respective action as defined by the variable name.
                QAction addTab;
                QAction renameTab;
                QAction _moveTabLeft;
                QAction _moveTabRight;
                QAction deleteCurrentTab;
        };
    }
}

#endif //TEXTUREATLASCREATOR_ATLASTABOPTIONSMENU_H
