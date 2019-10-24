//
// Created by BinyBrion on 2019-09-15.
//

#include <QtWidgets/QFileDialog>
#include "ChooseTexture.h"

namespace GUI
{
    namespace Dialogs
    {
        ChooseTexture::ChooseTexture(QWidget *parent) : QDialog{parent}, ui{new Ui::TextureChooser}
        {
            ui->setupUi(this);

            connect(ui->fileButton, &QPushButton::clicked, [this]()
            {
                ui->fileButton->setText(QFileDialog::getOpenFileName(this, tr("Open Image"), "C:/Users/BinyBrion/Pictures", tr("Image Files ()")));

                setStrikeOutErrorLabel(true);

                checkValidData();
            });

            connect(ui->intersectionWidthLineEdit, &QLineEdit::textChanged, [this](const QString &newText)
            {
                borderWidthSpecified(ui->intersectionWidthLineEdit, intersectionBorderWidth);

                checkValidData();
            });

            connect(ui->selectionBorderWidth, &QLineEdit::textChanged, [this](const QString &newText)
            {
                borderWidthSpecified(ui->selectionBorderWidth, selectionBorderWidth);

                checkValidData();
            });

            connect(ui->buttonBox, &QDialogButtonBox::accepted, [this]()
            {
                emit textureChosen(ui->fileButton->text(), intersectionBorderWidth, selectionBorderWidth);

                resetDialog();
            });

            connect(ui->buttonBox, &QDialogButtonBox::rejected, [this]()
            {
                resetDialog();
            });

            resetDialog();
        }

        void ChooseTexture::addTab(const QString &tabName)
        {
            ui->atlasChooser->addItem(tabName);
        }

        void ChooseTexture::closeEvent(QCloseEvent *event)
        {
            QDialog::closeEvent(event);

            resetDialog();
        }

        void ChooseTexture::borderWidthSpecified(QLineEdit *lineEdit, int &dimension)
        {
            if(lineEdit->text().isEmpty())
            {
                lineEdit->setStyleSheet("");

                dimension = -1;

                return;
            }

            bool ok;

            int requestedDimension = lineEdit->text().toInt(&ok);

            if(!ok || requestedDimension < 1)
            {
                lineEdit->setStyleSheet("background-color: rgba(255, 0, 0, 64)");

                dimension = -1;
            }
            else
            {
                lineEdit->setStyleSheet("");

                dimension = requestedDimension;
            }
        }

        void ChooseTexture::checkValidData()
        {
            if(ui->fileButton->text() != "Click here to select a texture" && intersectionBorderWidth != -1 && selectionBorderWidth != -1)
            {
                ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
            }
            else
            {
                ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
            }
        }

        void ChooseTexture::resetDialog()
        {
            ui->fileButton->setText("Click here to select a texture");

            ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

            ui->intersectionWidthLineEdit->setText("5");

            ui->selectionBorderWidth->setText("2");

            intersectionBorderWidth = 5;

            selectionBorderWidth = 2;

            ui->fileButton->setFocus();

            setStrikeOutErrorLabel(false);
        }

        void ChooseTexture::setStrikeOutErrorLabel(bool value)
        {
            QFont labelFont = ui->errorLabel->font();

            labelFont.setStrikeOut(value);

            ui->errorLabel->setFont(labelFont);
        }
    }
}