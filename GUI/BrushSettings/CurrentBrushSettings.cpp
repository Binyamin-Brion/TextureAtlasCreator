//
// Created by BinyBrion on 10/2/2019.
//

#include "CurrentBrushSettings.h"

#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QColorDialog>

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
            });
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