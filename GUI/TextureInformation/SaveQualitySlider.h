//
// Created by BinyBrion on 10/4/2019.
//

#ifndef TEXTUREATLASCREATOR_SAVEQUALITYSLIDER_H
#define TEXTUREATLASCREATOR_SAVEQUALITYSLIDER_H

#include <QtWidgets/QSlider>

namespace GUI
{
    namespace TextureInformation
    {
        /**
         * Provides a slider by which the user can choose the quality to save a selected image.
         * It is updated when a user chooses a new quality number through the respective line edit in
         * SelectedTextureInformation, and updates that line edit if the user interacts through this slider.
         */

        class SaveQualitySlider : public QSlider
        {
                Q_OBJECT

            signals:

                /**
                 *  Emitted when the slider moves, changing the value it represents.
                 *
                 * @param value that the slider now represents
                 */
                void sliderValueDifferent(int value);

            public:

                /**
                 * Initializes this object with the parent passed in.
                 *
                 * @param parent widget that has ownership over this object.
                 */
                explicit SaveQualitySlider(QWidget *parent = nullptr);

                /**
                 * Handles mouse presses event, which causes this object to update the slider position
                 * and to emit a signal specifying the new slider value.
                 *
                 * @param event associated with the mouse click
                 */
                void mousePressEvent(QMouseEvent *event) override;

                /**
                 * Handles the wheel event, which causes this object to update the slider position and to emit
                 * a signal specifying the new slider value.
                 *
                 * @param event
                 */
                void wheelEvent(QWheelEvent *event) override;
        };
    }
}

#endif //TEXTUREATLASCREATOR_SAVEQUALITYSLIDER_H
