//
// Created by BinyBrion on 10/4/2019.
//

#include "SaveQualitySlider.h"
#include <QMouseEvent>
#include <QWheelEvent>

namespace GUI
{
    namespace TextureInformation
    {
        SaveQualitySlider::SaveQualitySlider(QWidget *parent) : QSlider{parent}
        {
            setPageStep(5);
            setMinimum(0);
            setMaximum(100);
            setValue(100);
            setSingleStep(5);
        }

        void SaveQualitySlider::mousePressEvent(QMouseEvent *event)
        {
            QSlider::mousePressEvent(event);

            setValue(minimum() + ((maximum()-minimum()) * event->x()) / width() );

            emit sliderValueDifferent(value());

            sliderValueChanged = false;
        }

        void SaveQualitySlider::wheelEvent(QWheelEvent *event)
        {
            QAbstractSlider::wheelEvent(event);

            if(event->angleDelta().y() > 0)
            {
                setValue(value() + pageStep());
            }
            else
            {
                setValue(value() - pageStep());
            }

            emit sliderValueDifferent(value());
        }
    }
}