//
// Created by BinyBrion on 2019-09-15.
//

#include <QtWidgets/QFileDialog>
#include "ChooseTexture.h"

namespace GUI
{
    namespace Dialogs
    {
        // Beginning of public functions

        ChooseTexture::ChooseTexture(QWidget *parent) : QDialog{parent}, ui{new Ui::TextureChooser}
        {
            ui->setupUi(this);

            connect(ui->fileButton, SIGNAL(clicked()), this, SLOT(fileLocationButtonClicked()));

            // As the user is inputting a border width, check that the input is valid (as they type)
            connect(ui->intersectionWidthLineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(intersectionWidthTextChanged(const QString&)));

            connect(ui->selectionBorderWidth, SIGNAL(textChanged(const QString&)), this, SLOT(selectionWidthTextChanged(const QString&)));

            // If the user fulfilled all of the require information to load a texture, then proceed to load a texture.
            // Otherwise, if the user cancels the dialog, reset the dialog to a default state
            connect(ui->buttonBox, &QDialogButtonBox::accepted, [this]()
            {
                emit textureChosen(ui->textureButtonAreaChooser->currentText(), ui->fileButton->text(), intersectionBorderWidth, selectionBorderWidth);

                resetDialog();
            });

            connect(ui->buttonBox, &QDialogButtonBox::rejected, [this]() { resetDialog(); });

            // Ensure that even if the default state is not specified in the UI form, what the user sees upon dialog
            // popup is the default state
            resetDialog();
        }

        void ChooseTexture::addTab(const QString &tabName)
        {
            ui->textureButtonAreaChooser->addItem(tabName);

            tabNames.push_back(tabName);
        }

        void ChooseTexture::closeEvent(QCloseEvent *event)
        {
            QDialog::closeEvent(event);

            resetDialog();
        }

        void ChooseTexture::removeTab(const QString &tabName)
        {
            auto tabNameLocation = std::find(tabNames.begin(), tabNames.end(), tabName);


            Q_ASSERT_X(tabNameLocation != tabNames.end(), __PRETTY_FUNCTION__, "\n\nUnable to find tab name to remove.\n\n");

            ui->textureButtonAreaChooser->removeItem(std::distance(tabNames.begin(), tabNameLocation));

            tabNames.erase(tabNameLocation);
        }

        // Beginning of private slots

        void ChooseTexture::fileLocationButtonClicked()
        {
            // During debugging it is more convenient to have the file selection dialog automatically go to the pictures folder
            #ifdef QT_DEBUG
                ui->fileButton->setText(QFileDialog::getOpenFileName(this, tr("Open Image"), "C:/Users/BinyBrion/Pictures", tr("Image Files ()")));
            #else
                ui->fileButton->setText(QFileDialog::getOpenFileName(this, tr("Open Image"), QDir::homePath(), tr("Image Files ()")));
            #endif

            if(ui->fileButton->text().isEmpty())
            {
                // If user closes file selection dialog without selecting a location, the file button will have no text on it.
                // Add some text so that it remains clear what the button is for.
                ui->fileButton->setText("Click here to select a texture");

                validTexturePathSelected = false;

                setStrikeOutErrorLabel(false);

                checkValidData();

                return;
            }

            validTexturePathSelected = true;

            // User has specified the required information to remove the requirement message
            setStrikeOutErrorLabel(true);

            checkValidData();
        }

        void ChooseTexture::intersectionWidthTextChanged(const QString&)
        {
            borderWidthSpecified(ui->intersectionWidthLineEdit, intersectionBorderWidth);

            checkValidData();
        }

        void ChooseTexture::selectionWidthTextChanged(const QString&)
        {
            borderWidthSpecified(ui->selectionBorderWidth, selectionBorderWidth);

            checkValidData();
        }

        // Beginning of private functions

        void ChooseTexture::borderWidthSpecified(QLineEdit *lineEdit, int &dimension)
        {
            // If no input is in text field, then set the number associated with the text input to -1 as a default number
            if(lineEdit->text().isEmpty())
            {
                lineEdit->setStyleSheet("");

                dimension = -1;

                return;
            }

            // Try to convert user input to number; if it succeeds, accept the input. Otherwise show the text field
            // as red so that the user is aware of error.

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
            // User can only proceed to load the texture if a valid texture location is specified along with valid border widths
            if(validTexturePathSelected && intersectionBorderWidth != -1 && selectionBorderWidth != -1)
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

            // These are the default values chosen for the respective border widths

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