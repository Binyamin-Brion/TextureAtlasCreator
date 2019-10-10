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
            optionsMenu = new OptionsMenu{this};

            addNewTab = new Dialogs::AddNewTab{this};

            chooseTexture = new Dialogs::ChooseTexture{this};

            connect(chooseTexture, SIGNAL(textureChosen(QString)), this, SLOT(openTexture(QString)));

            QString defaultTabName{"Default"};
            addNewTab->addNameExistingTab(defaultTabName);

            auto *defaultTabScrollArea = new ScrollArea;

            connect(defaultTabScrollArea->getTextureArea(), SIGNAL(addNewTabRequest()), this, SLOT(showAddTabDialog()));

            currentTabs.emplace_back(defaultTabScrollArea, defaultTabName);

            addTab(currentTabs.back().first, currentTabs.back().second);

            chooseTexture->addTab(currentTabs.back().second);

            this->setContextMenuPolicy(Qt::CustomContextMenu);

            connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showContextMenu(const QPoint&)));

            connect(optionsMenu, SIGNAL(addTabActionTriggered()), this, SLOT(showAddTabDialog()));

            connect(addNewTab, &Dialogs::AddNewTab::newTabNameChosen, [this](QString newTabName)
            {
                auto *defaultTabScrollArea = new ScrollArea;

                connect(defaultTabScrollArea->getTextureArea(), SIGNAL(addNewTabRequest()), this, SLOT(showAddTabDialog()));

                currentTabs.emplace_back(defaultTabScrollArea, newTabName);

                currentTabs.back().first->setTextureBankReference(textureBank);

                addTab(currentTabs.back().first, currentTabs.back().second);

                addNewTab->addNameExistingTab(newTabName);
            });
        }

        void LoadedTextures::setTextureBankReference(const TextureLogic::TextureBank *textureBank)
        {
            if(this->textureBank == nullptr)
            {
                this->textureBank = textureBank;

                currentTabs.front().first->setTextureBankReference(this->textureBank);
            }
        }

        void LoadedTextures::showAddTabDialog()
        {
            addNewTab->show();
        }

        void LoadedTextures::showContextMenu(const QPoint& pos)
        {
            optionsMenu->exec(mapToGlobal(pos));
        }

        void LoadedTextures::showLoadTextureDialog()
        {
            chooseTexture->show();
        }

        void LoadedTextures::openTexture(QString textureLocation)
        {
            currentTabs[currentIndex()].first->addTextureButton(textureLocation);
        }
    }
}