//
// Created by binybrion on 3/12/20.
//

#include "SpecularTextureTabWidget.h"
#include "ScrollArea.h"
#include "../TextureLogic/TextureBank.h"

namespace GUI
{
    namespace CurrentTexture
    {
        namespace SpecularTexture
        {
            SpecularTextureTabWidget::SpecularTextureTabWidget(QWidget *parent)
                    :
                        QTabWidget(parent),
                        displayScrollArea{new ScrollArea{this}}
            {
                addTab(displayScrollArea, "Specular Texture");
            }

            void SpecularTextureTabWidget::setSpecularTexture(const ::TextureLogic::Texture *texture)
            {
                int index = -1;

                displayScrollArea->setSpecularTexture(texture);

                if(texture != nullptr)
                {
                    // Required to index into the texturs stored in texture bank. See TextureBank.cpp
                    formatIndex = TextureHelperFunctions::indexFormat(texture->getImage(TextureLogic::Zoom::Normal).format(), true);

                    for(const auto &i : textureBank->getTextures()[formatIndex].first)
                    {
                        index += 1;

                        if(i.textureLocation() == texture->textureLocation())
                        {
                            break;
                        }
                    }

                    // If the texture was not found in the texture bank, that is a fatal error
                    if(index == -1)
                    {
                        Q_ASSERT_X(false, __PRETTY_FUNCTION__, "Error- invalid texture passed as the Current Selected Texture");
                    }
                }

                currentTextureIndex = index;
            }

            void SpecularTextureTabWidget::setTextureBankReference(const ::TextureLogic::TextureBank *textureBank)
            {
                if(this->textureBank == nullptr)
                {
                    this->textureBank = textureBank;
                }
            }

            void SpecularTextureTabWidget::updateTextureReference()
            {
                if(currentTextureIndex != -1)
                {
                    displayScrollArea->setSpecularTexture(&textureBank->getTextures()[formatIndex].first[currentTextureIndex]);
                }
            }

            // Beginning of public slots

            void SpecularTextureTabWidget::repaintSpecularTexture()
            {
                displayScrollArea->repaintSelectedTexture();
            }
        }
    }
}