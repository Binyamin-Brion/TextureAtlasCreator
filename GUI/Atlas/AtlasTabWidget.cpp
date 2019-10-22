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

            connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showContextMenu(const QPoint&)));

            atlasTabOptionsMenu = new AtlasTabOptionsMenu{this};

            connect(atlasTabOptionsMenu, SIGNAL(addTabActionTriggered()), this, SLOT(showAddNewAtlasTab()));

            connect(atlasTabOptionsMenu, SIGNAL(renameTabActionTriggered()), this, SLOT(showRenameTabDialog()));

            connect(atlasTabOptionsMenu, SIGNAL(moveTabLeft()), this, SLOT(moveTabLeft()));

            connect(atlasTabOptionsMenu, SIGNAL(moveTabRight()), this, SLOT(moveTabRight()));

            renameTab = new Dialogs::AddNewTab{this};

            addNewAtlasTab = new Dialogs::AddNewAtlasTab{this};

            addNewAtlasTab->addExistingTabName("Default");

            connect(addNewAtlasTab, &Dialogs::AddNewAtlasTab::newAtlasInformationSpecified, [this]
                   (QString newTabName, int requestedWidth, int requestedHeight, QImage::Format atlasFormat)
            {
                addAtlasWidget(newTabName, QSize{requestedWidth, requestedHeight}, atlasFormat);
            });

            connect(renameTab, &Dialogs::AddNewTab::newTabNameChosen, [this](QString newTabName)
            {
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
                    textureBank->textureSelected(nullptr);

                    removeTab(currentIndex());

                    delete currentTabs[currentIndex() + 1].first;

                    renameTab->removeNameExistingTab(currentTabs[currentIndex() + 1].second);

                    currentTabs.erase(currentTabs.begin() + currentIndex() + 1);

                    if(currentTabs.empty())
                    {
                        addAtlasWidget("Default", QSize{1920, 1080}, QImage::Format_RGB32);
                    }
                }
            });

            connect(this, &QTabWidget::currentChanged, [this](int index)
            {
                if(index == -1)
                {
                    return;
                }

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

        void AtlasTabWidget::updateTextureReferences(AccessRestriction::PassKey<TextureLogic::TextureBank>)
        {
            for(auto &i : currentTabs)
            {
                i.first->updateTextureReferences(textureBank->getTextures());
            }
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