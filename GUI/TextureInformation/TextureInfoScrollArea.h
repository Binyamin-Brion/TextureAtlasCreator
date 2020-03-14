//
// Created by BinyBrion on 10/3/2019.
//

#ifndef TEXTUREATLASCREATOR_TEXTUREINFOSCROLLAREA_H
#define TEXTUREATLASCREATOR_TEXTUREINFOSCROLLAREA_H

#include <QtWidgets/QScrollArea>
#include "TextureLogic/Zoom.h"
#include "AccessRestriction/PassKey.h"

namespace TextureLogic
{
    class Texture;
    class TextureBank;
}

namespace GUI
{
    namespace TextureInformation
    {
        class SelectedTextureInformation;

        /**
         *  Provides a scrollable view for the SelectedTextureInformation Widget.
         *  Since that widget may potentially not fit all of its content into the screen space given to it,
         *  a scrollable view is needed to view all of it.
         */

        class TextureInfoScrollArea : public QScrollArea
        {
            Q_OBJECT

            public:

                /**
                 * Initializes this object with the parent passed in.
                 *
                 * @param parent widget that has ownership over this object.
                 */
                explicit TextureInfoScrollArea(QWidget *parent = nullptr);

                /**
                 *  Sets the widget for this scroll area. See internal note in TextureInfoScrollArea.cpp.
                 */
                void setCentralWidget();

                /**
                 *  This is called when the currently selected texture is modified in the CurrentTexture Render Area.
                 */
                void selectedTextureModified();

                /**
                 *  Sets the currently selected texture.
                 *
                 * @param texture reference to the currently selected texture
                 */
                void setTexture(const TextureLogic::Texture *texture, AccessRestriction::PassKey<TextureLogic::TextureBank>);

                /**
                 * Sets the reference to the texture bank.
                 *
                 * @param textureBank reference to object that stores all textures
                 */
                void setTextureBankReference(TextureLogic::TextureBank *textureBank);

            private slots:
                void changeIntersectionBorderWidth(TextureLogic::Texture *texture, TextureLogic::Zoom zoom, unsigned int newBorderWidth);

                void reuploadTexture(const QString& textureLocation, const TextureLogic::Texture *texture, unsigned int intersectionBorderWidth, unsigned int selectionBorderWidth);

            private:
                SelectedTextureInformation *selectedTextureInformation = nullptr;
                TextureLogic::TextureBank *textureBank = nullptr;

        };
    }
}

#endif //TEXTUREATLASCREATOR_TEXTUREINFOSCROLLAREA_H
