//
// Created by BinyBrion on 2019-09-26.
//

#ifndef TEXTUREATLASCREATOR_CURRENTTEXTURETABWIDGET_H
#define TEXTUREATLASCREATOR_CURRENTTEXTURETABWIDGET_H

#include <QtWidgets/QTabWidget>
#include <array>
#include "AccessRestriction/PassKey.h"
#include "PaintArea.h"

namespace TextureLogic
{
    class TextureBank;
    class Texture;
}

namespace GUI
{
    namespace CurrentTexture
    {
        class ScrollArea;

        /**
         *  Holds all of the scroll areas which in turn hold the render areas for the currently selected texture.
         */

        class CurrentTextureTabWidget : public QTabWidget
        {
                Q_OBJECT

            signals:

                /**
                 *  Emitted when the selected texture has been modified with a brush.
                 */
                void repaintSelectedTexture();

                /**
                 *  Emitted when the selected texture texture in the atlas changes.
                 */
                void selectedTextureChanged(QSize, QSize);

                /**
                 *  Emitted when the zoom changes. Emits the new zoom level.
                 */
                void zoomChanged(::TextureLogic::Zoom);

            public:

                /**
                 * Initializes this object with the parent passed in.
                 *
                 * @param parent widget that has ownership over this object.
                 */
                explicit CurrentTextureTabWidget(QWidget *parent = nullptr);

                /**
                 *  Notifies this object that unsaved changes have been saved.
                 */
                void changesSaved();

                /**
                 * Gets the reference to the brush used for painting.
                 *
                 * @return reference to painting brush
                 */
                ::PaintFunctions::Brush* getBrush();

                /**
                 * Returns whether there are any unsaved changes made to the texture.
                 *
                 * @return true if there are unsaved changes
                 */
                bool getUnsavedChanges() const;

                /**
                 * Set the texture that can be painted. Forwards argument to the scroll area holding the render area.
                 *
                 * @param texture to be painted
                 */
                void setSelectedTexture(TextureLogic::Texture *texture, AccessRestriction::PassKey<TextureLogic::TextureBank>);

                /**
                 * Sets the reference to the texture bank.
                 *
                 * @param textureBank reference where all textures that have been loaded are stored for the program
                 */
                void setTextureBankReference(TextureLogic::TextureBank *textureBank);

                /**
                 * Called when the vector holding the textures may have been resized due to a texture being added to it.
                 * See internal note at CurrentTextureTabWidget.cpp
                 *
                 * @param textures vector containing all of th texture loaded into program
                 */
                void setTexturesReference(std::vector<std::pair<std::vector<TextureLogic::Texture>, std::vector<unsigned int>>> &textures);

            private:
                // Holds all of the scroll areas holding the render areas
                ScrollArea *paintAreaScrollArea = nullptr;

                // See internal note at CurrentTextureTabWidget.cpp
                std::vector<std::pair<std::vector<TextureLogic::Texture>, std::vector<unsigned int>>> *textures = nullptr;
                // Index into the textures vector; indicates if a texture is selected (if no texture is selected, then has value of -1)
                int currentTextureIndex = -1;

                TextureLogic::TextureBank *textureBank = nullptr;
        };
    }
}

#endif //TEXTUREATLASCREATOR_CURRENTTEXTURETABWIDGET_H
