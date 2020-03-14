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

           connect(ui->textureNameLineEdit, SIGNAL(returnPressed()), this, SLOT(textureNameChanged()));

           connect(ui->intersectionBorderWidthLineEdit, SIGNAL(returnPressed()), this, SLOT(intersectionBorderWidthChanged()));

           connect(ui->selectionBorderWidthLineEdit, SIGNAL(returnPressed()), this, SLOT(selectionBorderWidthChanged()));

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
            static int textureModifyCount = 0;

            // This check should not be needed as the render area and this widget are synchronized in the widget that is selected.
            // The only way for a texture to be modified is if the render area holds a selected texture, in which case so does this widget
            if(texture != nullptr)
            {
                if(texture->textureName().contains("_Modified_"))
                {
                    return;
                }

                texture->setTextureName(texture->textureName() + "_Modified_" + QString::number(textureModifyCount++), {});

                ui->textureNameLineEdit->setText(texture->textureName());

                QString previousTextureLocation = texture->textureLocation();

                QString newTextureLocation = texture->textureLocation();

                newTextureLocation.remove(newTextureLocation.lastIndexOf('/') + 1, newTextureLocation.size() - newTextureLocation.lastIndexOf('/') + 1);

                newTextureLocation += texture->textureName();
                newTextureLocation += '.' + texture->textureFormat();

                texture->setTextureLocation(newTextureLocation, {});

                ui->textureFormatLabel->setText("Texture Format: " + texture->textureFormat());

                // Any texture buttons that represent the previous image (before it was saved) need to be updated so that
                // they refer to the newly saved image.
                emit updateTextureButtonLocation(previousTextureLocation, newTextureLocation);
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


            ui->intersectionBorderWidthLineEdit->setEnabled(true);
            ui->selectionBorderWidthLineEdit->setEnabled(true);
            ui->textureNameLineEdit->setEnabled(true);
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

            // Get the last time the selected texture was modified and display it to the user.
            // Note that the location of the texture used for loading the image is queried.
            QFileInfo fileInfo{texture->textureLocation()};

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

        void SelectedTextureInformation::textureNameChanged()
        {
            // Note that even if return is pressed, the texture is not changed until the texture is saved.
            // This is because due to how textures are loaded when a texture button is pressed (texture location is given to find an image)
            // the user will still not be able to load the old texture with the old name in. Therefore there is no point in doing so now.
            if(ui->textureNameLineEdit->text().contains("_Modified_"))
            {
                QMessageBox::warning(this, tr("Error: Invalid Texture Name"), "Cannot have a texture name with the word Modified.", QMessageBox::Ok);

                return;
            }
            else if(ui->textureNameLineEdit->text() == texture->textureName())
            {
                QMessageBox::warning(this, tr("Error: Invalid Texture Name"), "You must choose unique name, different than an existing texture's!", QMessageBox::Ok);

                return;
            }

            if(texture != nullptr)
            {
                texture->setTextureName(ui->textureNameLineEdit->text(), {});
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

            ui->intersectionBorderWidthLineEdit->setEnabled(false);
            ui->selectionBorderWidthLineEdit->setEnabled(false);
            ui->textureNameLineEdit->setEnabled(false);
            ui->intersectionZoomComboxBox->setEnabled(false);
            ui->selectionZoomComboBox->setEnabled(false);
        }
    }
}