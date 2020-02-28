//
// Created by BinyBrion on 10/10/2019.
//

#include "AddNewTab.h"

#include "ui_newTabDialog.h"
#include <QPushButton>

namespace GUI
{
    namespace Dialogs
    {
        // Beginning of public functions

        AddNewTab::AddNewTab(QWidget *parent)
                    :
                        QDialog{parent},
                        ui{new Ui::NewTabDialog}
        {
            ui->setupUi(this);

            setWindowTitle("Add New Tab");

            // Ensure that the dialog is in a default state, even if some defaults are incorrectly specified in the UI form file,
            // as well as the fact that some dialog state must be done in code (such as disabling confirm button by default)
            resetDialog();

            // As the user is entering texture atlas information, check that the input is valid (as they type)
            connect(ui->tabNameLineEdit, SIGNAL(textEdited(const QString&)), this, SLOT(checkTabNameValid(const QString&)));

            // If the user is done entering a new tab information, then proceed to rename the current tab. Then reset dialog to default state.
            connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(newTabNameConfirmed()));

            connect(ui->buttonBox, &QDialogButtonBox::rejected, [this]() { resetDialog(); });
        }

        void AddNewTab::addNameExistingTab(QString tabName)
        {
            existingTabNames.push_back(tabName);
        }

        void AddNewTab::removeNameExistingTab(QString tabName)
        {
            auto tabNameIndex = std::find(existingTabNames.begin(), existingTabNames.end(), tabName);

            if(tabNameIndex == existingTabNames.end())
            {
                QString errorMessage{"Asking how to delete tab that does not exist: " + tabName};

                Q_ASSERT_X(false, __PRETTY_FUNCTION__, errorMessage.toStdString().c_str());
            }

            existingTabNames.erase(tabNameIndex);
        }

        // Beginning of private slots

        void AddNewTab::checkTabNameValid(const QString &tabName)
        {
            if(tabName.isEmpty())
            {
                ui->errorMessageLabel->setText("You must enter a tab name before continuing.");

                ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
            }
            else
            {
                if(std::find(existingTabNames.begin(), existingTabNames.end(), tabName) != existingTabNames.end())
                {
                    ui->errorMessageLabel->setText("That tab name is already taken.");

                    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
                }
                else
                {
                    ui->errorMessageLabel->setText("");

                    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
                }
            }
        }

        void AddNewTab::newTabNameConfirmed()
        {
            // Rename the current tab with the name specified by the user
            emit newTabNameChosen(ui->tabNameLineEdit->text());

            resetDialog();
        }

        // Beginning of private functions

        void AddNewTab::resetDialog()
        {
            ui->tabNameLineEdit->setText("");

            ui->errorMessageLabel->setText("You must enter a tab name before continuing.");

            // By default the confirmation button is disabled as the user must first enter a tab name
            ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
        }
    }
}