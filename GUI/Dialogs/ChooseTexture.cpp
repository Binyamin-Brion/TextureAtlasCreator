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

            ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

            connect(ui->fileButton, &QPushButton::clicked, [this]()
            {
                ui->fileButton->setText(QFileDialog::getOpenFileName(this, tr("Open Image"), "C:/Users/BinyBrion/Pictures", tr("Image Files ()")));

                ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);

                setStrikeOutErrorLabel(true);
            });

            connect(ui->buttonBox, &QDialogButtonBox::accepted, [this]()
            {
                emit textureChosen(ui->fileButton->text());

                resetDialog();
            });

            connect(ui->buttonBox, &QDialogButtonBox::rejected, [this]()
            {
                resetDialog();
            });
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

        void ChooseTexture::resetDialog()
        {
            ui->fileButton->setText("Click here to select a texture");

            ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

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