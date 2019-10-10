//
// Created by BinyBrion on 10/10/2019.
//

#include "OptionsMenu.h"

namespace GUI
{
    namespace LoadResults
    {
        OptionsMenu::OptionsMenu(QWidget *parent) : QMenu(parent)
        {
            moveTabLeft.setText("Move Tab Left");
            moveTabRight.setText("Move Tab Right");
            deleteTextureButton.setText("Delete texture button");

            addAction(&moveTabLeft);
            addAction(&moveTabRight);

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