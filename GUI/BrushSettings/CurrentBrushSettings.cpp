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
        // Beginning of public functions

        CurrentBrushSettings::CurrentBrushSettings(QWidget *parent)
                                :
                                    QWidget{parent},
                                    layout{new QHBoxLayout{this}},
                                    currentBrushColourLabel{new QLabel{this}},
                                    currentBrushColourButton{new QPushButton{this}},
                                    currentBrushWidthLabel{new QLabel{this}},
                                    currentBrushWidthLineEdit{new QLineEdit{this}},
                                    colourDialog{new QColorDialog{this}}

        {
            currentZoom = TextureLogic::Zoom::Normal;

            // Setup the GUI components and add them to a layout as required

            currentBrushColourLabel->setText("Current Colour: ");

            currentBrushColourButton->setStyleSheet(createStyleSheet_CurrentBrushColour(QColor{0, 0, 0}));

            currentBrushWidthLabel->setText("Brush Width (in pixels): ");

            currentBrushWidthLineEdit->setText("");

            layout->addWidget(currentBrushColourLabel);
            layout->addWidget(currentBrushColourButton, Qt::AlignLeft);

            layout->addWidget(currentBrushWidthLabel);
            layout->addWidget(currentBrushWidthLineEdit);

            setLayout(layout);

            // Show a colour dialog if the colour brush button is pressed
            connect(currentBrushColourButton, SIGNAL(clicked()), this, SLOT(handleColourButtonPressed()));

            //  Update the brush to the selected colour if a new one is chosen
            connect(colourDialog, SIGNAL(colorSelected(QColor)), this, SLOT(updateBrushColour(QColor)));

            // Update the brush size to the new size specified in the appropriate QLabel
            connect(currentBrushWidthLineEdit, SIGNAL(returnPressed()), this, SLOT(updateBrushSize()));
        }

        // Beginning of public functions

        void CurrentBrushSettings::showDifferentBrush(const PaintFunctions::Brush &brush)
        {
            this->brush = const_cast<PaintFunctions::Brush*>(&brush);

            // The brush colour is consistent through its shape, so it does not matter the coordinates to receive brush colour
            QColor brushColour = brush.getPaintImage(currentZoom).pixelColor(0, 0);

            // Show the properties of the new brush in this widget
            currentBrushColourButton->setStyleSheet(createStyleSheet_CurrentBrushColour(brushColour));

            currentBrushWidthLineEdit->setText(QString::fromStdString(std::to_string(brush.getPaintImage(currentZoom).size().width())));
        }

        void CurrentBrushSettings::updateSelectedTextureSize(QSize size, QSize brushSize)
        {
            // Update what the current texture size is so that the next time the user manually enters a brush width,
            // it can be checked to make sure that that width is valid.
            selectedTextureSize = size;

            if(size.width() == -1) // No texture is selected in the texture atlas, thus no texture to paint. Brush width cannot be set until a texture is selected.
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

        void CurrentBrushSettings::zoomChanged(TextureLogic::Zoom newZoom)
        {
            currentZoom = newZoom;

            // With a different zoom, the brush changes its size so that it remains relative to the size of the texture,
            // which changes when the zoom does. This is displayed as required in this widget.
            currentBrushWidthLineEdit->setText(QString::number(brush->getPaintImage(currentZoom).width()));
        }

        // Beginning of private slots

        void CurrentBrushSettings::handleColourButtonPressed()
        {
            colourDialog->show();
        }

        void CurrentBrushSettings::updateBrushColour(QColor selectedColour)
        {
            currentBrushColourButton->setStyleSheet(createStyleSheet_CurrentBrushColour(selectedColour));

            brush->setPaintTypeSolid(currentZoom, brush->getPaintImage(currentZoom).size(), selectedColour);
        }

        void CurrentBrushSettings::updateBrushSize()
        {
            bool validIntegerSizeSpecified = false;

            int enteredWidth = currentBrushWidthLineEdit->text().toInt(&validIntegerSizeSpecified);

            if(!validIntegerSizeSpecified || (enteredWidth > selectedTextureSize.width() || enteredWidth > selectedTextureSize.height() || enteredWidth <= 0))
            {
                QMessageBox::warning(this,
                                 tr("Texture Atlas Creator"),
                                 tr("You have entered an invalid brush dimension. \n"
                                    "The width must be greater than 0 and not greater than either the selected texture width or height!"),
                                     QMessageBox::Ok);

                // Visually show the brush size as what it was before the user tried to change the brush size
                currentBrushWidthLineEdit->setText(QString::fromStdString(std::to_string(brush->getPaintImage(currentZoom).size().width())));
            }
            else
            {
                // Brush colour does not change when the size is changed
                QColor brushColour = brush->getPaintImage(currentZoom).pixelColor(0, 0);

                // Visually the text displaying size of the brush has changed, but the brush itself needs to be changed to the new size
                brush->setPaintTypeSolid(currentZoom, QSize{enteredWidth, enteredWidth}, brushColour);
            }
        }

        // Beginning of private functions

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