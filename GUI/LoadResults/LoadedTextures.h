//
// Created by BinyBrion on 2019-09-15.
//

#ifndef TEXTUREATLASCREATOR_LOADEDTEXTURES_H
#define TEXTUREATLASCREATOR_LOADEDTEXTURES_H

#include <QtWidgets/QTabWidget>
#include "ScrollArea.h"

namespace TextureLogic
{
    class TextureBank;
}

namespace GUI
{
    namespace Dialogs
    {
        class AddNewTab;
        class ChooseTexture;
    }

    namespace LoadResults
    {
        class OptionsMenu;
        class ScrollArea;

        /*
         *  Holds all of the TextureButtonsArea.
         */

        class LoadedTextures : public QTabWidget
        {
            Q_OBJECT

            public:
                explicit LoadedTextures(QWidget *parent = nullptr);
                void setTextureBankReference(const TextureLogic::TextureBank *textureBank);

            public slots:
                void showAddTabDialog();
                void showContextMenu(const QPoint &pos);
                void showLoadTextureDialog();
                void showRenameTabDialog();

            private slots:
                void openTexture(QString textureLocation, unsigned int intersectionBorderWidth, unsigned int selectionBorderWidth);
                void moveTabLeft();
                void moveTabRight();
                void deleteCurrentTab();

            private:
                void addTextureButtonArea(const QString &tabName);

                std::vector<std::pair<GUI::LoadResults::ScrollArea*, QString>> currentTabs;
                const TextureLogic::TextureBank *textureBank = nullptr;
                Dialogs::AddNewTab *addNewTab = nullptr;
                bool renameTab = false;
                Dialogs::ChooseTexture *chooseTexture = nullptr;

                OptionsMenu *optionsMenu = nullptr;
        };
    }
}

#endif //TEXTUREATLASCREATOR_LOADEDTEXTURES_H