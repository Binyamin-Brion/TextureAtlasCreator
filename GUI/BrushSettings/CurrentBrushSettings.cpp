//
// Created by BinyBrion on 10/2/2019.
//

#include "CurrentBrushSettings.h"

#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QColorDialog>
#include <QtWidgets/QMessageBox>
#include "PaintFunctions/Brush.h"

namespace GUI
{
    namespace BrushSettings
    {
        CurrentBrushSettings::CurrentBrushSettings(QWidget *parent) : QWidget{parent}
        {
            layout = new QHBoxLayout{this};


            currentBrushColourLabel = new QLabel{this};
            currentBrushColourLabel->setText("Current Colour: ");

            currentBrushColourButton = new QPushButton{this};
            currentBrushColourButton->setStyleSheet(createStyleSheet_CurrentBrushColour(QColor{0, 0, 0}));

            currentBrushWidthLabel = new QLabel{this};
            currentBrushWidthLabel->setText("Brush Width (in pixels): ");

            currentBrushWidthLineEdit = new QLineEdit{this};
            currentBrushWidthLineEdit->setText("");

            colourDialog = new QColorDialog{this};

            layout->addWidget(currentBrushColourLabel);
            layout->addWidget(currentBrushColourButton, Qt::AlignLeft);

            layout->addWidget(currentBrushWidthLabel);
            layout->addWidget(currentBrushWidthLineEdit);

            setLayout(layout);

            connect(currentBrushColourButton, SIGNAL(clicked()), this, SLOT(handleColourButtonPressed()));

            connect(colourDialog, &QColorDialog::colorSelected, [this](QColor selectedColour)
            {
                currentBrushColourButton->setStyleSheet(createStyleSheet_CurrentBrushColour(selectedColour));

                brush->setPaintTypeSolid( brush->getPaintImage().size(), selectedColour);
            });

            connect(currentBrushWidthLineEdit, &QLineEdit::returnPressed, [this]()
            {
                bool ok = false;

                int enteredWidth = currentBrushWidthLineEdit->text().toInt(&ok);

                if(!ok || (enteredWidth > selectedTextureSize.width() || enteredWidth > selectedTextureSize.height() || enteredWidth <= 0))
                {
                    QMessageBox::warning(this, tr("Texture Atlas Creator"), tr("You have entered an invalid brush dimension. \n"
                                        "The width must be greater than 0 and not greater than either the selected texture width or height!"),
                                         QMessageBox::Ok);
                }
                else
                {
                    QColor brushColour = brush->getPaintImage().pixelColor(0, 0);

                    brush->setPaintTypeSolid(QSize{enteredWidth, enteredWidth}, brushColour);
                }
            });
        }

        void CurrentBrushSettings::showDifferentBrush(const PaintFunctions::Brush &brush)
        {
            this->brush = const_cast<PaintFunctions::Brush*>(&brush);

            QColor brushColour = brush.getPaintImage().pixelColor(0, 0);

            currentBrushColourButton->setStyleSheet(createStyleSheet_CurrentBrushColour(brushColour));

            currentBrushWidthLineEdit->setText(QString::fromStdString(std::to_string(brush.getPaintImage().size().width())));
        }

        void CurrentBrushSettings::updateSelectedTextureSize(QSize size, QSize brushSize)
        {
            selectedTextureSize = size;

            if(size.width() == -1)
            {
                currentBrushWidthLineEdit->setText("No texture selected");

                currentBrushWidthLineEdit->setEnabled(false);
            }
            else
            {
                currentBrushWidthLineEdit->setText(QString::fromStdString(std::to_string(brushSize.width())));

                currentBrushWidthLineEdit->setEnabled(true);
            }
        }

        void CurrentBrushSettings::handleColourButtonPressed()
        {
            colourDialog->show();
        }

        QString CurrentBrushSettings::createStyleSheet_CurrentBrushColour(QColor colour) const
        {
            QString styleSheet;

            styleSheet += QString("background-color:rgb(%1,%2,%3);").arg(colour.red()).arg(colour.green()).arg(colour.blue());
            styleSheet += "border: 1px solid black;";
            styleSheet += "border-radius: 5px;";

            return styleSheet;
        }
    }
}