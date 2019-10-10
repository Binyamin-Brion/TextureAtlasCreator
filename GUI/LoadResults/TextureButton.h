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
                void enterEvent(QEvent *event) override;
                void leaveEvent(QEvent *event) override;
                const QString& getTextureLocation() const;
                bool mouseOver(QPoint mousePos) const;
                static const int buttonSizeLength = 150;

            signals:
                void buttonClicked(const QString &textureLocation);
                void cursorOverButton(const TextureButton*);
                void cursorNotOverButton(const TextureButton*);

            private:
                QPixmap pixMap;
                QIcon buttonIcon;
                QString textureLocation;

        };
    }
}

#endif //TEXTUREATLASCREATOR_TEXTUREBUTTON_H
