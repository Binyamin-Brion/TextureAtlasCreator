//
// Created by binybrion on 3/12/20.
//

#include "HelperUtilities/Assert.h"
#include "SpecularTextureTabWidget.h"
#include "ScrollArea.h"
#include "../TextureLogic/TextureBank.h"

namespace GUI
{
    namespace CurrentTexture
    {
        namespace SpecularTexture
        {
            // Note: a tab widget is used, even if there is only one tab, is that is provides a nice looking label as to
            //      what the area held within the tab is about. This could be done with a separate label, but it was decided
            //      that it doesn't look as good.

            SpecularTextureTabWidget::SpecularTextureTabWidget(QWidget *parent)
                    :
                        QTabWidget(parent),
                        displayScrollArea{new ScrollArea{this}}
            {
                addTab(displayScrollArea, "Specular Texture");

                connect(displayScrollArea, &ScrollArea::zoomChanged, [this](::TextureLogic::Zoom newZoomValue) { emit zoomChanged(newZoomValue); });
            }

            void SpecularTextureTabWidget::setSpecularTexture(const ::TextureLogic::Texture *texture)
            {
                int index = -1;

                displayScrollArea->setSpecularTexture(texture);

                if(texture != nullptr)
                {
                    // Required to index into the textures stored in texture bank. See TextureBank.cpp
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
                        ASSERT(false, __PRETTY_FUNCTION__, "Error- invalid texture passed as the Current Selected Texture");
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