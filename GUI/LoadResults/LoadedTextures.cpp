//
// Created by BinyBrion on 2019-09-15.
//

#include "LoadedTextures.h"

#include "ScrollArea.h"

namespace GUI
{
    namespace LoadResults
    {
        LoadedTextures::LoadedTextures(QWidget *parent) : QTabWidget{parent}
        {
            QString defaultTabName{"Default"};

            auto *defaultTabScrollArea = new ScrollArea;

            currentTabs.emplace_back(defaultTabScrollArea, defaultTabName);

            addTab(currentTabs.back().first, currentTabs.back().second);
        }

        void LoadedTextures::setTextureBankReference(const TextureLogic::TextureBank *textureBank)
        {
            this->textureBank = textureBank;
        }
    }
}