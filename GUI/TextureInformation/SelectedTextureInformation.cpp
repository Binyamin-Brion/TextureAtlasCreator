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
            // There is a need to use both the actual enum for the format of the string (as when saving) and its string
            // representation (for use with the GUI). To ease this conversion, all of the supported Image formats are pushed
            // to this vector along with its string representation. Whenever there is need to convert from one to the other,
            // search the vector using the information available. For example, if the string "Mono" is known, search the
            // vector for the word "Mono", which will yield the respective enum.

            internalFormatPairs.push_back(InternalFormatPair{QImage::Format_Invalid, "Invalid"});
            internalFormatPairs.push_back(InternalFormatPair{QImage::Format_Mono, "Mono"});
            internalFormatPairs.push_back(InternalFormatPair{QImage::Format_MonoLSB, "MonoLSB"});
            internalFormatPairs.push_back(InternalFormatPair{QImage::Format_Indexed8, "Indexed8"});
            internalFormatPairs.push_back(InternalFormatPair{QImage::Format_RGB32, "RGB32"});
            internalFormatPairs.push_back(InternalFormatPair{QImage::Format_ARGB32, "ARGB32"});
            internalFormatPairs.push_back(InternalFormatPair{QImage::Format_ARGB32_Premultiplied, "ARGB32_Premultiplied"});
            internalFormatPairs.push_back(InternalFormatPair{QImage::Format_RGB16, "RGB16"});
            internalFormatPairs.push_back(InternalFormatPair{QImage::Format_ARGB8565_Premultiplied, "ARGB8565_Premultiplied"});
            internalFormatPairs.push_back(InternalFormatPair{QImage::Format_RGB666, "RGB666"});
            internalFormatPairs.push_back(InternalFormatPair{QImage::Format_ARGB6666_Premultiplied, "ARGB666_Premultiplied"});
            internalFormatPairs.push_back(InternalFormatPair{QImage::Format_RGB555, "RGB555"});
            internalFormatPairs.push_back(InternalFormatPair{QImage::Format_ARGB8555_Premultiplied, "ARGB8555_Premultiplied"});
            internalFormatPairs.push_back(InternalFormatPair{QImage::Format_RGB888, "RGB888"});
            internalFormatPairs.push_back(InternalFormatPair{QImage::Format_RGB444, "RGB444"});
            internalFormatPairs.push_back(InternalFormatPair{QImage::Format_ARGB4444_Premultiplied, "ARGB444_Premultiplied"});
            internalFormatPairs.push_back(InternalFormatPair{QImage::Format_RGBX8888, "RGBX888"});
            internalFormatPairs.push_back(InternalFormatPair{QImage::Format_RGBA8888, "RGBA8888"});
            internalFormatPairs.push_back(InternalFormatPair{QImage::Format_RGBA8888_Premultiplied, "RGBA8888_Premultiplied"});
            internalFormatPairs.push_back(InternalFormatPair{QImage::Format_BGR30, "BGR30"});
            internalFormatPairs.push_back(InternalFormatPair{QImage::Format_RGB30, "RGB30"});
            internalFormatPairs.push_back(InternalFormatPair{QImage::Format_A2RGB30_Premultiplied, "A2RGB30_Premultiplied"});
            internalFormatPairs.push_back(InternalFormatPair{QImage::Format_A2BGR30_Premultiplied, "A2BGR30_Premultiplied"});
            internalFormatPairs.push_back(InternalFormatPair{QImage::Format_Alpha8, "Alpha8"});
            internalFormatPairs.push_back(InternalFormatPair{QImage::Format_Grayscale8, "Grayscale8"});

            // This is for convenience- whenever a new image extension needs to be added, just add it to this
            // vector which will ensure it is shown in the GUI

            imageExtensions.push_back("png");
            imageExtensions.push_back("jpg");
            imageExtensions.push_back("bmp");
            imageExtensions.push_back("gif");
            imageExtensions.push_back("svg");

            ui->setupUi(this);

            // Whenever the text of the texture description area changes, then it usually means that the user
            // typed a description in. However, because internally the texture does not store a "No Description"
            // description internally, the texture's description should only be updated when it is a different description.
            // Note that when a texture with no description is selected, the plaintextedit is updated to show "No Description"
            // which would inadvertently set the texture's description. Hence the check in the connect function.

           connect(ui->plainTextEdit, &QPlainTextEdit::textChanged, [this]()
           {
               if(texture != nullptr)
               {
                   if(ui->plainTextEdit->toPlainText() != "No Description")
                   {
                        texture->setTextureDescription(ui->plainTextEdit->toPlainText(), {});
                   }
               }
           });

           connect(ui->textureNameLineEdit, &QLineEdit::returnPressed, [this]()
           {
               printf("%s, %s \n",ui->textureNameLineEdit->text().toStdString().c_str() , texture->textureName().toStdString().c_str());

                if(ui->textureNameLineEdit->text().contains("Modified"))
                {
                    QMessageBox::warning(this, tr("Error: Invalid Texture Name"), "Cannot have a texture name with the word Modified.", QMessageBox::Ok);
                }
                else if(ui->textureNameLineEdit->text() == texture->textureName())
                {
                    QMessageBox::warning(this, tr("Error: Invalid Texture Name"), "You must choose unique name, different than an existing texture's!.", QMessageBox::Ok);
                }
                else
                {
                    if(texture != nullptr)
                    {
                        texture->setTextureName(ui->textureNameLineEdit->text(), {});
                    }
                }
           });

           connect(ui->pushButton, &QPushButton::pressed, [this]()
           {
               if(ui->textureNameLineEdit->text().isEmpty())
               {
                   QMessageBox::warning(this, tr("Error: No texture name given."), "Enter a texture name first before trying to save.", QMessageBox::Ok);
               }
               else if(texture == nullptr)
               {
                   QMessageBox::warning(this, tr("Error: No Texture Selected"), "You must first selected a texture.", QMessageBox::Ok);
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
           });

            for(auto i = internalFormatPairs.cbegin() + 1; i != internalFormatPairs.cend(); ++i)
            {
                ui->interalFormatComboBox->addItem(i->formatStringRepresentation);
            }

            ui->interalFormatComboBox->setCurrentIndex(3);

            for(const auto &i : imageExtensions)
            {
                ui->formatComboBox->addItem(i);
            }

            ui->saveQualityLineEdit->setText("100");
            ui->saveQualitySlider->setMinimum(0);
            ui->saveQualitySlider->setMaximum(100);
            ui->saveQualitySlider->setValue(100);
            ui->saveQualitySlider->setSingleStep(5);

            connect(ui->saveQualitySlider, &QSlider::sliderMoved, [this](int newValue)
            {
                ui->saveQualityLineEdit->setText(QString::number(newValue));
            });

            connect(ui->saveQualitySlider, &SaveQualitySlider::sliderValueDifferent, [this](int value)
            {
                ui->saveQualityLineEdit->setText(QString::number(value));
            });

            connect(ui->saveQualityLineEdit, &QLineEdit::returnPressed, [this]()
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
            });

            resetDefaultLabels();
        }

        void SelectedTextureInformation::selectedTextureModified()
        {
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
        }

        void SelectedTextureInformation::setTexture(const TextureLogic::Texture *texture)
        {
            this->texture = const_cast<TextureLogic::Texture*>(texture);

            if(texture == nullptr)
            {
                resetDefaultLabels();

                return;
            }

            ui->plainTextEdit->setEnabled(true);

            ui->pushButton->setEnabled(true);
            ui->formatComboBox->setEnabled(true);
            ui->interalFormatComboBox->setEnabled(true);
            ui->textureNameLineEdit->setEnabled(true);
            ui->saveQualityLineEdit->setEnabled(true);
            ui->saveQualitySlider->setEnabled(true);

            const QImage& textureImage = texture->getImage(TextureLogic::Zoom::Normal);

            ui->textureNameLineEdit->setText(texture->textureName());
            ui->textureLocationLabel->setText("Original Texture Location: " + texture->textureLocation());
            ui->textureWidthLabel->setText("Texture Width: " + QString::number(textureImage.width()));
            ui->textureHeightLabel->setText("Texture Height: " + QString::number(textureImage.height()));
            ui->textureDepthLabel->setText("Texture Depth: " + QString::number(textureImage.depth()));
            ui->textureSizeBytesLabel->setText("Texture Size Bytes: " + QString::number(textureImage.byteCount()));
            ui->lastSavedToLabel->setText("Last Saved: " + texture->textureLocation());

            QFileInfo fileInfo{texture->textureLocation()};

            ui->lastSavedLabel->setText("Last Saved To: " + fileInfo.lastModified().toString());

            if(texture->getTextureDescription({}).isEmpty())
            {
                ui->plainTextEdit->setPlainText("No Description");
            }
            else
            {
                ui->plainTextEdit->setPlainText(texture->getTextureDescription({}));
            }

            auto texturePixelInternalFormat = std::find_if(internalFormatPairs.begin(), internalFormatPairs.end(), [texture](const InternalFormatPair &internalFormatPair)
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
        }

        void SelectedTextureInformation::resetDefaultLabels()
        {
            ui->textureLocationLabel->setText("Original Texture Location: N/A");
            ui->textureWidthLabel->setText("Texture Width: N/A");
            ui->textureHeightLabel->setText("Texture Height: N/A");
            ui->textureDepthLabel->setText("Texture Depth: N/A");
            ui->textureSizeBytesLabel->setText("Texture Size Bytes: N/A");
            ui->lastSavedLabel->setText("Last Saved: N/A");
            ui->lastSavedToLabel->setText("Last Saved To: NA");
            ui->plainTextEdit->setPlainText("No Texture Selected");
            ui->plainTextEdit->setEnabled(false);

            ui->pushButton->setEnabled(false);
            ui->formatComboBox->setEnabled(false);
            ui->interalFormatComboBox->setEnabled(false);
            ui->textureNameLineEdit->setEnabled(false);
            ui->saveQualityLineEdit->setEnabled(false);
            ui->saveQualitySlider->setEnabled(false);
        }

        void SelectedTextureInformation::saveImage(const QString &newFileLocation)
        {
            auto texturePixelInternalFormat = std::find_if(internalFormatPairs.begin(), internalFormatPairs.end(), [this](const InternalFormatPair &internalFormatPair)
            {
                return internalFormatPair.formatStringRepresentation == ui->interalFormatComboBox->currentText();
            });

            if(texturePixelInternalFormat == internalFormatPairs.end())
            {
                Q_ASSERT_X(false, __PRETTY_FUNCTION__, "Unknown texture internal format encountered");
            }

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

                emit reuploadTexture(previousTextureLocation, texture);

            }
        }
    }
}