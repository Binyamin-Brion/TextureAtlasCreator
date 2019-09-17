//
// Created by BinyBrion on 2019-09-15.
//

#ifndef TEXTUREATLASCREATOR_TEXTUREBUTTON_H
#define TEXTUREATLASCREATOR_TEXTUREBUTTON_H

#include <QtWidgets/QPushButton>

namespace TextureLogic
{
    class TextureBank;
}

namespace GUI
{
    namespace LoadResults
    {

        class TextureButton : public QPushButton
        {
            Q_OBJECT

            public:
                TextureButton(const QString &textureLocation, QWidget *parent = nullptr);

                static const int buttonSizeLength = 150;

            signals:
                void buttonClicked(const QString &textureLocation);

            private:
                QPixmap pixMap;
                QIcon buttonIcon;
                QString textureLocation;

        };
    }
}

#endif //TEXTUREATLASCREATOR_TEXTUREBUTTON_H
