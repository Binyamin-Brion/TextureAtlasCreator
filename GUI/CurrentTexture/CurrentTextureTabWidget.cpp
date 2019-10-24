//
// Created by BinyBrion on 2019-09-26.
//

#include <TextureLogic/TextureBank.h>
#include "CurrentTextureTabWidget.h"

#include "CurrentTextureImage.h"
#include "TextureLogic/TextureBank.h"
#include "ScrollArea.h"

namespace GUI
{
    namespace CurrentTexture
    {
        CurrentTextureTabWidget::CurrentTextureTabWidget(QWidget *parent) : QTabWidget{parent}
        {
            currentTextureIndex = -1;
            selectedTextureSize = QSize{-1, -1};

            currentTexture[GetCurrentTextureImageValue(CurrentTextureImage::SelectedTexture)].first = "Selected Texture";
            currentTexture[GetCurrentTextureImageValue(CurrentTextureImage::SelectedTexture)].second = new ScrollArea{CurrentTextureImage::SelectedTexture, this};

            currentTexture[GetCurrentTextureImageValue(CurrentTextureImage::SpecularTexture)].first = "Specular Texture";
            currentTexture[GetCurrentTextureImageValue(CurrentTextureImage::SpecularTexture)].second = new ScrollArea{CurrentTextureImage::SpecularTexture, this};

            for(auto &i : currentTexture)
            {
                addTab(i.second, i.first);

                connect(i.second, &ScrollArea::zoomChanged, [this](TextureLogic::Zoom newZoom)
                {
                    emit zoomChanged(newZoom);
                });
            }

            connect(currentTexture[GetCurrentTextureImageValue(CurrentTextureImage::SelectedTexture)].second, &ScrollArea::repaintSelectedTexture, [this]()
            {
                emit repaintSelectedTexture();
            });

            connect(currentTexture[GetCurrentTextureImageValue(CurrentTextureImage::SelectedTexture)].second, &ScrollArea::paintedSelectedTexture, [this]()
            {
                if(textureBank != nullptr)
                {
                    textureBank->selectedTextureChanged();
                }
            });

            connect(this, &QTabWidget::currentChanged, [this](int index)
            {
                emit changedRenderArea(currentTexture[index].second->getBrush());
            });
        }

        void CurrentTextureTabWidget::setSelectedTexture(TextureLogic::Texture *texture, AccessRestriction::PassKey<TextureLogic::TextureBank>)
        {
            int index = -1;

            for(auto &i : currentTexture)
            {
                i.second->setTexture(texture);
            }

            if(texture != nullptr)
            {
                unsigned formatIndex = TextureHelperFunctions::indexFormat(texture->getImage(TextureLogic::Zoom::Normal).format(), true);

                for(const auto &i : (*textures)[formatIndex].first)
                {
                    index += 1;

                    if(i.textureLocation() == texture->textureLocation())
                    {
                        break;
                    }
                }

                if(index == -1)
                {
                    Q_ASSERT_X(false, __PRETTY_FUNCTION__, "Error- invalid texture passed as the Current Selected Texture");
                }

                selectedTextureSize = texture->getImage(TextureLogic::Zoom::Normal).size();

                const ScrollArea *currentScrollArea = currentTexture[currentIndex()].second;

                emit selectedTextureChanged(texture->getImage(TextureLogic::Zoom::Normal).size(), currentScrollArea->getBrush().getPaintImage(currentScrollArea->getZoom()).size());
            }
            else
            {
                const ScrollArea *currentScrollArea = currentTexture[currentIndex()].second;

                emit selectedTextureChanged(QSize{-1, -1}, currentTexture[currentIndex()].second->getBrush().getPaintImage(currentScrollArea->getZoom()).size());
            }

            currentTextureIndex = index;
        }

        void CurrentTextureTabWidget::setTextureBankReference(TextureLogic::TextureBank *textureBank)
        {
            if(this->textureBank == nullptr)
            {
                this->textureBank = textureBank;
            }
        }

        void CurrentTextureTabWidget::setTexturesReference(std::vector<std::pair<std::vector<TextureLogic::Texture>, std::vector<unsigned int>>>& textures)
        {
            this->textures = &textures;

            unsigned int formatIndex = TextureHelperFunctions::indexFormat(currentTexture[
                                       GetCurrentTextureImageValue(CurrentTextureImage::SelectedTexture)].second->getCurrentTextureFormat(), true);

            if(currentTextureIndex != -1)
            {

                for(auto &i : currentTexture)
                {
                    i.second->setTexture(&(*this->textures)[formatIndex].first[currentTextureIndex]);
                }
            }
        }
    }
}