//
// Created by BinyBrion on 10/3/2019.
//

#include "SelectedTextureInformation.h"
#include "TextureLogic/Texture.h"

namespace GUI
{
    namespace TextureInformation
    {
        SelectedTextureInformation::SelectedTextureInformation(QWidget *parent) : QWidget{parent}, ui{new Ui::SelectedTextureInformation}
        {
            ui->setupUi(this);

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

            const QImage& textureImage = texture->getImage(TextureLogic::Zoom::Normal);

            ui->textureWidthLabel->setText("Texture Width: " + QString::number(textureImage.width()));
            ui->textureHeightLabel->setText("Texture Height: " + QString::number(textureImage.height()));
            ui->textureDepthLabel->setText("Texture Depth: " + QString::number(textureImage.depth()));
            ui->textureSizeBytesLabel->setText("Texture Size Bytes: " + QString::number(textureImage.byteCount()));
            ui->lastSavedLabel->setText("Last Saved: N/A");
            ui->lastSavedToLabel->setText("Last Saved To: NA");

            if(texture->getTextureDescription({}).isEmpty())
            {
                ui->plainTextEdit->setPlainText("No Description");
            }
            else
            {
                ui->plainTextEdit->setPlainText(texture->getTextureDescription({}));
            }
        }

        void SelectedTextureInformation::resetDefaultLabels()
        {
            ui->textureLocationLabel->setText("Texture Location: N/A");
            ui->textureWidthLabel->setText("Texture Width: N/A");
            ui->textureHeightLabel->setText("Texture Height: N/A");
            ui->textureDepthLabel->setText("Texture Depth: N/A");
            ui->textureSizeBytesLabel->setText("Texture Size Bytes: N/A");
            ui->lastSavedLabel->setText("Last Saved: N/A");
            ui->lastSavedToLabel->setText("Last Saved To: NA");
            ui->plainTextEdit->setPlainText("No Texture Selected");
            ui->plainTextEdit->setEnabled(false);
        }
    }
}