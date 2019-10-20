//
// Created by BinyBrion on 2019-09-15.
//

#ifndef TEXTUREATLASCREATOR_CHOOSETEXTURE_H
#define TEXTUREATLASCREATOR_CHOOSETEXTURE_H

#include "ui_chooseTexture.h"

namespace GUI
{
    namespace Dialogs
    {
        class ChooseTexture : public QDialog
        {
            Q_OBJECT

            public:
                explicit ChooseTexture(QWidget *parent = nullptr);
                void addTab(const QString &tabName);
                void closeEvent(QCloseEvent *event) override;

            signals:
                void textureChosen(QString, unsigned int, unsigned int);

            private:
                void borderWidthSpecified(QLineEdit *lineEdit, int &dimension);
                void checkValidData();
                void resetDialog();
                void setStrikeOutErrorLabel(bool value);

                int intersectionBorderWidth;
                int selectionBorderWidth;

                Ui::TextureChooser *ui = nullptr;
        };
    }
}

#endif //TEXTUREATLASCREATOR_CHOOSETEXTURE_H
