//
// Created by BinyBrion on 2020-03-20.
//

#include "ResizeAtlas.h"
#include "ui_resizeAtlasDialog.h"
#include <QPushButton>

namespace GUI
{
    namespace Dialogs
    {
        ResizeAtlas::ResizeAtlas(QWidget *parent)
                    :
                        QDialog{parent},
                        ui{new Ui::ResizeAtlasDialog}
        {
            ui->setupUi(this);

            connect(ui->newHeightLineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(handleAtlasHeightSpecified()));

            connect(ui->newWidthLineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(handleAtlasWidthSpecified()));

            connect(this, &QDialog::rejected, [this]() { resetDialog(); });

            resetDialog();
        }

        QSize ResizeAtlas::getRequestedSize() const
        {
            return QSize{requestedWidth, requestedHeight};
        }

        void ResizeAtlas::showEvent(QShowEvent *event)
        {
            resetDialog();

            QDialog::showEvent(event);
        }

        void ResizeAtlas::handleAtlasHeightSpecified()
        {
            atlasDimensionSpecified(ui->newHeightLineEdit, requestedHeight);
        }

        void ResizeAtlas::handleAtlasWidthSpecified()
        {
            atlasDimensionSpecified(ui->newWidthLineEdit, requestedWidth);
        }

        void ResizeAtlas::resetDialog()
        {
            ui->newHeightLineEdit->setText("");

            ui->newWidthLineEdit->setText("");

            ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
        }

        void ResizeAtlas::atlasDimensionSpecified(QLineEdit *lineEdit, int &dimension)
        {
            if(lineEdit->text().isEmpty())
            {
                lineEdit->setStyleSheet("");

                dimension = -1;

                ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

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

            // Only allow the user to click the 'Ok' button if they entered valid information
            if(requestedHeight == -1 || requestedWidth == -1)
            {
                ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
            }
            else
            {
                ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
            }
        }
    }
}