//
// Created by binybrion on 3/14/20.
//

#include "NameForm.h"
#include "ui_nameUI.h"

namespace GUI
{

    NameForm::NameForm(QWidget *parent)
                :
                    QDialog{parent},
                    ui{new Ui::NameForm}
    {
        ui->setupUi(this);

        setWindowTitle("Choose a Project Name");

        connect(ui->confirmationButton, &QPushButton::clicked, [this]() { close(); });
    }

    QString NameForm::getChosenName() const
    {
        QString chosenName = ui->nameEdit->text();

        // Reset line edit for the next time the dialog appears
        ui->nameEdit->setText("");

        return chosenName;
    }
}