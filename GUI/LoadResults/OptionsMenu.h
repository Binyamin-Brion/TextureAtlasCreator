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
        /*
         *  Provides a menu of options that can be performed on the current TextureButtonArea if it is right-clicked.
         *  Note that the LoadedTextures Widget also used this Menu. Both TextureButtonArea and LoadTextures Widget
         *  pass in the appropriate bools to the constructor so that only valid actions are presented depending if the user
         *  clicks the button area or the tab widget holding those areas.
         */

        class OptionsMenu : public QMenu
        {
            Q_OBJECT

            public:
                OptionsMenu(bool showDeleteTextureButton, bool showDeleteTextureButtonArea, QWidget *parent = nullptr);
                void showDeleteAction(bool value);

            signals:
                void addTabActionTriggered();
                void renameTabActionTriggered();
                void moveTabLeft();
                void moveTabRight();
                void deleteActionTriggered();
                void deleteTabTriggered();

            private:
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
