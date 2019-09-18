//
// Created by BinyBrion on 2019-09-15.
//

#include "LoadedTextures.h"

#include "ScrollArea.h"
#include "GUI/Dialogs/ChooseTexture.h"

namespace GUI
{
    namespace LoadResults
    {
        LoadedTextures::LoadedTextures(QWidget *parent) : QTabWidget{parent}
        {
            chooseTexture = new Dialogs::ChooseTexture{this};

            connect(chooseTexture, SIGNAL(textureChosen(QString)), this, SLOT(openTexture(QString)));

            QString defaultTabName{"Default"};

            auto *defaultTabScrollArea = new ScrollArea;

            currentTabs.emplace_back(defaultTabScrollArea, defaultTabName);

            addTab(currentTabs.back().first, currentTabs.back().second);

            chooseTexture->addTab(currentTabs.back().second);
        }

        void LoadedTextures::setTextureBankReference(const TextureLogic::TextureBank *textureBank)
        {
            if(this->textureBank == nullptr)
            {
                this->textureBank = textureBank;

                currentTabs.front().first->setTextureBankReference(this->textureBank);
            }
        }

        void LoadedTextures::showLoadTextureDialog()
        {
            chooseTexture->show();
        }

        void LoadedTextures::openTexture(QString textureLocation)
        {
            printf("%s \n", textureLocation.toStdString().c_str());

            currentTabs[currentIndex()].first->addTextureButton(textureLocation);
        }
    }
}