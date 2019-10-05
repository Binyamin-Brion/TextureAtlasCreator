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
        class SaveQualitySlider : public QSlider
        {
            Q_OBJECT

            public:
                explicit SaveQualitySlider(QWidget *parent = nullptr);
                void mousePressEvent(QMouseEvent *event) override;
                void wheelEvent(QWheelEvent *event) override;

            signals:
                void sliderValueDifferent(int value);

            private:
                bool sliderValueChanged;
        };
    }
}

#endif //TEXTUREATLASCREATOR_SAVEQUALITYSLIDER_H
