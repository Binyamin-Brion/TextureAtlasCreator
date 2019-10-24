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
            renameTab.setText("Rename Tab");
            _moveTabLeft.setText("Move Current Tab Left");
            _moveTabRight.setText("Move Current Tab Right");
            deleteCurrentTab.setText("Delete Current Tab");

            addAction(&addTab);
            addAction(&renameTab);
            addAction(&_moveTabLeft);
            addAction(&_moveTabRight);
            addAction(&deleteCurrentTab);

            connect(&addTab, &QAction::triggered, [this]()
            {
                emit addTabActionTriggered();
            });

            connect(&_moveTabLeft, &QAction::triggered, [this]()
            {
                emit moveTabLeft();
            });

            connect(&_moveTabRight, &QAction::triggered, [this]()
            {
                emit moveTabRight();
            });

            connect(&renameTab, &QAction::triggered, [this]()
            {
                emit renameTabActionTriggered();
            });

            connect(&deleteCurrentTab, &QAction::triggered, [this]()
            {
                emit deleteTabActionTriggered();
            });
        }
    }
}
