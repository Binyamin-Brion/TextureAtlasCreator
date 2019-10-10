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
        AddNewTab::AddNewTab(QWidget *parent) : QDialog{parent}, ui{new Ui::NewTabDialog}
        {
            ui->setupUi(this);

            setWindowTitle("Add New Tab");

            ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

            connect(ui->tabNameLineEdit, &QLineEdit::textEdited, [this](const QString &newText)
            {
                if(newText.isEmpty())
                {
                    ui->errorMessageLabel->setText("You must enter a tab name before continuing.");

                    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
                }
                else
                {
                    if(std::find(existingTabNames.begin(), existingTabNames.end(), newText) != existingTabNames.end())
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
            });

            connect(ui->buttonBox, &QDialogButtonBox::accepted, [this]()
            {
                emit newTabNameChosen(ui->tabNameLineEdit->text());

                ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

                ui->tabNameLineEdit->setText("");

                ui->errorMessageLabel->setText("You must enter a tab name before continuing.");
            });
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
    }
}