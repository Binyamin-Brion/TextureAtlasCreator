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
        // For the moveTabXXX and the deleteCurrentTab functions, see the AtlasTabWidget.cpp file for description of how
        // these functions work. That class uses the same function it implement the same behaviour. Refer to that class
        // for comments on how these functions work.

        // Beginning of public functions

        LoadedTextures::LoadedTextures(QWidget *parent)
                        :
                            QTabWidget{parent}
        {
            // This widget cannot tell a button area to delete a texture button area, to provide a clear separation of
            // who is responsible for the buttons. Hence the first argument to the optionsMenu is false.
            // See OptionsMenu class for notes.
            optionsMenu = new OptionsMenu{false, true,true, this};

            addNewTab = new Dialogs::AddNewTab{this};

            chooseTexture = new Dialogs::ChooseTexture{this};

            connect(chooseTexture, SIGNAL(textureChosen(QString, QString, unsigned int, unsigned int)), this, SLOT(openTexture(QString, QString, unsigned int, unsigned int)));

            // By default there is always at least one tab to hold texture buttons
            addTextureButtonArea("Default");

            // Set up the code so that the context menu with valid actions are shown and that when an action is requested
            // the appropriate code is executed. This is only required to get the access menu for the tab itself; not the tab contents
            this->setContextMenuPolicy(Qt::CustomContextMenu);

            // These connections are for the tab widget itself; not the contents within

            connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showContextMenu(const QPoint&)));

            connect(optionsMenu, SIGNAL(addTabActionTriggered()), this, SLOT(showAddTabDialog()));

            connect(optionsMenu, SIGNAL(renameTabActionTriggered()), this, SLOT(showRenameTabDialog()));

            connect(optionsMenu, SIGNAL(moveTabLeftTriggered()), this, SLOT(moveTabLeft()));

            connect(optionsMenu, SIGNAL(moveTabRightTriggered()), this, SLOT(moveTabRight()));

            // The logic for renaming and deleting a tab is the same as that of AtlasTabWidget. Refer to that class for more information.

            connect(optionsMenu, SIGNAL(deleteTabTriggered(bool)), this, SLOT(deleteCurrentTab(bool)));

            connect(addNewTab, SIGNAL(newTabNameChosen(QString)), this, SLOT(newTabNameChosen(QString)));

        }

        void LoadedTextures::addTextureButtonArea(const QString &tabName)
        {
            auto *scrollArea = new ScrollArea;

            scrollArea->setTextureBankReference(textureBank);

            // These connections are for the tab contents, unlike the connections in the constructor

            connect(scrollArea->getTextureArea(), SIGNAL(renameTabRequest()), this, SLOT(showRenameTabDialog()));

            connect(scrollArea->getTextureArea(), SIGNAL(moveTabLeftTriggered()), this, SLOT(moveTabLeft()));

            connect(scrollArea->getTextureArea(), SIGNAL(moveTabRightTriggered()), this, SLOT(moveTabRight()));

            currentTabs.emplace_back(scrollArea, tabName);

            addTab(currentTabs.back().first, currentTabs.back().second);

            addNewTab->addNameExistingTab(tabName);

            chooseTexture->addTab(currentTabs.back().second);
        }

        void LoadedTextures::closeAllTabs()
        {
            for(unsigned int i = 0; i < currentTabs.size(); ++i)
            {
                deleteCurrentTab(true);
            }

            // Deleting all of the tabs in the above loop will put in a default tab, which is not required as this function
            // is called when a project is to be opened. When that happens, there will be tabs to be placed.

            if(!currentTabs.empty())
            {
                removeTab(0);

                delete currentTabs.front().first;
                currentTabs.clear();
            }
        }

        bool LoadedTextures::getUnsavedTextureButtonAreas() const
        {
            bool unsavedButtonAreas = false;

            for(const auto &i : currentTabs)
            {
                unsavedButtonAreas |= i.first->getUnsavedChanges();
            }

            return unsavedButtonAreas;
        }

        void LoadedTextures::saveLoadedTextures(const QString &saveLocation) const
        {
            for(const auto &i : currentTabs)
            {
                i.first->saveLoadedTextures(i.second, saveLocation);
            }
        }

        void LoadedTextures::setTextureBankReference(TextureLogic::TextureBank *textureBank)
        {
            // This should only be called once; the reference to the texture bank should never become invalid
            if(this->textureBank == nullptr)
            {
                this->textureBank = textureBank;

                // If there are tabs loaded when this is called (and there will always be at least one, as if none are specified by the user
                // then a default tab is given) then make sure they all have a reference to the texture bank
                for(auto &i : currentTabs)
                {
                    i.first->setTextureBankReference(this->textureBank);
                }
            }
        }

        // Beginning of public slots

        void LoadedTextures::showAddTabDialog()
        {
            // Since the dialog is used for both adding and renaming a dialog, the window title should be changed
            // depending on what the dialog is being used for
            addNewTab->setWindowTitle("Add New Tab");

            // Mark what the dialog that is about to be shown is being used for
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
            // Since the dialog is used for both adding and renaming a dialog, the window title should be changed
            // depending on what the dialog is being used for
            addNewTab->setWindowTitle("Rename Current Tab");

            // Mark what the dialog that is about to be shown is being used for
            renameTab = true;

            addNewTab->show();
        }

        void LoadedTextures::deleteCurrentTab(bool closingProgram)
        {
            // See note at the top of this file

            if(currentIndex() != -1)
            {
                int previousIndex = currentIndex();

                currentTabs[currentIndex()].first->deleteTextureButtons(closingProgram);

                chooseTexture->removeTab(currentTabs[previousIndex].second);

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

        // Beginning of private slots

        void LoadedTextures::moveTabLeft()
        {
            // See note at the top of this file

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
            // See note at the top of this file

            // Realistically the maximum number of tabs will not exceed max value of int, and the cast removes a warning
            // that can be distracting and cause other warnings to be missed
            if(currentIndex() == static_cast<int>(currentTabs.size() - 1))
            {
                return;
            }

            int previousIndex = currentIndex();

            removeTab(currentIndex());

            insertTab(previousIndex + 1, currentTabs[previousIndex].first, currentTabs[previousIndex].second);

            std::swap(currentTabs[previousIndex], currentTabs[previousIndex + 1]);

            setCurrentIndex(previousIndex + 1);
        }

        void LoadedTextures::newTabNameChosen(QString newTabName)
        {
            // Remember that depending on the action selected in the context menu, the result of the AddNewTab dialog changes

            if(renameTab)
            {
                // In order to rename a tab, the current tab has to be removed and inserted back in with the new name

                int previousIndex = currentIndex();

                removeTab(currentIndex());

                addNewTab->removeNameExistingTab(currentTabs[previousIndex].second);

                chooseTexture->removeTab(currentTabs[previousIndex].second);

                currentTabs[previousIndex].second = newTabName;

                insertTab(previousIndex, currentTabs[previousIndex].first, currentTabs[previousIndex].second);

                addNewTab->addNameExistingTab(newTabName);

                setCurrentIndex(previousIndex);

                chooseTexture->addTab(newTabName);
            }
            else
            {
                addTextureButtonArea(newTabName);
            }
        }

        void LoadedTextures::openTexture(QString textureButtonAreaName, QString textureLocation, unsigned int intersectionBorderWidth, unsigned int selectionBorderWidth)
        {
            // Remember that the user can choose into what button area to load a texture button into; therefore that
            // area has to be found first before it can be added there
            for(auto &i : currentTabs)
            {
                if(i.second == textureButtonAreaName)
                {
                    i.first->addTextureButton(textureLocation, intersectionBorderWidth, selectionBorderWidth);

                    return;
                }
            }

            Q_ASSERT_X(true, __PRETTY_FUNCTION__, "\n Unable to find tab requested in which to place texture button\n");
        }
    }
}