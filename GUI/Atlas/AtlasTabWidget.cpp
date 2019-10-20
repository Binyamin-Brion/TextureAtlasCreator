//
// Created by BinyBrion on 2019-09-16.
//

#include <QtWidgets/QMessageBox>
#include "AtlasTabWidget.h"
#include "TextureLogic/TextureBank.h"
#include "ScrollArea.h"

#include "AtlasTabOptionsMenu.h"
#include "GUI/Dialogs/AddNewAtlasTab.h"

namespace GUI
{
    namespace Atlas
    {
        AtlasTabWidget::AtlasTabWidget(QWidget *parent) : QTabWidget{parent}
        {
            addAtlasWidget("Default", QSize{1920, 1080}, QImage::Format_RGB32);

            this->setContextMenuPolicy(Qt::CustomContextMenu);

            connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showContextMenu(const QPoint&)));

            atlasTabOptionsMenu = new AtlasTabOptionsMenu{this};

            connect(atlasTabOptionsMenu, SIGNAL(addTabActionTriggered()), this, SLOT(showAddNewAtlasTab()));

            addNewAtlasTab = new Dialogs::AddNewAtlasTab{this};

            addNewAtlasTab->addExistingTabName("Default");

            connect(addNewAtlasTab, &Dialogs::AddNewAtlasTab::newAtlasInformationSpecified, [this]
                   (QString newTabName, int requestedWidth, int requestedHeight, QImage::Format atlasFormat)
            {
                addAtlasWidget(newTabName, QSize{requestedWidth, requestedHeight}, atlasFormat);
            });

            connect(this, &QTabWidget::currentChanged, [this](int index)
            {
                const ScrollArea *const scrollArea = currentTabs[index].first;

                emit currentAtlasInformationChanged(scrollArea->getAtlasInformation());
            });
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

        void AtlasTabWidget::addAtlasWidget(const QString &tabName, QSize atlasSize, QImage::Format atlasFormat)
        {
            auto *tabScrollArea = new ScrollArea{atlasSize, atlasFormat, this};

            tabScrollArea->setTextureBankReference(textureBank);

            currentTabs.emplace_back(tabScrollArea, tabName);

            addTab(currentTabs.back().first, currentTabs.back().second);

            connect(tabScrollArea, &ScrollArea::currentAtlasInformationChanged, [this](::Atlas::AtlasInformationBundle atlasInformation)
            {
                emit currentAtlasInformationChanged(atlasInformation);
            });
        }
    }
}