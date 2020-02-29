//
// Created by BinyBrion on 2019-09-15.
//

#include <QtWidgets/QShortcut>
#include "MainWindow.h"
#include "TextureLogic/TextureBank.h"

namespace GUI
{
    MainWindow::MainWindow(QWidget *parent)
                :
                    QMainWindow{parent},
                    ui{new Ui::MainWindow}, textureBank{std::make_unique<TextureLogic::TextureBank>()}
    {
        ui->setupUi(this);

        setWindowTitle("Atlas Creator");

        ui->selectedTextureInformation->setCentralWidget();

        // Remember that all of the widgets that deal with textures share the same resource (ie the texture bank).

        ui->loadedTextures->setTextureBankReference(textureBank.get());

        ui->atlasWidget->setTextureBankReference(textureBank.get());

        ui->currentTexture->setTextureBankReference(textureBank.get());

        ui->selectedTextureInformation->setTextureBankReference(textureBank.get());

        // Setup texture bank with the required widget references

        textureBank->setAtlasTabWidgetReference(ui->atlasWidget);

        textureBank->setCurrentTextureTabWidgetReference(ui->currentTexture);

        textureBank->setTextureInfoScrollAreaReference(ui->selectedTextureInformation);

        // Connections between widgets

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

        // For some reason is required in order for the brush colour to be the default colour upon start for the "Texture" Render area (not the specular area)
        ui->currentTexture->setCurrentIndex(1);
        ui->currentTexture->setCurrentIndex(0);

        // By default the brush has a default size and cannot currently paint on anyting as no texture is selected
        ui->brushSettings->updateSelectedTextureSize(QSize{-1, 1}, QSize{25, 25});

        new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_L), ui->loadedTextures, SLOT(showLoadTextureDialog()));
        new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_E), ui->atlasWidget, SLOT(exportTexture()));

        // Below this size and things look weird
        setMinimumSize(1280, 720);
    }

    MainWindow::~MainWindow()
    {
        // For some reason, this destructor is required in order for the unique_ptr in use with the TextureBank to compile.
    }
}