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
        /*
         * Holds the options that a user can perform on a TextureAtlas when right clicking it.
         * Whenever an action is pressed, program control goes to the appropriate SLOT in the
         * AtlasTabWidget class.
         */

        class AtlasTabOptionsMenu : public QMenu
        {
            Q_OBJECT

            public:
                explicit AtlasTabOptionsMenu(QWidget *parent = nullptr);

            signals:
                void addTabActionTriggered();
                void moveTabLeft();
                void moveTabRight();
                void deleteTabActionTriggered();
                void renameTabActionTriggered();

            private:
                QAction addTab;
                QAction renameTab;
                QAction _moveTabLeft;
                QAction _moveTabRight;
                QAction deleteCurrentTab;
        };
    }
}

#endif //TEXTUREATLASCREATOR_ATLASTABOPTIONSMENU_H
