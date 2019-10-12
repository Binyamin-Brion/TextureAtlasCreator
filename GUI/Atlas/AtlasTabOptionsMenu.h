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
        class AtlasTabOptionsMenu : public QMenu
        {
            Q_OBJECT

            public:
                explicit AtlasTabOptionsMenu(QWidget *parent = nullptr);

            signals:
                void addTabActionTriggered();

            private:
                QAction addTab;
                QAction moveTabLeft;
                QAction moveTabRight;
                QAction deleteCurrentTab;
                QAction currentTabProperties;
        };
    }
}

#endif //TEXTUREATLASCREATOR_ATLASTABOPTIONSMENU_H
