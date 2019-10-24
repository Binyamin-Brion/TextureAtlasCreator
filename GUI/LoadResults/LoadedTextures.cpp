//
// Created by BinyBrion on 2019-09-15.
//

#include "LoadedTextures.h"
#include "OptionsMenu.h"

#include "TextureButtonArea.h"
#include "ScrollArea.h"
#include "GUI/Dialogs/ChooseTexture.h"
#include "GUI/Dialogs/AddNewTab.h"
#include <QMouseEvent>

namespace GUI
{
    namespace LoadResults
    {
        LoadedTextures::LoadedTextures(QWidget *parent) : QTabWidget{parent}
        {
            // This widget cannot tell a button area to delete a texture button area, to provide a clear separation of
            // who is responsible for the buttons. Hence the first argument to the optionsMenu is false.

            optionsMenu = new OptionsMenu{false, true, this};

            addNewTab = new Dialogs::AddNewTab{this};

            chooseTexture = new Dialogs::ChooseTexture{this};

            connect(chooseTexture, SIGNAL(textureChosen(QString, unsigned int, unsigned int)), this, SLOT(openTexture(QString, unsigned int, unsigned int)));

            addTextureButtonArea("Default");

            // Set up the code so that the context menu with valid actions are shown and that when an action is requested
            // the appropriate code is executed

            this->setContextMenuPolicy(Qt::CustomContextMenu);

            connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showContextMenu(const QPoint&)));

            connect(optionsMenu, SIGNAL(addTabActionTriggered()), this, SLOT(showAddTabDialog()));

            connect(optionsMenu, SIGNAL(renameTabActionTriggered()), this, SLOT(showRenameTabDialog()));

            connect(optionsMenu, SIGNAL(moveTabLeft()), this, SLOT(moveTabLeft()));

            connect(optionsMenu, SIGNAL(moveTabRight()), this, SLOT(moveTabRight()));

            connect(optionsMenu, SIGNAL(deleteTabTriggered()), this, SLOT(deleteCurrentTab()));

            // The logic for renaming and deleting a tab is the same as that of AtlasTabWidget. Refer to that class
            // for more information.

            connect(addNewTab, &Dialogs::AddNewTab::newTabNameChosen, [this](QString newTabName)
            {
                if(renameTab)
                {
                    int previousIndex = currentIndex();

                    removeTab(currentIndex());

                    addNewTab->removeNameExistingTab(currentTabs[previousIndex].second);

                    currentTabs[previousIndex].second = newTabName;

                    insertTab(previousIndex, currentTabs[previousIndex].first, currentTabs[previousIndex].second);

                    addNewTab->addNameExistingTab(newTabName);

                    setCurrentIndex(previousIndex);
                }
                else
                {
                    addTextureButtonArea(newTabName);
                }
            });
        }

        void LoadedTextures::setTextureBankReference(const TextureLogic::TextureBank *textureBank)
        {
            if(this->textureBank == nullptr)
            {
                this->textureBank = textureBank;

                for(auto &i : currentTabs)
                {
                    i.first->setTextureBankReference(this->textureBank);
                }
            }
        }

        void LoadedTextures::showAddTabDialog()
        {
            addNewTab->setWindowTitle("Add New Tab");

            renameTab = false;

            addNewTab->open();
        }

        void LoadedTextures::showContextMenu(const QPoint& pos)
        {
            optionsMenu->exec(mapToGlobal(pos));
        }

        void LoadedTextures::showLoadTextureDialog()
        {
            chooseTexture->show();
        }

        void LoadedTextures::showRenameTabDialog()
        {
            addNewTab->setWindowTitle("Rename Current Tab");

            renameTab = true;

            addNewTab->show();
        }

        void LoadedTextures::openTexture(QString textureLocation, unsigned int intersectionBorderWidth, unsigned int selectionBorderWidth)
        {
            currentTabs[currentIndex()].first->addTextureButton(textureLocation, intersectionBorderWidth, selectionBorderWidth);
        }

        void LoadedTextures::moveTabLeft()
        {
            if(currentIndex() == 0)
            {
                return;
            }

            int previousIndex = currentIndex();

            removeTab(previousIndex);

            insertTab(previousIndex - 1, currentTabs[previousIndex].first, currentTabs[previousIndex].second);

            std::swap(currentTabs[previousIndex], currentTabs[previousIndex - 1]);

            setCurrentIndex(previousIndex - 1);
        }

        void LoadedTextures::moveTabRight()
        {
            if(currentIndex() == currentTabs.size() - 1)
            {
                return;
            }

            int previousIndex = currentIndex();

            removeTab(currentIndex());

            insertTab(previousIndex + 1, currentTabs[previousIndex].first, currentTabs[previousIndex].second);

            std::swap(currentTabs[previousIndex], currentTabs[previousIndex + 1]);

            setCurrentIndex(previousIndex + 1);
        }

        void LoadedTextures::deleteCurrentTab()
        {
            if(currentIndex() != -1)
            {
                int previousIndex = currentIndex();

                currentTabs[currentIndex()].first->deleteTextureButtons();

                removeTab(currentIndex());

                delete currentTabs[previousIndex].first;

                addNewTab->removeNameExistingTab(currentTabs[previousIndex].second);

                currentTabs.erase(currentTabs.begin() + previousIndex);

                if(currentTabs.empty())
                {
                    addTextureButtonArea("Default");
                }
            }
        }

        void LoadedTextures::addTextureButtonArea(const QString &tabName)
        {
            auto *scrollArea = new ScrollArea;

            scrollArea->setTextureBankReference(textureBank);

            connect(scrollArea->getTextureArea(), SIGNAL(addNewTabRequest()), this, SLOT(showAddTabDialog()));

            connect(scrollArea->getTextureArea(), SIGNAL(renameTabRequest()), this, SLOT(showRenameTabDialog()));

            connect(scrollArea->getTextureArea(), SIGNAL(moveTabLeft()), this, SLOT(moveTabLeft()));

            connect(scrollArea->getTextureArea(), SIGNAL(moveTabRight()), this, SLOT(moveTabRight()));

            currentTabs.emplace_back(scrollArea, tabName);

            addTab(currentTabs.back().first, currentTabs.back().second);

            addNewTab->addNameExistingTab(tabName);

            chooseTexture->addTab(currentTabs.back().second);
        }
    }
}