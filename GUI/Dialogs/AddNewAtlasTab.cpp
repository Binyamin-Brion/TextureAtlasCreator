//
// Created by BinyBrion on 10/11/2019.
//

#include "AddNewAtlasTab.h"
#include "ui_newTextureAtlasDialog.h"
#include <QPushButton>
#include "HelperUtilities/Assert.h"

namespace GUI
{
    namespace Dialogs
    {
        // Beginning of public functions

        AddNewAtlasTab::AddNewAtlasTab(QWidget *parent)
                        :
                            QDialog{parent},
                            ui{new Ui::NewTextureAtlasDialog}
        {
            ui->setupUi(this);

            internalFormatPairs = TextureHelperFunctions::internalFormatPairRepresentations();

            setWindowTitle("Create New Atlas Tab");

            // Ensure that the dialog is in a default state, even if some defaults are incorrectly specified in the UI form file
            // as well as the fact that some dialog state must be done in code (such as disabling confirm button by default)
            resetDialog();

            for(auto i = internalFormatPairs.cbegin() + 1; i != internalFormatPairs.cend(); ++i)
            {
                ui->textureFormatComboBox->addItem(i->formatStringRepresentation);
            }

            // By default, it will be assumed the desired texture atlas format will be RGB32
            ui->textureFormatComboBox->setCurrentIndex(3);

            // As the user is entering texture atlas information, check that the input is valid (as they type)
            connect(ui->newTabNameLineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(checkTabNameValid(const QString&)));

            connect(ui->textureAtlasHeightLineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(handleAtlasHeightSpecified()));

            connect(ui->texutreAtlasWidthLineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(handleAtlasWidthSpecified()));

            // If the user is done with form after entering information, then proceed to create a new texture atlas; otherwise
            // reset the texture atlas to the default state (information entered is not kept between times dialog is shown)
            connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(textureAtlasInformationConfirmed()));

            connect(this, &QDialog::rejected, [this]() { resetDialog(); });
        }

        void AddNewAtlasTab::addExistingTabName(QString tabName)
        {
            existingTabNames.push_back(tabName);
        }

        void AddNewAtlasTab::removeNameExistingTab(QString tabName)
        {
            auto tabNameIndex = std::find(existingTabNames.begin(), existingTabNames.end(), tabName);

            if(tabNameIndex == existingTabNames.end())
            {
                QString errorMessage{"Asking how to delete tab that does not exist: " + tabName};

                ASSERT(false, __PRETTY_FUNCTION__, errorMessage.toStdString().c_str());
            }

            existingTabNames.erase(tabNameIndex);
        }

        // Beginning of private slots

        void AddNewAtlasTab::checkTabNameValid(const QString &tabName)
        {
            if(std::find(existingTabNames.begin(), existingTabNames.end(), tabName) != existingTabNames.end())
            {
                // User will see the line edit turn a shade of red indicating the tab name entered is not valid
                ui->newTabNameLineEdit->setStyleSheet("background-color: rgba(255, 0, 0, 64)");
            }
            else
            {
                ui->newTabNameLineEdit->setStyleSheet("");
            }

            checkAllInformationValid();
        }

        // There are different functions for when the user enters the width and height of the texture atlas so that
        // the program can track if the dimensions entered are valid independently. This allows the dialog to ensure that
        // if a dimension is invalid, the other one is not shown as invalid

        void AddNewAtlasTab::handleAtlasHeightSpecified()
        {
            atlasDimensionSpecified(ui->textureAtlasHeightLineEdit, requestedHeight);

            checkAllInformationValid();
        }

        void AddNewAtlasTab::handleAtlasWidthSpecified()
        {
            atlasDimensionSpecified(ui->texutreAtlasWidthLineEdit, requestedWidth);

            checkAllInformationValid();
        }

        void AddNewAtlasTab::textureAtlasInformationConfirmed()
        {
            // Create new tab with an empty texture atlas using the information specified
            emit newAtlasInformationSpecified(ui->newTabNameLineEdit->text(), QSize{requestedWidth, requestedHeight},
                                              internalFormatPairs[ui->textureFormatComboBox->currentIndex() + 1].format);

            resetDialog();
        }

        // Beginning of private functions

        void AddNewAtlasTab::atlasDimensionSpecified(QLineEdit *lineEdit, int &dimension)
        {
            if(lineEdit->text().isEmpty())
            {
                lineEdit->setStyleSheet("");

                dimension = -1;

                return;
            }

            bool validDimensionSpecified;

            int requestedDimension = lineEdit->text().toInt(&validDimensionSpecified);

            if(!validDimensionSpecified || requestedDimension < 1)
            {
                // User will see the line edit turn a shade of red indicating the texture atlas dimension is not valid
                lineEdit->setStyleSheet("background-color: rgba(255, 0, 0, 64)");

                dimension = -1;
            }
            else
            {
                lineEdit->setStyleSheet("");

                dimension = requestedDimension;
            }
        }

        void AddNewAtlasTab::checkAllInformationValid()
        {
            // This is called every time the user enters new information. This allows the dialog to only accept the user
            // to proceed to create texture atlas once all information is present and valid. By calling this after each
            // information change it does not matter the order of texture atlas information the user enters

            if(requestedHeight != -1 && requestedWidth != -1 && !ui->newTabNameLineEdit->text().isEmpty())
            {
                if(std::find(existingTabNames.begin(), existingTabNames.end(), ui->newTabNameLineEdit->text()) == existingTabNames.end())
                {
                    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);

                    return;
                }
            }

            ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
        }

        void AddNewAtlasTab::resetDialog()
        {
            // By default no texture atlas information is given; the user must enter everything that is required
            // by themselves

            ui->textureAtlasHeightLineEdit->setText("");
            ui->texutreAtlasWidthLineEdit->setText("");
            ui->newTabNameLineEdit->setText("");

            ui->textureAtlasHeightLineEdit->setStyleSheet("");
            ui->texutreAtlasWidthLineEdit->setStyleSheet("");
            ui->newTabNameLineEdit->setStyleSheet("");

            ui->newTabNameLineEdit->setFocus();

            // By default the confirmation button is disabled as the user must first enter a tab name
            ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
        }
    }
}