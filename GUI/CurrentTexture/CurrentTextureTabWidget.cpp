//
// Created by BinyBrion on 2019-09-26.
//

#include <TextureLogic/TextureBank.h>
#include "CurrentTextureTabWidget.h"

#include "CurrentTextureImage.h"

namespace GUI
{
    namespace CurrentTexture
    {
        CurrentTextureTabWidget::CurrentTextureTabWidget(QWidget *parent) : QTabWidget{parent}
        {
            currentTexture[GetCurrentTextureImageValue(CurrentTextureImage::SelectedTexture)].first = "Selected Texture";
            currentTexture[GetCurrentTextureImageValue(CurrentTextureImage::SelectedTexture)].second = new RenderArea{CurrentTextureImage::SelectedTexture, this};

            currentTexture[GetCurrentTextureImageValue(CurrentTextureImage::SpecularTexture)].first = "Specular Texture";
            currentTexture[GetCurrentTextureImageValue(CurrentTextureImage::SpecularTexture)].second = new RenderArea{CurrentTextureImage::SpecularTexture, this};

            for(auto &i : currentTexture)
            {
                addTab(i.second, i.first);
            }
        }

        void CurrentTextureTabWidget::setSelectedTexture(TextureLogic::Texture *texture, AccessRestriction::PassKey<TextureLogic::TextureBank>)
        {
            currentTexture[GetCurrentTextureImageValue(CurrentTextureImage::SelectedTexture)].second->setTexture(texture);

            currentTexture[GetCurrentTextureImageValue(CurrentTextureImage::SpecularTexture)].second->setTexture(texture);
        }
    }
}