//
// Created by BinyBrion on 10/2/2019.
//

#include "CurrentBrushSettings.h"

#include <QLabel>
#include <QHBoxLayout>
#include <QColorDialog>
#include <QtWidgets/QMessageBox>
#include "PaintFunctions/Brush.h"
#include "ui_brushSettings.h"

namespace GUI
{
    namespace BrushSettings
    {
        // Beginning of public functions

        CurrentBrushSettings::CurrentBrushSettings(QWidget *parent)
                                :
                                    QWidget{parent},
                                    ui{new Ui::BrushSettings},
                                    colourDialog{new QColorDialog{this}}

        {
            ui->setupUi(this);

            previousDiffuseColour = QColor{255, 255, 255};

            previousSpecularDrawingColour = QColor{255, 255, 255};

            currentZoom = TextureLogic::Zoom::Normal;

            ui->paintAreaZoom->setText("Paint Area Zoom: " + QString::number(::TextureLogic::GetZoomValue(currentZoom) * 100) + "%");
            ui->specularDisplayZoom->setText("Specular Area Zoom: " + QString::number(::TextureLogic::GetZoomValue(currentZoom) * 100) + "%");
            ui->textureZoomDimensions->setText("Texture Zoom Dimensions: N/A:");

            // Show a colour dialog if the colour brush button is pressed
            connect(ui->brushColourButton, SIGNAL(clicked()), this, SLOT(handleColourButtonPressed()));

            //  Update the brush to the selected colour if a new one is chosen
            connect(colourDialog, SIGNAL(colorSelected(QColor)), this, SLOT(updateBrushColour(QColor)));

            // Update the brush size to the new size specified in the appropriate QLabel
            connect(ui->brushWidthLineEdit, SIGNAL(returnPressed()), this, SLOT(updateBrushSize()));

            connect(ui->editSpecularTexture, SIGNAL(toggled(bool)), this, SLOT(switchDrawingMode(bool)));
        }

        // Beginning of public functions

        void CurrentBrushSettings::setBrushReference(const PaintFunctions::Brush &brush)
        {
            this->brush = const_cast<PaintFunctions::Brush*>(&brush);

            // The brush colour is consistent through its shape, so it does not matter the coordinates to receive brush colour
            QColor brushColour = brush.getPaintImage(currentZoom).pixelColor(0, 0);

            // Show the properties of the new brush in this widget
            ui->brushColourButton->setStyleSheet(createStyleSheet_CurrentBrushColour(brushColour));

            ui->brushWidthLineEdit->setText(QString::fromStdString(std::to_string(brush.getPaintImage(currentZoom).size().width())));
        }

        void CurrentBrushSettings::updateSelectedTextureSize(QSize size, QSize brushSize)
        {
            // Update what the current texture size is so that the next time the user manually enters a brush width,
            // it can be checked to make sure that that width is valid.
            selectedTextureSize = size;

            if(size.width() == -1) // No texture is selected in the texture atlas, thus no texture to paint. Brush width cannot be set until a texture is selected.
            {
                ui->brushWidthLineEdit->setText("No texture selected");

                ui->brushWidthLineEdit->setEnabled(false);

                ui->textureZoomDimensions->setText("Texture Zoom Dimensions: N/A:");
            }
            else
            {
                ui->brushWidthLineEdit->setText(QString::fromStdString(std::to_string(brushSize.width())));

                ui->brushWidthLineEdit->setEnabled(true);

                ui->textureZoomDimensions->setText("Texture Zoom Dimensions: " + QString::number(selectedTextureSize.width()) + " x " + QString::number(selectedTextureSize.height()));
            }
        }

        void CurrentBrushSettings::zoomChangedDisplayArea(::TextureLogic::Zoom newZoom)
        {
            ui->specularDisplayZoom->setText("Specular Area Zoom: " + QString::number(::TextureLogic::GetZoomValue(newZoom) * 100) + "%");
        }

        void CurrentBrushSettings::zoomChangedPaintArea(::TextureLogic::Zoom newZoom)
        {
            float zoomFactor = TextureLogic::GetZoomValue(newZoom) / TextureLogic::GetZoomValue(currentZoom);

            currentZoom = newZoom;

            // With a different zoom, the brush changes its size so that it remains relative to the size of the texture,
            // which changes when the zoom does. This is displayed as required in this widget.
            ui->brushWidthLineEdit->setText(QString::number(brush->getPaintImage(currentZoom).width()));

            ui->paintAreaZoom->setText("Paint Area Zoom: " + QString::number(::TextureLogic::GetZoomValue(newZoom) * 100) + "%");

            selectedTextureSize *= zoomFactor;

            ui->textureZoomDimensions->setText("Texture Zoom Dimensions: " + QString::number(selectedTextureSize.width()) + " x " + QString::number(selectedTextureSize.height()));
        }

        // Beginning of private slots

        void CurrentBrushSettings::handleColourButtonPressed()
        {
            colourDialog->show();
        }

        void CurrentBrushSettings::switchDrawingMode(bool value)
        {
            if(brush != nullptr) // Could be called before the brush reference is set, depending on the behaviour of Qt
            {
                brush->setPaintingSpecularTexture(value);

                if(brush->getPaintingSpecularTexture())
                {
                    colourDialog->setCurrentColor(previousSpecularDrawingColour);

                    ui->brushColourButton->setStyleSheet(createStyleSheet_CurrentBrushColour(previousSpecularDrawingColour));

                    brush->setPaintTypeSolid(currentZoom, brush->getPaintImage(currentZoom).size(), previousSpecularDrawingColour);
                }
                else
                {
                    colourDialog->setCurrentColor(previousDiffuseColour);

                    ui->brushColourButton->setStyleSheet(createStyleSheet_CurrentBrushColour(previousDiffuseColour));

                    brush->setPaintTypeSolid(currentZoom, brush->getPaintImage(currentZoom).size(), previousDiffuseColour);
                }
            }
        }

        void CurrentBrushSettings::updateBrushColour(QColor selectedColour)
        {
            // In specular painting mode, only grey-scale values area allowed
            if(brush->getPaintingSpecularTexture() &&
              (selectedColour.red() != selectedColour.green() ||
               selectedColour.red() != selectedColour.blue() ||
               selectedColour.green() != selectedColour.blue()))
            {
                int averageColorValue = (selectedColour.red() + selectedColour.green() + selectedColour.blue()) / 3;

                selectedColour = QColor{averageColorValue, averageColorValue, averageColorValue};

                QString newColourString = QString::number(averageColorValue) + " , " + QString::number(averageColorValue) + " , " + QString::number(averageColorValue) + ".";

                QMessageBox::warning(this, "Invalid colour chosen",
                        "When drawing in specular mode, a grey-scale colour must be chosen.\n"
                        "The chosen colour has been averaged to: " + newColourString, QMessageBox::Ok);
            }

            ui->brushColourButton->setStyleSheet(createStyleSheet_CurrentBrushColour(selectedColour));

            brush->setPaintTypeSolid(currentZoom, brush->getPaintImage(currentZoom).size(), selectedColour);

            if(brush->getPaintingSpecularTexture())
            {
                previousSpecularDrawingColour = selectedColour;
            }
            else
            {
                previousDiffuseColour = selectedColour;
            }
        }

        void CurrentBrushSettings::updateBrushSize()
        {
            bool validIntegerSizeSpecified = false;

            int enteredWidth = ui->brushWidthLineEdit->text().toInt(&validIntegerSizeSpecified);

            if(!validIntegerSizeSpecified || (enteredWidth > selectedTextureSize.width() || enteredWidth > selectedTextureSize.height() || enteredWidth <= 0))
            {
                QMessageBox::warning(this,
                                 tr("Texture Atlas Creator"),
                                 tr("You have entered an invalid brush dimension. \n"
                                    "The width must be greater than 0 and not greater than either the selected texture width or height!"),
                                     QMessageBox::Ok);

                // Visually show the brush size as what it was before the user tried to change the brush size
                ui->brushWidthLineEdit->setText(QString::fromStdString(std::to_string(brush->getPaintImage(currentZoom).size().width())));
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