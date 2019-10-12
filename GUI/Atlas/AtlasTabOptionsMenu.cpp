//
// Created by BinyBrion on 10/11/2019.
//

#include "AtlasTabOptionsMenu.h"

namespace GUI
{
    namespace Atlas
    {
        AtlasTabOptionsMenu::AtlasTabOptionsMenu(QWidget *parent) : QMenu{parent}
        {
            addTab.setText("Add New Tab");
            moveTabLeft.setText("Move Current Tab Left");
            moveTabRight.setText("Move Current Tab Right");
            deleteCurrentTab.setText("Delete Current Tab");
            currentTabProperties.setText("Current Atlas Properties");

            addAction(&addTab);
            addAction(&moveTabLeft);
            addAction(&moveTabRight);
            addAction(&deleteCurrentTab);
            addAction(&currentTabProperties);

            connect(&addTab, &QAction::triggered, [this]()
            {
                emit addTabActionTriggered();
            });
        }
    }
}
