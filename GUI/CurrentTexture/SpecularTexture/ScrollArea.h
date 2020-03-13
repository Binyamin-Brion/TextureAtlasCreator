//
// Created by binybrion on 3/12/20.
//

#ifndef TEXTUREATLASCREATOR_SPECULAR_TEXTURE_SCROLLAREA_H
#define TEXTUREATLASCREATOR_SPECULAR_TEXTURE_SCROLLAREA_H

#include <QtWidgets/QScrollArea>

namespace TextureLogic
{
    class Texture;
}

namespace GUI
{
    namespace CurrentTexture
    {
        namespace SpecularTexture
        {
            class DisplayArea;

             /**
              *  Provides a scrollable window to view the specular display area. Since the display area scales with the size of the
              *  texture selected in the texture atlas, the RenderArea may be bigger than the size of the area reserved for it.
              *
              */

            class ScrollArea : public QScrollArea
            {
                    Q_OBJECT

                public:

                    /**
                     * Initializes this object with the parent passed in.
                     *
                     * @param parent widget that has ownership over this object.
                     */
                    explicit ScrollArea(QWidget *parent = nullptr);

                    /**
                     * When the cursor enters this widget, it automatically grabs the focus of the keyboard so that
                     * zoom shortcuts are received and processed as expected
                     *
                     * @param event unused
                     */
                    void enterEvent(QEvent *event) override;

                    /**
                     * Handles key presses made to this widget.
                     *
                     * @param event the key that was pressed
                     */
                    void keyPressEvent(QKeyEvent *event) override;

                    /**
                     * Handles key releases made to this widget.
                     *
                     * @param event the key that was released
                     */
                    void keyReleaseEvent(QKeyEvent *event) override;

                    /**
                     * When the cursor leaves this widget, it releases the focus of the keyboard so that other widgets
                     * are able to handle keyboard input directed to them.
                     *
                     * @param event unused
                     */
                    void leaveEvent(QEvent *event) override;

                    /**
                     * Forwards request to the display area to update the specular texture it is showing as a result
                     * of the user painting in specular mode.
                     */
                    void repaintSelectedTexture();

                    /**
                     * Set the specular texture to be displayed. Forwards argument to the display area.
                     *
                     * @param texture to holding the specular texture to display
                     */
                    void setSpecularTexture(const ::TextureLogic::Texture *texture);

                    /**
                     * Handles wheel events from the user mouse. If the control key is down, then this function tells
                     * the atlas widget to zoom in or out on the atlas, depending on the direction of the scroll. Otherwise
                     * the area of the atlas that is viewed changes.
                     *
                     * @param event
                     */
                    void wheelEvent(QWheelEvent *event) override;

                private:
                    DisplayArea *displayArea = nullptr;
                    // Keeps track if the user is holding down the control key on their keyboard
                    bool controlKeyDown = false;
            };
        }
    }
}

#endif //TEXTUREATLASCREATOR_SCROLLAREA_H
