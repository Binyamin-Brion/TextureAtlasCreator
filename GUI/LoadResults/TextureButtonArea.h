//
// Created by BinyBrion on 2019-09-15.
//

#ifndef TEXTUREATLASCREATOR_TEXTUREBUTTONAREA_H
#define TEXTUREATLASCREATOR_TEXTUREBUTTONAREA_H

#include <QtWidgets/QWidget>

class QGridLayout;

namespace TextureLogic
{
    class TextureBank;
}

namespace GUI
{
    namespace LoadResults
    {
        class TextureButton;

        /*
         * The widget embedded in the scroll area held within the LoadTextures class.
         * It stores all the texture buttons (see the TextureButton class) for the current
         * tab in the LoadedTexture widget.
         */

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

                void addTextureButtonPlaceHolders(int addRows);

                std::vector<TextureButton*> textureButtons;
                std::vector<QWidget*> textureButtonPlaceHolders; // See TextureButtonArea.cpp for explanation of this
                TextureLogic::TextureBank *textureBank = nullptr;

                int currentRow = 0;
                int currentColumn = 0;

                int maxRowCount = 0;
                int maxColumnCount;

                QGridLayout *gridLayout = nullptr;

                int gridHorizontalSpacing;
        };
    }
}

#endif //TEXTUREATLASCREATOR_TEXTUREBUTTONAREA_H
