//
// Created by BinyBrion on 2019-09-15.
//

#ifndef TEXTUREATLASCREATOR_TEXTUREBUTTONAREA_H
#define TEXTUREATLASCREATOR_TEXTUREBUTTONAREA_H

#include <QtWidgets/QWidget>

namespace TextureLogic
{
    class TextureBank;
}

namespace GUI
{
    namespace LoadResults
    {
        class TextureButton;

        class TextureButtonArea : public QWidget
        {
            Q_OBJECT

            public:
                explicit TextureButtonArea(QWidget *parent = nullptr);
                void addTextureButton(const QString &textureLocation);
                void setTextureBankReference(TextureLogic::TextureBank *textureBank);

            private slots:
                void textureButtonClicked(const QString &textureLocation);

            private:
                std::vector<TextureButton*> textureButtons;
                TextureLogic::TextureBank *textureBank = nullptr;
        };
    }
}

#endif //TEXTUREATLASCREATOR_TEXTUREBUTTONAREA_H
