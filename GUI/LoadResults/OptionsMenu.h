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
        class OptionsMenu : public QMenu
        {
            Q_OBJECT

            public:
                explicit OptionsMenu(QWidget *parent = nullptr);
                void showDeleteAction(bool value);

            signals:
                void deleteActionTriggered();

            private:
                QAction moveTabLeft;
                QAction moveTabRight;
                QAction deleteTextureButton;
        };
    }
}

#endif //TEXTUREATLASCREATOR_OPTIONSMENU_H
