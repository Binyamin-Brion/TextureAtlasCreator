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
        /*
         * Provides a slider by which the user can choose the quality to save a selected image.
         * It is updated when a user chooses a new quality number through the respective line edit in
         * SelectedTextureInformation, and updates that line edit if the user interacts through this slider.
         */

        class SaveQualitySlider : public QSlider
        {
            Q_OBJECT

            public:
                explicit SaveQualitySlider(QWidget *parent = nullptr);
                void mousePressEvent(QMouseEvent *event) override;
                void wheelEvent(QWheelEvent *event) override;

            signals:
                void sliderValueDifferent(int value);
        };
    }
}

#endif //TEXTUREATLASCREATOR_SAVEQUALITYSLIDER_H
