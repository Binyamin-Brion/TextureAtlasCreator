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

        ui->loadedTextures->setTextureBankReference(textureBank.get());

        ui->atlasWidget->setTextureBankReference(textureBank.get());

        textureBank->setAtlasTabWidgetReference(ui->atlasWidget);

        textureBank->setCurrentTextureTabWidgetReference(ui->currentTexture);

        connect(ui->currentTexture, SIGNAL(repaintSelectedTexture()), ui->atlasWidget, SLOT(repaintSelectedTexture()));

        connect(ui->currentTexture, SIGNAL(changedRenderArea(const PaintFunctions::Brush&)), ui->widget, SLOT(showDifferentBrush(const PaintFunctions::Brush&)));
        connect(ui->currentTexture, SIGNAL(selectedTextureChanged(QSize, QSize)), ui->widget, (SLOT(updateSelectedTextureSize(QSize, QSize))));

        ui->currentTexture->setCurrentIndex(1);
        ui->currentTexture->setCurrentIndex(0);
        ui->widget->updateSelectedTextureSize(QSize{-1, 1}, QSize{25, 25});

        new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_L), ui->loadedTextures, SLOT(showLoadTextureDialog()));

        setMinimumSize(1280, 720);
    }

    MainWindow::~MainWindow()
    {

    }

}