//
// Created by BinyBrion on 10/10/2019.
//

#include <QtWidgets/QMessageBox>
#include "OptionsMenu.h"

namespace GUI
{
    namespace LoadResults
    {
        OptionsMenu::OptionsMenu(QWidget *parent) : QMenu(parent)
        {
            addTab.setText("Add New Tab");
            renameTab.setText("Rename Tab");
            _moveTabLeft.setText("Move Tab Left");
            _moveTabRight.setText("Move Tab Right");
            deleteTab.setText("Delete Current Tab");
            deleteTextureButton.setText("Delete Texture Button");

            addAction(&addTab);
            addAction(&renameTab);
            addAction(&_moveTabLeft);
            addAction(&_moveTabRight);
            addAction(&deleteTab);

            connect(&addTab, &QAction::triggered, [this]()
            {
                emit addTabActionTriggered();
            });

            connect(&renameTab, &QAction::triggered, [this]()
            {
                emit renameTabActionTriggered();
            });

            connect(&_moveTabLeft, &QAction::triggered, [this]()
            {
                emit moveTabLeft();
            });

            connect(&_moveTabRight, &QAction::triggered, [this]()
            {
                emit moveTabRight();
            });

            connect(&deleteTextureButton, &QAction::triggered, [this]()
            {
                 emit deleteActionTriggered();
            });
        }

        void OptionsMenu::showDeleteAction(bool value)
        {
            if(value)
            {
                addAction(&deleteTextureButton);
            }
            else
            {
                removeAction(&deleteTextureButton);
            }
        }
    }
}