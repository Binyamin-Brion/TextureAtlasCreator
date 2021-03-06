//
// Created by binybrion on 3/12/20.
//

#ifndef TEXTUREATLASCREATOR_SPECULARTEXTURETABWIDGET_H
#define TEXTUREATLASCREATOR_SPECULARTEXTURETABWIDGET_H

#include <QtWidgets/QTabWidget>
#include "../TextureLogic/Zoom.h"

namespace TextureLogic
{
    class Texture;
    class TextureBank;
}

namespace GUI
{
    namespace CurrentTexture
    {
        namespace SpecularTexture
        {
            class ScrollArea;

            /**
             *  Holds the tab where the display area for the currently selected texture resides.
             */

            class SpecularTextureTabWidget : public QTabWidget
            {
                    Q_OBJECT

                signals:

                    /**
                     *  Emitted when the zoom of the specular display area changes.
                     */
                    void zoomChanged(::TextureLogic::Zoom);

                public:

                    /**
                     * Initializes this object with the parent passed in.
                     *
                     * @param parent widget that has ownership over this object.
                     */
                    explicit SpecularTextureTabWidget(QWidget *parent = nullptr);

                    /**
                    * Set the texture that can be painted. Forwards argument to the scroll area holding the display area.
                    *
                    * @param texture holding the specular texture to display
                    */
                    void setSpecularTexture(const ::TextureLogic::Texture *texture);

                    /**
                     * Sets the reference to the texture bank.
                     *
                     * @param textureBank reference where all textures that have been loaded are stored for the program
                     */
                    void setTextureBankReference(const ::TextureLogic::TextureBank *textureBank);

                    /**
                     * Called when the passed in texture reference may no longer be valid due to texture bank
                     * being modified with addition of a new texture.
                     */
                    void updateTextureReference();

                public slots:

                    /**
                      * Forwards request to the display area to update the specular texture it is showing as a result
                      * of the user painting in specular mode.
                      */
                    void repaintSpecularTexture();

                private:
                    ScrollArea *displayScrollArea = nullptr;
                    int currentTextureIndex = -1;
                    unsigned int formatIndex;
                    const TextureLogic::TextureBank *textureBank = nullptr;
            };
        }
    }
}

#endif //TEXTUREATLASCREATOR_SPECULARTEXTURETABWIDGET_H
