//
// Created by binybrion on 3/12/20.
//

#ifndef TEXTUREATLASCREATOR_DISPLAYAREA_H
#define TEXTUREATLASCREATOR_DISPLAYAREA_H

#include <QtWidgets/QWidget>
#include "../TextureLogic/Zoom.h"

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
            /**
             *  Displays the specular texture for the currently selected texture. Whenever the user paints in specular
             *  texture mode, the texture shown in this area is updated accordingly.
             */

            class DisplayArea : public QWidget
            {
                    Q_OBJECT

                public:

                    /**
                     * Initializes this object with the parent passed in.
                     *
                     * @param parent widget that has ownership over this object
                     */
                    explicit DisplayArea(QWidget *parent = nullptr);

                    /**
                     * Gets the zoom that this display area is currently at.
                     *
                     * @return zoom value of the display area
                     */
                    ::TextureLogic::Zoom getZoom() const;

                    /**
                     * Handles requests to paint the selected texture, effectively showing the selected texture after
                     * it was painted.
                     *
                     * @param event unused
                     */
                    void paintEvent(QPaintEvent *event) override;

                    /**
                     *  Called when the user painted the selected texture in the specular painting mode.
                     */
                    void repaintSpecularTexture();

                    /**
                     * Sets the specular texture for the currently selected texture.
                     *
                     * @param specularTexture reference to the texture that was selected in the atlas
                     */
                    void setSpecularTexture(const ::TextureLogic::Texture *specularTexture);

                    /**
                     * Update the drawing position of the texture in this object, if there is one, to account for the change in zoom.
                     * This widget is also resized to match the new size of the texture at the new zoom level.
                     *
                     * Note: zoom is in done in steps, ie from 100% to 200%, not from 100% to 400%. This negates the need to
                     *       pass in the new zoom value in to this function.
                     */
                    void zoomIn();

                    /**
                     * Update the drawing position of the texture in this object, if there is one, to account for the change in zoom.
                     * This widget is also resized to match the new size of the texture at the new zoom level.
                     *
                     * Note: zoom is in done in steps, ie from 100% to 200%, not from 100% to 400%. This negates the need to
                     *       pass in the new zoom value in to this function.
                     */
                    void zoomOut();

                private:
                    const ::TextureLogic::Texture *specularTexture = nullptr;
                    ::TextureLogic::Zoom currentZoom;
            };
        }
    }
}


#endif //TEXTUREATLASCREATOR_DISPLAYAREA_H
