//
// Created by BinyBrion on 2019-09-26.
//

#include <TextureLogic/TextureBank.h>
#include <QtWidgets/qscrollarea.h>
#include "CurrentTextureTabWidget.h"

#include "ScrollArea.h"

namespace GUI
{
    namespace CurrentTexture
    {
        // Note: a tab widget is used, even if there is only one tab, is that is provides a nice looking label as to
        //      what the area held within the tab is about. This could be done with a separate label, but it was decided
        //      that it doesn't look as good.

        /*
         *  The variable textures is not strictly required. This class could always call getTextures() on the texture bank,
         *  which would negate the need for the textures variable. It is held for two reasons:
         *
         *  1. It is believed it increases readability given the entire class code
         *  2. No need to consider using a const_cast. The render areas need a mutable reference to the selected texture that
         *     is stored within texture bank. However, the result of getTextures() on texture bank returns an immutable reference.
         *     A const cast would be required for the program to work. While there is still the same level of mutability as the
         *     texture bank itself provides the mutable reference to this class, it has been decided that this is preferable
         *     as it is consistent that all users of getTextures() use it an immutable manner.
         */

        CurrentTextureTabWidget::CurrentTextureTabWidget(QWidget *parent)
                                    :
                                        QTabWidget{parent}
        {
            // At the start of the program no texture is selected
            currentTextureIndex = -1;

            // These are not created in the initialized list as it is more readable to create these here
            paintAreaScrollArea = new ScrollArea{this};

            addTab(paintAreaScrollArea, "Selected Texture");

            // When the zoom, changes, the brush needs to know so that it can change its size so that its size remains constant
            // relative to the adjusted size of the texture
            connect(paintAreaScrollArea, &ScrollArea::zoomChanged, [this](TextureLogic::Zoom newZoom) { emit zoomChanged(newZoom); });


            // Have to let texture atlas to know to repaint the selected texture as changes made in the render area are not automatically visible in the texture atlas
            connect(paintAreaScrollArea, &ScrollArea::repaintSelectedTexture, [this]() { emit repaintSelectedTexture(); });

            // When the user is done a paint operation, then mark the texture as having been modified in the information panel
            connect(paintAreaScrollArea, &ScrollArea::paintedSelectedTexture, [this]()
            {
                if(textureBank != nullptr)
                {
                    textureBank->selectedTextureChanged();
                }
            });
        }

        ::PaintFunctions::Brush* CurrentTextureTabWidget::getBrush()
        {
            return &paintAreaScrollArea->getBrush();
        }

        void CurrentTextureTabWidget::setSelectedTexture(TextureLogic::Texture *texture, AccessRestriction::PassKey<TextureLogic::TextureBank>)
        {
            // Index into texture vector in texture bank
            int index = -1;

            // Update the render areas so that they show and modify the new selected texture
            paintAreaScrollArea->setTexture(texture);


            // If a texture was selected, check that it indeed exists, and if it does, update the brush to reflect that
            // a new texture has been selected that may have a different size than the previous selected texture
            if(texture != nullptr)
            {
                // Required to index into the textures stored in texture bank. See TextureBank.cpp
                unsigned int formatIndex = TextureHelperFunctions::indexFormat(texture->getImage(TextureLogic::Zoom::Normal).format(), true);

                // Find the index of the texture within texture bank
                for(const auto &i : (*textures)[formatIndex].first)
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

                // Make the brush be aware of the new size restrictions when changing the brush size as a result of the selected texture size changing,
                // and give it a brush size to display
                emit selectedTextureChanged(texture->getImage(TextureLogic::Zoom::Normal).size(), paintAreaScrollArea->getBrush().getPaintImage(paintAreaScrollArea->getZoom()).size());
            }
            else // No texture is selected
            {
                // Inform the brush to disable any modifications to it until a texture is selected again
                emit selectedTextureChanged(QSize{-1, -1}, paintAreaScrollArea->getBrush().getPaintImage(paintAreaScrollArea->getZoom()).size());
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
            // See note at the top of this file.
            this->textures = &textures;

            // Required to index into vector in texture bank. See TextureBank.cpp for more details.
            unsigned int formatIndex = TextureHelperFunctions::indexFormat(paintAreaScrollArea->getCurrentTextureFormat(), true);

            // Iif(f there is a texture selected, update the references held in render area to ensure they are still pointing to
            // a valid memory location
            if(currentTextureIndex != -1)
            {
                paintAreaScrollArea->setTexture(&(*this->textures)[formatIndex].first[currentTextureIndex]);
            }
        }
    }
}