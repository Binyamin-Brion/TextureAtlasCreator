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

        textureBank->setAtlasTabWidgetReference(ui->atlasWidget);

        new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_L), ui->loadedTextures, SLOT(showLoadTextureDialog()));

        setMinimumSize(1600, 900);
    }

    MainWindow::~MainWindow()
    {

    }

}