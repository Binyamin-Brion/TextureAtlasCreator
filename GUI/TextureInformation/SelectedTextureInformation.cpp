//
// Created by BinyBrion on 10/3/2019.
//

#include <QtWidgets/QMessageBox>
#include <QtCore/QFileInfo>
#include <QtCore/QDateTime>
#include <QtWidgets/QFileDialog>
#include "SelectedTextureInformation.h"
#include "TextureLogic/Texture.h"

namespace GUI
{
    namespace TextureInformation
    {
        SelectedTextureInformation::SelectedTextureInformation(QWidget *parent) : QWidget{parent}, ui{new Ui::SelectedTextureInformation}
        {
            internalFormatPairs = TextureHelperFunctions::internalFormatPairRepresentations();

            imageExtensions = TextureHelperFunctions::listImageExtensions();

            zoomPairs = TextureLogic::getZoomPairs();

            ui->setupUi(this);

           connect(ui->textureDescription, SIGNAL(textChanged()), this, SLOT(textureDescriptionChanged()));

           connect(ui->textureNameLineEdit, SIGNAL(returnPressed()), this, SLOT(textureNameChanged()));

           connect(ui->saveTextureToButton, SIGNAL(pressed()), this, SLOT(saveTexturePressed()));

           connect(ui->intersectionBorderWidthLineEdit, SIGNAL(returnPressed()), this, SLOT(intersectionBorderWidthChanged()));

           connect(ui->selectionBorderWidthLineEdit, SIGNAL(returnPressed()), this, SLOT(selectionBorderWidthChanged()));

            // Next two connect statements ensure that the line edit controlling the export image quality
            // and the associated slider are synchronized in the values they are showing

            connect(ui->saveQualitySlider, &QSlider::sliderMoved, [this](int newValue) { ui->saveQualityLineEdit->setText(QString::number(newValue)); });

            connect(ui->saveQualitySlider, &SaveQualitySlider::sliderValueDifferent, [this](int value) { ui->saveQualityLineEdit->setText(QString::number(value)); });

            // Ensure valid quality factor is entered into the line edit associated with texture save quality
            connect(ui->saveQualityLineEdit, SIGNAL(returnPressed()), this, SLOT(saveQualityChanged()));

            connect(ui->intersectionZoomComboxBox, SIGNAL(currentIndexChanged(int)), this, SLOT(intersectionZoomComboBoxChanged(int)));

            connect(ui->selectionZoomComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(selectionZoomComboBoxChanged(int)));

            for(auto &i : zoomPairs)
            {
                 ui->intersectionZoomComboxBox->addItem(i.zoomStringRepresentation);
                 ui->selectionZoomComboBox->addItem(i.zoomStringRepresentation);
            }

            // Display the selected texture border widths at Normal zoom
            ui->intersectionZoomComboxBox->setCurrentIndex(2);
            ui->selectionZoomComboBox->setCurrentIndex(2);

            // When adding the internal format representations, the format "Invalid Format" is a weird option to be able to select.
            // Therefore the user should not be able to select it.
            for(auto i = internalFormatPairs.cbegin() + 1; i != internalFormatPairs.cend(); ++i)
            {
                ui->interalFormatComboBox->addItem(i->formatStringRepresentation);
            }

            // By default, set the texture format that a texture is saved as RGB32- since the first format was not
            // not added to the combo box, the index of RGB32 is 3, not 4
            ui->interalFormatComboBox->setCurrentIndex(3);

            for(const auto &i : imageExtensions)
            {
                ui->formatComboBox->addItem(i);
            }

            // By default the export texture quality should be 100
            ui->saveQualityLineEdit->setText("100");

            resetDefaultLabels();

            // With no texture selected there is no format right now
            selectedTextureFormat = QImage::Format_Invalid;
        }

        QImage::Format SelectedTextureInformation::getSelectedTextureFormat() const
        {
            return selectedTextureFormat;
        }

        void SelectedTextureInformation::setIntersectionWidthLineEdit(unsigned int previousBorderWidth)
        {
            ui->intersectionBorderWidthLineEdit->setText(QString::number(previousBorderWidth));
        }

        void SelectedTextureInformation::selectedTextureModified()
        {
            // This check should not be needed as the render area and this widget are synchronized in the widget that is selected.
            // The only way for a texture to be modified is if the render area holds a selected texture, in which case so does this widget
            if(texture != nullptr)
            {
                QString currentTextureName = texture->textureName();

                if(!currentTextureName.contains("_Modified"))
                {
                    currentTextureName += "_Modified";

                    texture->setTextureName(currentTextureName, {});

                    ui->textureNameLineEdit->setText(currentTextureName);

                    ui->textureLocationLabel->setText("Texture Location: " + texture->textureLocation());
                }
            }
            else
            {
                Q_ASSERT_X(false, __PRETTY_FUNCTION__, "Invalid if branch taken");
            }
        }

        void SelectedTextureInformation::setTexture(const TextureLogic::Texture *texture)
        {
            this->texture = const_cast<TextureLogic::Texture*>(texture);

            // No texture is selected, in which case reset to the default state of showing texture information
            // (Clear any texture-specific text and disable)
            if(texture == nullptr)
            {
                resetDefaultLabels();

                selectedTextureFormat = QImage::Format_Invalid;

                return;
            }

            // Valid texture is selected, and therefore the texture information area widgets
            // should be enabled so the user can interact with them
            ui->textureDescription->setEnabled(true);

            ui->saveTextureToButton->setEnabled(true);
            ui->intersectionBorderWidthLineEdit->setEnabled(true);
            ui->selectionBorderWidthLineEdit->setEnabled(true);
            ui->formatComboBox->setEnabled(true);
            ui->interalFormatComboBox->setEnabled(true);
            ui->textureNameLineEdit->setEnabled(true);
            ui->saveQualityLineEdit->setEnabled(true);
            ui->saveQualitySlider->setEnabled(true);
            ui->intersectionZoomComboxBox->setEnabled(true);
            ui->selectionZoomComboBox->setEnabled(true);

            // Note that when displaying information about a texture, a sample image is needed from information
            // can be queried from. For everything but the image dimensions, any zoom can be selected, as the
            // texture format is the same. However, to be consistent with what the user loaded form disk, the
            // normal zoom is selected to ensure that the dimensions shown reflect what is on the disk.
            const QImage& textureImage = texture->getImage(TextureLogic::Zoom::Normal);

            ui->intersectionBorderWidthLineEdit->setText(QString::number(texture->getIntersectionBorderWidth(zoomPairs[ui->intersectionZoomComboxBox->currentIndex()].zoom)));
            ui->selectionBorderWidthLineEdit->setText(QString::number(texture->getSelectedBorderWidth(zoomPairs[ui->selectionZoomComboBox->currentIndex()].zoom)));
            ui->textureNameLineEdit->setText(texture->textureName());
            ui->textureLocationLabel->setText("Original Texture Location: " + texture->textureLocation());
            ui->textureWidthLabel->setText("Texture Width: " + QString::number(textureImage.width()));
            ui->textureHeightLabel->setText("Texture Height: " + QString::number(textureImage.height()));
            ui->textureDepthLabel->setText("Texture Depth: " + QString::number(textureImage.depth()));
            ui->textureSizeBytesLabel->setText("Texture Size Bytes: " + QString::number(textureImage.byteCount()));
            ui->lastSavedToLabel->setText("Last Saved: " + texture->textureLocation());

            // Get the last time the selected texture was modified and display it to the user.
            // Note that the location of the texture used for loading the image is queried.
            QFileInfo fileInfo{texture->textureLocation()};

            ui->lastSavedLabel->setText("Last Saved To: " + fileInfo.lastModified().toString());

            if(texture->getTextureDescription().isEmpty())
            {
                ui->textureDescription->setPlainText("No Description");
            }
            else
            {
                ui->textureDescription->setPlainText(texture->getTextureDescription());
            }

            // Find and display format information to the user
            auto texturePixelInternalFormat = std::find_if(internalFormatPairs.begin(), internalFormatPairs.end(), [texture](const TextureHelperFunctions::InternalFormatPair &internalFormatPair)
            {
                return internalFormatPair.format == texture->getImage(TextureLogic::Zoom::Normal).format();
            });

            if(texturePixelInternalFormat == internalFormatPairs.end())
            {
                Q_ASSERT_X(false, __PRETTY_FUNCTION__, "Unknown texture internal format encountered");
            }

            ui->textureInternalFormatLabel->setText("Texture Internal Format: " + texturePixelInternalFormat->formatStringRepresentation);
            ui->interalFormatComboBox->setCurrentIndex(std::distance(internalFormatPairs.begin(), texturePixelInternalFormat) - 1);

            ui->textureFormatLabel->setText("Texture Format: " + texture->textureFormat());

            selectedTextureFormat = texture->getImage(TextureLogic::Zoom::Normal).format();
        }

        // Beginning of private slots

        void SelectedTextureInformation::intersectionBorderWidthChanged()
        {
            int chosenWidth = checkValidBorderWidth(ui->intersectionBorderWidthLineEdit, 50.f * normalZoomFactorValue(zoomPairs[ui->selectionZoomComboBox->currentIndex()].zoom));

            if(chosenWidth == -1)
            {
                return;
            }

            emit newIntersectionBorderWidth(texture, zoomPairs[ui->intersectionZoomComboxBox->currentIndex()].zoom, ui->intersectionBorderWidthLineEdit->text().toUInt());
        }

        void SelectedTextureInformation::intersectionZoomComboBoxChanged(int index)
        {
            // See textureDescriptionChanged() for why this check exists
            if(texture != nullptr)
            {
                ui->intersectionBorderWidthLineEdit->setText(QString::number(texture->getIntersectionBorderWidth(zoomPairs[index].zoom)));
            }
        }

        void SelectedTextureInformation::saveQualityChanged()
        {
            bool ok;

            int value = ui->saveQualityLineEdit->text().toInt(&ok);

            if(!ok || (value < 0 || value > 100))
            {
                QMessageBox::warning(this, tr("Error: Invalid Quality Value"), "Enter a quality value between and including 0 to 100", QMessageBox::Ok);
            }
            else
            {
                ui->saveQualitySlider->setValue(ui->saveQualityLineEdit->text().toInt());
            }
        }

        void SelectedTextureInformation::saveTexturePressed()
        {
            if(ui->textureNameLineEdit->text().isEmpty())
            {
                QMessageBox::warning(this, tr("Error: No texture name given."), "Enter a texture name first before trying to save.", QMessageBox::Ok);
            }
            else if(texture == nullptr) // This if statement should not occur as the push button is disabled when there is no texture selected. But just in case.
            {
                QMessageBox::warning(this, tr("Error: No Texture Selected"), "You must first selected a texture.", QMessageBox::Ok);

                Q_ASSERT_X(false, __PRETTY_FUNCTION__, "Invalid if branch taken");
            }
            else
            {
                QString newFileLocation = QFileDialog::getSaveFileName(this, tr("Save Image"), "C:/Users/BinyBrion/Pictures/" + ui->textureNameLineEdit->text(), tr("Image ()"));

                if(!newFileLocation.isEmpty())
                {
                    newFileLocation += '.';
                    newFileLocation += ui->formatComboBox->currentText();

                    if(QFileInfo::exists(newFileLocation) && QFileInfo(newFileLocation).isFile())
                    {
                        int response = QMessageBox::warning(this, tr("Error: Image Already Exists"), "An image by the selected name already exists. \n"
                                                                                                     "Do you wish to overwrite it?", QMessageBox::Ok | QMessageBox::Cancel);

                        if(response == QMessageBox::Ok)
                        {
                            QFile file{newFileLocation};

                            if(!file.remove())
                            {
                                QMessageBox::critical(this, tr("Error: Unable to write image"), "Failed to overwrite the existing file.", QMessageBox::Ok);
                            }
                            else
                            {
                                saveImage(newFileLocation);
                            }
                        }
                    }
                    else
                    {
                        saveImage(newFileLocation);
                    }
                }
            }
        }

        void SelectedTextureInformation::selectionBorderWidthChanged()
        {
            int chosenWidth = checkValidBorderWidth(ui->selectionBorderWidthLineEdit, 10.f * normalZoomFactorValue(zoomPairs[ui->selectionZoomComboBox->currentIndex()].zoom));

            if(chosenWidth == -1)
            {
                return;
            }

            emit newSelectionBorderWidth(texture, zoomPairs[ui->selectionZoomComboBox->currentIndex()].zoom, chosenWidth);
        }

        void SelectedTextureInformation::selectionZoomComboBoxChanged(int index)
        {
            // See textureDescriptionChanged() for why this check exists
            if(texture != nullptr)
            {
                ui->selectionBorderWidthLineEdit->setText(QString::number(texture->getSelectedBorderWidth(zoomPairs[index].zoom)));
            }
        }

        void SelectedTextureInformation::textureDescriptionChanged()
        {
            // The line-edit calls this function as the text "No Description" during program startup is made after the creation of this connection.
            // Rather than reordering the code in the constructor, just make this function more robust by only allowing this function's logic to
            // be executed after checking that a texture is selected.
            if(texture != nullptr)
            {
                if(ui->textureDescription->toPlainText() != "No Description")
                {
                    texture->setTextureDescription(ui->textureDescription->toPlainText(), {});
                }
            }
        }

        void SelectedTextureInformation::textureNameChanged()
        {
            // Note that even if return is pressed, the texture is not changed until the texture is saved.
            // This is because due to how textures are loaded when a texture button is pressed (texture location is given to find an image)
            // the user will still not be able to load the old texture with the old name in. Therefore there is no point in doing so now.
            if(ui->textureNameLineEdit->text().contains("Modified"))
            {
                QMessageBox::warning(this, tr("Error: Invalid Texture Name"), "Cannot have a texture name with the word Modified.", QMessageBox::Ok);
            }
            else if(ui->textureNameLineEdit->text() == texture->textureName())
            {
                QMessageBox::warning(this, tr("Error: Invalid Texture Name"), "You must choose unique name, different than an existing texture's!.", QMessageBox::Ok);
            }
        }

        // Beginning of private functions

        int SelectedTextureInformation::checkValidBorderWidth(QLineEdit *lineEdit, int maxValue)
        {
            bool ok;

            int chosenWidth = lineEdit->text().toInt(&ok);

            if(lineEdit->text().isEmpty() || !ok || chosenWidth < 0 || chosenWidth > maxValue)
            {
                lineEdit->setStyleSheet("background-color: rgba(255, 0, 0, 64)");

                return -1;
            }

            lineEdit->setStyleSheet("");

            return chosenWidth;
        }

        float SelectedTextureInformation::normalZoomFactorValue(TextureLogic::Zoom zoom) const
        {
            return TextureLogic::GetZoomValue(TextureLogic::Zoom::Normal) / TextureLogic::GetZoomValue(zoom);
        }

        void SelectedTextureInformation::resetDefaultLabels()
        {
            // This is called during initialization, which is a subset of the scenario of when this function is called:
            // when no texture is selected. In that case, all of the texture information widgets are disabled, which
            // prevents the any querying of a null pointer to a selected texture

            ui->textureLocationLabel->setText("Original Texture Location: N/A");
            ui->textureWidthLabel->setText("Texture Width: N/A");
            ui->textureHeightLabel->setText("Texture Height: N/A");
            ui->textureDepthLabel->setText("Texture Depth: N/A");
            ui->textureSizeBytesLabel->setText("Texture Size Bytes: N/A");
            ui->lastSavedLabel->setText("Last Saved: N/A");
            ui->lastSavedToLabel->setText("Last Saved To: NA");
            ui->textureDescription->setPlainText("No Texture Selected");
            ui->textureDescription->setEnabled(false);

            ui->saveTextureToButton->setEnabled(false);
            ui->intersectionBorderWidthLineEdit->setEnabled(false);
            ui->selectionBorderWidthLineEdit->setEnabled(false);
            ui->formatComboBox->setEnabled(false);
            ui->interalFormatComboBox->setEnabled(false);
            ui->textureNameLineEdit->setEnabled(false);
            ui->saveQualityLineEdit->setEnabled(false);
            ui->saveQualitySlider->setEnabled(false);
            ui->intersectionZoomComboxBox->setEnabled(false);
            ui->selectionZoomComboBox->setEnabled(false);
        }

        void SelectedTextureInformation::saveImage(const QString &newFileLocation)
        {
            auto texturePixelInternalFormat = std::find_if(internalFormatPairs.begin(), internalFormatPairs.end(), [this](const TextureHelperFunctions::InternalFormatPair &internalFormatPair)
            {
                return internalFormatPair.formatStringRepresentation == ui->interalFormatComboBox->currentText();
            });

            if(texturePixelInternalFormat == internalFormatPairs.end())
            {
                Q_ASSERT_X(false, __PRETTY_FUNCTION__, "Unknown texture internal format encountered");
            }

            // If the selected texture has a different format different than what the the user selected to save as,
            // then convert the selected texture to that chosen format. Note however, that this means converting
            // all of the zoom textures as well, as that is "part" of the selected texture.
            if(texturePixelInternalFormat->format != texture->getImage(TextureLogic::Zoom::Normal).format())
            {
                for(auto &i : TextureLogic::AllZoomValues)
                {
                    auto& textureRef = const_cast<QImage&>(texture->getImage(i));

                    textureRef = textureRef.convertToFormat(texturePixelInternalFormat->format);
                }
            }

            bool ok;

            int qualityFactor = ui->saveQualityLineEdit->text().toInt(&ok);

            if(!ok)
            {
                Q_ASSERT_X(false, __PRETTY_FUNCTION__, "Fatal internal error- invalid quality factor specified in line edit!");
            }

            if(!texture->getImage(TextureLogic::Zoom::Normal).save(newFileLocation, nullptr, qualityFactor))
            {
                QString errorMessage = "Failed to save to: " + newFileLocation;

                QMessageBox::critical(this, tr("Error: Unable to write image"), errorMessage, QMessageBox::Ok);
            }
            else
            {
                texture->setTextureName(ui->textureNameLineEdit->text(), {});

                QString previousTextureLocation = texture->textureLocation();

                texture->setTextureLocation(newFileLocation, {});

                ui->textureFormatLabel->setText("Texture Format: " + texture->textureFormat());

                // After saving the texture, it now has a new texture name and location. However, this effectively "erased"
                // the old texture that a texture button refers to. Thus when that button is pressed again, an error saying
                // no texture of that location was loaded will appear. To deal with that, the old texture has to be reuploaded
                // so that in the texturebank a texture with the old details is present.
                // The user must load the saved texture as any other texture in order for a texture button representing the
                // modified texture to be created.

                unsigned int intersectionBorderWidth = texture->getIntersectionBorderWidth(TextureLogic::Zoom::Normal);

                unsigned int selectionBorderWidth = texture->getSelectedBorderWidth(TextureLogic::Zoom::Normal);

                emit reuploadTexture(previousTextureLocation, texture, intersectionBorderWidth, selectionBorderWidth);

            }
        }
    }
}