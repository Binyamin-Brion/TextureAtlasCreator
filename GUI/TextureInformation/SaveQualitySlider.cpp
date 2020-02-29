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
        SaveQualitySlider::SaveQualitySlider(QWidget *parent)
                            :
                                QSlider{parent}
        {
            // Percentage wise quality, with 0% specifying saving the image with the lowest possible quality and
            // 100% saving the image with the highest possible quality.
            setMinimum(0);
            setMaximum(100);

            // By default, it will be assumed the user wants to save the highest quality possible as the additional
            // disk space taken can likely be handled.
            setValue(100);

            // Default change in quality when scrolling is 5%. Chosen as it provides enough granular control but not
            // too much that would involves excessive scrolling
            setPageStep(5);
            setSingleStep(5);
        }

        void SaveQualitySlider::mousePressEvent(QMouseEvent *event)
        {
            // The slider base still needs to know about this event to ensure that the slider visually moves to new location
            QSlider::mousePressEvent(event);

            // Find the value that the new slider should have depending on where the mouse press occured. While
            // minimum() may return 0 depending on the value set in the constructor, it is best to keep it in case the value changes
            setValue(minimum() + ((maximum() - minimum()) * event->x()) / width() );

            emit sliderValueDifferent(value());
        }

        void SaveQualitySlider::wheelEvent(QWheelEvent *event)
        {
            // The slider base still needs to know about this event to ensure that the slider visually moves to new location
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