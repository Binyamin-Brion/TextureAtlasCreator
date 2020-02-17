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
        AtlasTabWidget::AtlasTabWidget(QWidget *parent) : QTabWidget{parent}
        {
            this->setContextMenuPolicy(Qt::CustomContextMenu);

            // Code that sets up the logic to apply actions to the tabs that hold a texture atlas

            connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showContextMenu(const QPoint&)));

            atlasTabOptionsMenu = new AtlasTabOptionsMenu{this};

            connect(atlasTabOptionsMenu, SIGNAL(addTabActionTriggered()), this, SLOT(showAddNewAtlasTab()));

            connect(atlasTabOptionsMenu, SIGNAL(renameTabActionTriggered()), this, SLOT(showRenameTabDialog()));

            connect(atlasTabOptionsMenu, SIGNAL(moveTabLeft()), this, SLOT(moveTabLeft()));

            connect(atlasTabOptionsMenu, SIGNAL(moveTabRight()), this, SLOT(moveTabRight()));

            renameTab = new Dialogs::AddNewTab{this};

            addNewAtlasTab = new Dialogs::AddNewAtlasTab{this};

            // By default there is one tab created, which holds an empty texture atlas.
            // When this is done, the default name for this tab has to be stored so that other
            // tabs do not have the same name. The tab with this default name is created at the end of
            // this constructor.

            addNewAtlasTab->addExistingTabName("Default");

            connect(addNewAtlasTab, &Dialogs::AddNewAtlasTab::newAtlasInformationSpecified, [this]
                   (QString newTabName, int requestedWidth, int requestedHeight, QImage::Format atlasFormat)
            {
                addAtlasWidget(newTabName, QSize{requestedWidth, requestedHeight}, atlasFormat);
            });

            // When renaming a tab, there is no direct way to renane a tab. As such, in order to rename a tab,
            // the old tab has to be removed and a new tab with the new name has to be inserted.

            connect(renameTab, &Dialogs::AddNewTab::newTabNameChosen, [this](QString newTabName)
            {
                // Keep track of the tab index of the tab being removed as this will change after the tab is removed.
                // Keeping track of this eases the logic of being the operations required to rename a tab.

                int previousIndex = currentIndex();

                removeTab(previousIndex);

                renameTab->removeNameExistingTab(currentTabs[previousIndex].second);

                currentTabs[previousIndex].second = newTabName;

                insertTab(previousIndex, currentTabs[previousIndex].first, currentTabs[previousIndex].second);

                renameTab->addNameExistingTab(newTabName);

                setCurrentIndex(previousIndex);
            });

            connect(atlasTabOptionsMenu, &AtlasTabOptionsMenu::deleteTabActionTriggered, [this]()
            {
                if(currentIndex() != -1)
                {
                    // Remember there is a one to one correspondence between the order of the tabs shown and the
                    // order tabs stored internally. For example, the first tab shown is stored at the beginning of
                    // the currentTabs variable, the second tab at the second spot in the currentTab variable, and so on

                    int previousIndex = currentIndex();

                    textureBank->textureSelected(nullptr);

                    removeTab(currentIndex());

                    delete currentTabs[previousIndex].first;

                    renameTab->removeNameExistingTab(currentTabs[previousIndex].second);

                    currentTabs.erase(currentTabs.begin() + previousIndex);

                    // Make sure that there is always at least one tab shown, even if the last one was previous deleted

                    if(currentTabs.empty())
                    {
                        addAtlasWidget("Default", QSize{1920, 1080}, QImage::Format_RGB32);
                    }
                }
            });

            connect(this, &QTabWidget::currentChanged, [this](int index)
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
            });

            addAtlasWidget("Default", QSize{1920, 1080}, QImage::Format_RGB32);
        }

        void AtlasTabWidget::addTextureToCurrentAtlas(const TextureLogic::Texture &texture)
        {
            currentTabs[currentIndex()].first->addTexture(texture);
        }

        QImage::Format AtlasTabWidget::getCurrentAtlasFormat() const
        {
            return currentTabs[currentIndex()].first->getAtlasFormat();
        }

        void AtlasTabWidget::removeTexture(const TextureLogic::Texture *texture)
        {
            for(auto &i : currentTabs)
            {
                i.first->removeTexture(texture);
            }
        }

        bool AtlasTabWidget::setIntersectionWidth(TextureLogic::Texture *texture)
        {
            // When a new intersection width is specified for a texture, that new intersection width is checked
            // to see if it is valid in all of the texture atlases that use that texture. If there is a problem with
            // the new width, the user has to be notified with an appropriate error message.

            std::vector<QString> atlasNames;

            bool borderWidthChangeFailed = false;

            for(auto &i : currentTabs)
            {
                bool result = i.first->setIntersectionWidth(texture);

                if(result)
                {
                    atlasNames.push_back(i.second);
                }

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

               QMessageBox::warning(this, tr("Error: Invalid New Intersection Border Wdith"), errorMessage.toStdString().c_str(), QMessageBox::Ok);
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

        // Called when the vector holding all of the texture references has been added to, and so previous references
        // to the contents within that vector may no longer be valid

        void AtlasTabWidget::updateTextureReferences(AccessRestriction::PassKey<TextureLogic::TextureBank>)
        {
            for(auto &i : currentTabs)
            {
                i.first->updateTextureReferences();
            }
        }

        void AtlasTabWidget::exportTexture()
        {
            currentTabs[currentIndex()].first->exportTexture();
        }

        void AtlasTabWidget::moveTabLeft()
        {
            if(currentIndex() == 0)
            {
                return;
            }

            int previousIndex = currentIndex();

            removeTab(previousIndex);

            insertTab(previousIndex - 1, currentTabs[previousIndex].first, currentTabs[previousIndex].second);

            // Swap done to keep one-to-one correspondence between the order of tabs shown visually and the order the tabs are stored internally

            std::swap(currentTabs[previousIndex], currentTabs[previousIndex - 1]);

            setCurrentIndex(previousIndex - 1);
        }

        void AtlasTabWidget::moveTabRight()
        {
            if(currentIndex() == currentTabs.size() - 1)
            {
                return;
            }

            int previousIndex = currentIndex();

            removeTab(currentIndex());

            insertTab(previousIndex + 1, currentTabs[previousIndex].first, currentTabs[previousIndex].second);

            // Swap done to keep one-to-one correspondence between the order of tabs shown visually and the order the tabs are stored internally

            std::swap(currentTabs[previousIndex], currentTabs[previousIndex + 1]);

            setCurrentIndex(previousIndex + 1);
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

        void AtlasTabWidget::addAtlasWidget(const QString &tabName, QSize atlasSize, QImage::Format atlasFormat)
        {
            auto *tabScrollArea = new ScrollArea{atlasSize, atlasFormat, this};

            tabScrollArea->setTextureBankReference(textureBank);

            currentTabs.emplace_back(tabScrollArea, tabName);

            addTab(currentTabs.back().first, currentTabs.back().second);

            renameTab->addNameExistingTab(tabName);

            connect(tabScrollArea, &ScrollArea::currentAtlasInformationChanged, [this](::Atlas::AtlasInformationBundle atlasInformation)
            {
                emit currentAtlasInformationChanged(atlasInformation);
            });
        }
    }
}