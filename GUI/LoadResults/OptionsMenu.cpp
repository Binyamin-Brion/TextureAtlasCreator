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
            moveTabLeft.setText("Move Tab Left");
            moveTabRight.setText("Move Tab Right");
            deleteTab.setText("Delete Current Tab");
            deleteTextureButton.setText("Delete Texture Button");

            addAction(&addTab);
            addAction(&moveTabLeft);
            addAction(&moveTabRight);
            addAction(&deleteTab);

            connect(&addTab, &QAction::triggered, [this]()
            {
                emit addTabActionTriggered();
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