//
// Created by BinyBrion on 2019-09-26.
//

#include <TextureLogic/TextureBank.h>
#include "CurrentTextureTabWidget.h"

#include "CurrentTextureImage.h"
#include "TextureLogic/TextureBank.h"

namespace GUI
{
    namespace CurrentTexture
    {
        CurrentTextureTabWidget::CurrentTextureTabWidget(QWidget *parent) : QTabWidget{parent}
        {
            currentTextureIndex = -1;
            selectedTextureSize = QSize{-1, -1};

            currentTexture[GetCurrentTextureImageValue(CurrentTextureImage::SelectedTexture)].first = "Selected Texture";
            currentTexture[GetCurrentTextureImageValue(CurrentTextureImage::SelectedTexture)].second = new RenderArea{CurrentTextureImage::SelectedTexture, this};

            currentTexture[GetCurrentTextureImageValue(CurrentTextureImage::SpecularTexture)].first = "Specular Texture";
            currentTexture[GetCurrentTextureImageValue(CurrentTextureImage::SpecularTexture)].second = new RenderArea{CurrentTextureImage::SpecularTexture, this};

            for(auto &i : currentTexture)
            {
                addTab(i.second, i.first);
            }

            connect(currentTexture[GetCurrentTextureImageValue(CurrentTextureImage::SelectedTexture)].second, &RenderArea::repaintSelectedTexture, [this]()
            {
                emit repaintSelectedTexture();
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
                for(const auto &i : *textures)
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

                emit selectedTextureChanged(texture->getImage(TextureLogic::Zoom::Normal).size(), currentTexture[currentIndex()].second->getBrush().getPaintImage().size());
            }
            else
            {
                emit selectedTextureChanged(QSize{-1, -1}, currentTexture[currentIndex()].second->getBrush().getPaintImage().size());
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

        void CurrentTextureTabWidget::setTexturesReference(std::vector<TextureLogic::Texture>& textures)
        {
            this->textures = &textures;

            if(currentTextureIndex != -1)
            {
                for(auto &i : currentTexture)
                {
                    i.second->setTexture(&(*this->textures)[currentTextureIndex]);
                }
            }
        }
    }
}