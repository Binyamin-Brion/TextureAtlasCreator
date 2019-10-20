//
// Created by BinyBrion on 2019-09-15.
//

#include <QtWidgets/QShortcut>
#include "MainWindow.h"
#include "TextureLogic/TextureBank.h"

namespace GUI
{
    MainWindow::MainWindow(QWidget *parent) : QMainWindow{parent}, ui{new Ui::MainWindow}, textureBank{std::make_unique<TextureLogic::TextureBank>()}
    {
        ui->setupUi(this);

        ui->selectedTextureInformation->setCentralWidget();

        ui->loadedTextures->setTextureBankReference(textureBank.get());

        ui->atlasWidget->setTextureBankReference(textureBank.get());

        ui->currentTexture->setTextureBankReference(textureBank.get());

        ui->selectedTextureInformation->setTextureBankReference(textureBank.get());

        textureBank->setAtlasTabWidgetReference(ui->atlasWidget);

        textureBank->setCurrentTextureTabWidgetReference(ui->currentTexture);

        textureBank->setTextureInfoScrollAreaReference(ui->selectedTextureInformation);

        connect(ui->currentTexture, SIGNAL(repaintSelectedTexture()), ui->atlasWidget, SLOT(repaintSelectedTexture()));

        connect(ui->currentTexture, SIGNAL(changedRenderArea(const PaintFunctions::Brush&)), ui->brushSettings, SLOT(showDifferentBrush(const PaintFunctions::Brush&)));
        connect(ui->currentTexture, SIGNAL(selectedTextureChanged(QSize, QSize)), ui->brushSettings, (SLOT(updateSelectedTextureSize(QSize, QSize))));
        connect(ui->currentTexture, SIGNAL(zoomChanged(TextureLogic::Zoom)), ui->brushSettings, SLOT(zoomChanged(TextureLogic::Zoom)));

        connect(ui->atlasWidget, &Atlas::AtlasTabWidget::currentAtlasInformationChanged, [this](::Atlas::AtlasInformationBundle atlasInformation)
        {
            ui->atlasFormatLabel->setText("Atlas Format: " + TextureHelperFunctions::convertToString(atlasInformation.textureFormat));
            ui->numberTexturesLabel->setText("Number of Textures in Atlas: " + QString::number(atlasInformation.numberTexturesUsed));
            ui->percentageAtlasUsed->setText("Percentage Atlas Used: " + QString::number(atlasInformation.percentageAtlasUsed));
        });

        ui->currentTexture->setCurrentIndex(1);
        ui->currentTexture->setCurrentIndex(0);
        ui->brushSettings->updateSelectedTextureSize(QSize{-1, 1}, QSize{25, 25});

        new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_L), ui->loadedTextures, SLOT(showLoadTextureDialog()));

        setMinimumSize(1280, 720);
    }

    MainWindow::~MainWindow()
    {

    }

}