//
// Created by BinyBrion on 10/11/2019.
//

#include "AddNewAtlasTab.h"
#include "ui_newTextureAtlasDialog.h"
#include <QPushButton>

namespace GUI
{
    namespace Dialogs
    {
        AddNewAtlasTab::AddNewAtlasTab(QWidget *parent) : QDialog{parent}, ui{new Ui::NewTextureAtlasDialog}
        {
            ui->setupUi(this);

            internalFormatPairs = TextureHelperFunctions::internalFormatPairRepresentations();

            setWindowTitle("Create New Atlas Tab");

            resetDialog();

            for(auto i = internalFormatPairs.cbegin() + 1; i != internalFormatPairs.cend(); ++i)
            {
                ui->textureFormatComboBox->addItem(i->formatStringRepresentation);
            }

            ui->textureFormatComboBox->setCurrentIndex(3);

            connect(ui->newTabNameLineEdit, &QLineEdit::textChanged, [this](const QString &text)
            {
                if(std::find(existingTabNames.begin(), existingTabNames.end(), text) != existingTabNames.end())
                {
                    ui->newTabNameLineEdit->setStyleSheet("background-color: rgba(255, 0, 0, 64)");
                }
                else
                {
                    ui->newTabNameLineEdit->setStyleSheet("");
                }

                checkAllInformationValid();
            });

            connect(ui->texutreAtlasWidthLineEdit, &QLineEdit::textChanged, [this](const QString &newText)
            {
                atlasDimensionSpecified(ui->texutreAtlasWidthLineEdit, requestedWidth);

                checkAllInformationValid();
            });

            connect(ui->textureAtlasHeightLineEdit, &QLineEdit::textChanged, [this](const QString &newText)
            {
                atlasDimensionSpecified(ui->textureAtlasHeightLineEdit, requestedHeight);

                checkAllInformationValid();
            });

            connect(ui->buttonBox, &QDialogButtonBox::accepted, [this]()
            {
                emit newAtlasInformationSpecified(ui->newTabNameLineEdit->text(), requestedWidth, requestedHeight,
                                                  internalFormatPairs[ui->textureFormatComboBox->currentIndex() + 1].format);

                resetDialog();
            });

            connect(this, &QDialog::rejected, [this]()
            {
               resetDialog();
            });
        }

        void AddNewAtlasTab::addExistingTabName(QString tabName)
        {
            existingTabNames.push_back(tabName);
        }

        void AddNewAtlasTab::atlasDimensionSpecified(QLineEdit *lineEdit, int &dimension)
        {
            if(lineEdit->text().isEmpty())
            {
                lineEdit->setStyleSheet("");

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

        void AddNewAtlasTab::checkAllInformationValid()
        {
            if(requestedHeight != -1 && requestedWidth != -1)
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
            ui->textureAtlasHeightLineEdit->setText("");
            ui->texutreAtlasWidthLineEdit->setText("");
            ui->newTabNameLineEdit->setText("");

            ui->textureAtlasHeightLineEdit->setStyleSheet("");
            ui->texutreAtlasWidthLineEdit->setStyleSheet("");
            ui->newTabNameLineEdit->setStyleSheet("");

            ui->newTabNameLineEdit->setFocus();

            ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
        }
    }
}