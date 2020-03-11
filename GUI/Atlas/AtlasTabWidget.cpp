//
// Created by BinyBrion on 2019-09-16.
//

#include <QtWidgets/QMessageBox>
#include "AtlasTabWidget.h"
#include "TextureLogic/TextureBank.h"
#include "ScrollArea.h"

#include "AtlasTabOptionsMenu.h"
#include "GUI/Dialogs/AddNewTab.h"
#include "GUI/Dialogs/AddNewAtlasTab.h"

namespace GUI
{
    namespace Atlas
    {
        /*
         *  About the variable currentTabs:
         *
         *  Its use is not required. It was originally implemented it was not known that Qt supports switching the placement
         *  of tabs. This variable was used to help out with implementing a manual version of switching the order of tabs.
         *
         *  It could be removed, but it is likely not worth it. An attempt was made, but quickly a segfault came up.
         *  Considering how rarely tab order is changed, the current way- right clicking the currently viewed tab, and selecting
         *  to either move left or right- is sufficient.
         */

        // Beginning of public functions

        AtlasTabWidget::AtlasTabWidget(QWidget *parent)
                        :
                            QTabWidget{parent},
                            atlasTabOptionsMenu{new AtlasTabOptionsMenu{this}},
                            renameTab{new Dialogs::AddNewTab{this}},
                            addNewAtlasTab{new Dialogs::AddNewAtlasTab{this}}
        {
            this->setContextMenuPolicy(Qt::CustomContextMenu);

            // Code that sets up the logic to apply actions to the tabs that hold a texture atlas
            connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showContextMenu(const QPoint&)));

            // The following connect signals connect any action clicked in the context menu to the appropriate slot
            // so that the user request for the currently displayed tab can be processed

            connect(atlasTabOptionsMenu, SIGNAL(addTabActionTriggered()), this, SLOT(showAddNewAtlasTab()));

            connect(atlasTabOptionsMenu, SIGNAL(renameTabActionTriggered()), this, SLOT(showRenameTabDialog()));

            connect(atlasTabOptionsMenu, SIGNAL(moveTabLeftTriggered()), this, SLOT(moveTabLeft()));

            connect(atlasTabOptionsMenu, SIGNAL(moveTabRightTriggered()), this, SLOT(moveTabRight()));

            // Following connections ensure that the appropriate function is executed after a dialog was shown to the user after clicking on an action in the context menu

            connect(addNewAtlasTab, SIGNAL(newAtlasInformationSpecified(QString, QSize, QImage::Format)), this, SLOT(addAtlasWidget(const QString&, QSize, QImage::Format)));

            connect(renameTab, SIGNAL(newTabNameChosen(QString)), this, SLOT(newTabNameChosen(const QString&)));

            connect(atlasTabOptionsMenu, SIGNAL(deleteTabActionTriggered()), this, SLOT(deleteCurrentTab()));

            connect(this, SIGNAL(currentChanged(int)), this, SLOT(currentTabChanged(int)));

            addAtlasWidget("Default", QSize{1920, 1080}, QImage::Format_RGB32);
        }

        void AtlasTabWidget::addTextureToAtlas(const TextureLogic::Texture &texture, QString atlasName, QPoint position)
        {
            for(const auto &i : currentTabs)
            {
                if(i.second == atlasName)
                {
                    i.first->addTexture(texture, position);
                }
            }
        }

        void AtlasTabWidget::closeAllTabs()
        {
            for(int i = 0; i < currentTabs.size(); ++i)
            {
                deleteCurrentTab();
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

        QImage::Format AtlasTabWidget::getCurrentAtlasFormat() const
        {
            return currentTabs[currentIndex()].first->getAtlasFormat();
        }

        ::Atlas::AtlasInformationBundle AtlasTabWidget::getCurrentAtlasInformation() const
        {
            return currentTabs[currentIndex()].first->getCurrentAtlasInformation();
        }

        QString AtlasTabWidget::getCurrentAtlasName() const
        {
            return currentTabs[currentIndex()].second;
        }

        bool AtlasTabWidget::getUnsavedAtlases() const
        {
            bool unsavedAtlases = false;

            for(const auto &i : currentTabs)
            {
                unsavedAtlases |= i.first->getUnsavedChanges();
            }

            return unsavedAtlases;
        }

        void AtlasTabWidget::removeTexture(const TextureLogic::Texture *texture)
        {
            for(auto &i : currentTabs)
            {
                i.first->removeTexture(texture);
            }
        }

        void AtlasTabWidget::saveAtlases(const QString &saveLocation) const
        {
            for(const auto &i : currentTabs)
            {
                i.first->saveAtlas(i.second, saveLocation);
            }
        }

        bool AtlasTabWidget::setIntersectionWidth(TextureLogic::Texture *texture)
        {
            // When a new intersection width is specified for a texture, that new intersection width is checked
            // to see if it is valid in all of the texture atlases that use that texture. If there is a problem with
            // the new width, the user has to be notified with an appropriate error message.
            std::vector<QString> atlasNames;

            // This holds the result of checking all the texture atlases.
            // If one or more atlases state the texture cannot have the selected texture intersection width change,
            // this variable is true
            bool borderWidthChangeFailed = false;

            for(auto &i : currentTabs)
            {
                bool result = i.first->newIntersectionBorderWidthValid(texture);

                if(result)
                {
                    atlasNames.push_back(i.second);
                }

                // Bitwise operation needed so that the most recent check does not override the previous checks
                borderWidthChangeFailed |= result;
            }

            if(!atlasNames.empty())
            {
               QString errorMessage{"The new border width leads to intersection in the follows atlases: "};

               for(const auto &i : atlasNames)
               {
                   errorMessage += i + ", ";
               }

               errorMessage.chop(1);

               QMessageBox::warning(this, tr("Error: Invalid New Intersection Border Width"), errorMessage.toStdString().c_str(), QMessageBox::Ok);
            }

            return borderWidthChangeFailed;
        }

        void AtlasTabWidget::setTextureBankReference(TextureLogic::TextureBank *textureBank)
        {
            if(textureBank != nullptr)
            {
                this->textureBank = textureBank;

                for(auto &i : currentTabs)
                {
                    i.first->setTextureBankReference(this->textureBank);
                }
            }
        }

        void AtlasTabWidget::updateTextureReferences(AccessRestriction::PassKey<TextureLogic::TextureBank>)
        {
            for(auto &i : currentTabs)
            {
                i.first->updateTextureReferences();
            }
        }

        // Beginning of public slots

        void AtlasTabWidget::exportTexture()
        {
            currentTabs[currentIndex()].first->exportTexture();
        }

        // Beginning of private slots

        void AtlasTabWidget::addAtlasWidget(const QString &tabName, QSize atlasSize, QImage::Format atlasFormat)
        {
            auto *tabScrollArea = new ScrollArea{atlasSize, atlasFormat, this};

            tabScrollArea->setTextureBankReference(textureBank);

            currentTabs.emplace_back(tabScrollArea, tabName);

            addTab(currentTabs.back().first, currentTabs.back().second);

            // The name for the tab was checked to make sure it was unique before this function (addAtlasWidget()) was called

            addNewAtlasTab->addExistingTabName(tabName);

            renameTab->addNameExistingTab(tabName);

            // Make sure that if the just added atlas changes in any way, the statistics are displayed
            connect(tabScrollArea, &ScrollArea::currentAtlasInformationChanged, [this](::Atlas::AtlasInformationBundle atlasInformation)
            {
                emit currentAtlasInformationChanged(atlasInformation);
            });
        }

        void AtlasTabWidget::currentTabChanged(int index)
        {
            // If the last tab is deleted, and before a new one is set, this lambda is called when the currentIndex is -1.
            // This leads to a segfault due to an invalid index. Thus a check against an invalid currentIndex has to be performed.
            if(index == -1)
            {
                return;
            }

            // Variable just used to increase readability
            const ScrollArea *const scrollArea = currentTabs[index].first;

            emit currentAtlasInformationChanged(scrollArea->getAtlasInformation());
        }

        void AtlasTabWidget::deleteCurrentTab()
        {
            // The user should not be able to delete a tab that would cause the currentIndex() to be -1, as this indicates
            // that there is no current tab. This should not happen as if there is no tab, then a default one is placed,
            // but the user theoretically may be quick enough to ask to delete a tab before the default one is placed.
            // Placing a check here ensures the program does not crash in such a situation.
            if(currentIndex() != -1)
            {
                // Remember there is a one to one correspondence between the order of the tabs shown and the
                // order tabs stored internally. For example, the first tab shown is stored at the beginning of
                // the currentTabs variable, the second tab at the second spot in the currentTab variable, and so on
                int previousIndex = currentIndex();

                textureBank->textureSelected(nullptr);

                removeTab(currentIndex());

                delete currentTabs[previousIndex].first;

                addNewAtlasTab->removeNameExistingTab(currentTabs[previousIndex].second);

                renameTab->removeNameExistingTab(currentTabs[previousIndex].second);

                currentTabs.erase(currentTabs.begin() + previousIndex);

                // Make sure that there is always at least one tab shown, even if the last one was previous deleted
                if(currentTabs.empty())
                {
                    addAtlasWidget("Default", QSize{1920, 1080}, QImage::Format_RGB32);
                }
            }
        }

        void AtlasTabWidget::moveTabLeft()
        {
            // The left most tab cannot be moved left
            if(currentIndex() == 0)
            {
                return;
            }

            // Remove the current tab and place it in a position to the left
            int previousIndex = currentIndex();

            removeTab(previousIndex);

            insertTab(previousIndex - 1, currentTabs[previousIndex].first, currentTabs[previousIndex].second);

            // Swap done to keep one-to-one correspondence between the order of tabs shown visually and the order the tabs are stored internally
            std::swap(currentTabs[previousIndex], currentTabs[previousIndex - 1]);

            // Make sure that the tab that was moved left is still the one being displayed
            setCurrentIndex(previousIndex - 1);
        }

        void AtlasTabWidget::moveTabRight()
        {
            // There will realistically not be more than the max value int number of tabs, so this cast is safe,
            // and removes a warning between between signed and unsigned comparisons
            if(currentIndex() == static_cast<int>(currentTabs.size() - 1))
            {
                return;
            }

            // Order of operations same as moveTabLeftTriggered()
            int previousIndex = currentIndex();

            removeTab(currentIndex());

            insertTab(previousIndex + 1, currentTabs[previousIndex].first, currentTabs[previousIndex].second);

            // Swap done to keep one-to-one correspondence between the order of tabs shown visually and the order the tabs are stored internally
            std::swap(currentTabs[previousIndex], currentTabs[previousIndex + 1]);

            setCurrentIndex(previousIndex + 1);
        }

        void AtlasTabWidget::newTabNameChosen(const QString &newTabName)
        {
            // When renaming a tab, there is no direct way to rename a tab. As such, in order to rename a tab,
            // the old tab has to be removed and a new tab with the new name has to be inserted.

            // Keep track of the tab index of the tab being removed as this will change after the tab is removed.
            // Keeping track of this eases the logic of being the operations required to rename a tab.
            int previousIndex = currentIndex();

            removeTab(previousIndex);

            addNewAtlasTab->removeNameExistingTab(currentTabs[previousIndex].second);

            renameTab->removeNameExistingTab(currentTabs[previousIndex].second);

            currentTabs[previousIndex].second = newTabName;

            insertTab(previousIndex, currentTabs[previousIndex].first, currentTabs[previousIndex].second);

            renameTab->addNameExistingTab(newTabName);

            addNewAtlasTab->addExistingTabName(newTabName);

            setCurrentIndex(previousIndex);
        }

        void AtlasTabWidget::repaintSelectedTexture()
        {
            currentTabs[currentIndex()].first->repaintSelectedTexture();
        }

        void AtlasTabWidget::showAddNewAtlasTab()
        {
            addNewAtlasTab->show();
        }

        void AtlasTabWidget::showContextMenu(const QPoint &pos)
        {
            atlasTabOptionsMenu->exec(mapToGlobal(pos));
        }

        void AtlasTabWidget::showRenameTabDialog()
        {
            renameTab->open();
        }
    }
}