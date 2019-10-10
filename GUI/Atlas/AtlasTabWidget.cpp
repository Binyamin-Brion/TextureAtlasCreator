//
// Created by BinyBrion on 2019-09-16.
//

#include "AtlasTabWidget.h"
#include "TextureLogic/TextureBank.h"
#include "ScrollArea.h"

namespace GUI
{
    namespace Atlas
    {
        AtlasTabWidget::AtlasTabWidget(QWidget *parent) : QTabWidget{parent}
        {
            QString defaultTabName{"Default"};

            auto *defaultTabScrollArea = new ScrollArea{QSize{1920, 1080}, this};

            currentTabs.emplace_back(defaultTabScrollArea, defaultTabName);

            addTab(currentTabs.back().first, currentTabs.back().second);
        }

        void AtlasTabWidget::addTextureToCurrentAtlas(const TextureLogic::Texture &texture)
        {
            currentTabs[currentIndex()].first->addTexture(texture);
        }

        void AtlasTabWidget::removeTexture(const TextureLogic::Texture *texture)
        {
            for(auto &i : currentTabs)
            {
                i.first->removeTexture(texture);
            }
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
                i.first->updateTextureReferences(textureBank->getTextures({}));
            }
        }

        void AtlasTabWidget::repaintSelectedTexture()
        {
            currentTabs[currentIndex()].first->repaintSelectedTexture();
        }
    }
}