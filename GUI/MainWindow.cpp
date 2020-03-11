//
// Created by BinyBrion on 2019-09-15.
//

#include <QtWidgets/QShortcut>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QFileDialog>
#include <QtCore/QTextStream>
#include "MainWindow.h"
#include "TextureLogic/TextureBank.h"
#include "ProjectLoader/ProjectParser.h"

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

        textureBank->setSelectedTextureTabWidgetReference(ui->currentTexture);

        textureBank->setTextureInfoScrollAreaReference(ui->selectedTextureInformation);

        // Connections between widgets

        connect(ui->currentTexture, SIGNAL(repaintSelectedTexture()), ui->atlasWidget, SLOT(repaintSelectedTexture()));

        connect(ui->currentTexture, SIGNAL(changedRenderArea(const PaintFunctions::Brush&)), ui->brushSettings, SLOT(showDifferentBrush(const PaintFunctions::Brush&)));

        connect(ui->currentTexture, SIGNAL(selectedTextureChanged(QSize, QSize)), ui->brushSettings, (SLOT(updateSelectedTextureSize(QSize, QSize))));

        connect(ui->currentTexture, SIGNAL(zoomChanged(TextureLogic::Zoom)), ui->brushSettings, SLOT(zoomChanged(TextureLogic::Zoom)));

        // Action connections

        connect(ui->actionNew_Project, SIGNAL(triggered()), this, SLOT(newProject()));

        connect(ui->actionOpen_Project, SIGNAL(triggered()), this, SLOT(openProject()));

        connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(saveProject()));

        connect(ui->actionSave_As, SIGNAL(triggered()), this, SLOT(saveAsProject()));

        connect(ui->atlasWidget, SIGNAL(currentAtlasInformationChanged(::Atlas::AtlasInformationBundle)), this, SLOT(showPercentageUsed(::Atlas::AtlasInformationBundle)));

        // For some reason is required in order for the brush colour to be the default colour upon start for the "Texture" Render area (not the specular area)
        ui->currentTexture->setCurrentIndex(1);
        ui->currentTexture->setCurrentIndex(0);

        // By default the brush has a default size and cannot currently paint on anyting as no texture is selected
        ui->brushSettings->updateSelectedTextureSize(QSize{-1, 1}, QSize{25, 25});

        new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_L), ui->loadedTextures, SLOT(showLoadTextureDialog()));
        new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_E), ui->atlasWidget, SLOT(exportTexture()));
        new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_S), this, SLOT(saveProject()));
        new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_O), this, SLOT(openProject()));
        new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_N), this, SLOT(newProject()));

        // Below this size and things look weird
        setMinimumSize(1280, 720);
    }

    MainWindow::~MainWindow()
    {
        // For some reason, this destructor is required in order for the unique_ptr in use with the TextureBank to compile.
    }

    // Beginning of private slots

    void MainWindow::newProject()
    {
        closeProject();

        ui->loadedTextures->addTextureButtonArea("Default");

        ui->atlasWidget->addAtlasWidget("Default", QSize{1920, 1080}, QImage::Format_RGB32);

        // Don't automatically overwrite previously saved location if there is one
        previousSaveLocation.clear();
    }

    void MainWindow::openProject()
    {
        QString openProjectLocation = QFileDialog::getOpenFileName(this, "Open Project", QDir::homePath());

        // User closed the file selection dialog without choosing anything
        if(openProjectLocation.isEmpty())
        {
            return;
        }

        closeProject();

        ::ProjectLoader::ProjectParser projectParser;

        try
        {
            projectParser.parseFile(openProjectLocation);
        }
        catch(std::runtime_error &e)
        {
            QMessageBox::critical(this, "Error Loading Project", e.what(), QMessageBox::Ok);

            if(previousSaveLocation.isEmpty())
            {
                newProject();
            }

            return;
        }

        // The selection and intersection border width are stored in the texture button area part of the project file.
        // When reading the results of the atlas project file, there has to be a way to reference the border widths for the
        // current texture. This variable does that.
        QMap<QString, std::pair<int, int>> loadedTexturesBorderWidths;

        for(const auto &i : projectParser.getTextureButtonAreas())
        {
            ui->loadedTextures->addTextureButtonArea(i.areaName);

            for(const auto &j : i.textures)
            {
                QString textureLocation = createPathToTexture(j.textureLocation);

                ui->loadedTextures->openTexture(i.areaName, textureLocation, j.intersectionWidth, j.selectionWidth);

                loadedTexturesBorderWidths.insert(textureLocation, std::make_pair(j.intersectionWidth, j.selectionWidth));
            }
        }

        for(const auto &i : projectParser.getTextureAtlases())
        {
            ui->atlasWidget->addAtlasWidget(i.atlasName, i.atlasSize, TextureHelperFunctions::convertToFormat(i.format));

            for(const auto &j : i.textures)
            {
                QString textureLocation = createPathToTexture(j.textureLocation);

                // Certain logic is required when adding a texture to an atlas. Therefore go through the process program would take to add a texture to an atlas
                // during runtime when user clicks a texture button.
                textureBank->textureButtonPressedMainWindow(i.atlasName, textureLocation, loadedTexturesBorderWidths[textureLocation].first, loadedTexturesBorderWidths[textureLocation].second, j.position, {});
            }
        }

        showPercentageUsed(ui->atlasWidget->getCurrentAtlasInformation());

        // Don't automatically overwrite previously saved location if there is one
        previousSaveLocation.clear();
    }

    void MainWindow::saveAsProject()
    {
        previousSaveLocation = QFileDialog::getSaveFileName(this, "Save Project As", QDir::homePath(), projectExtension) + projectExtension;

        // User closed the file selection dialog without choosing anything
        if(previousSaveLocation.isEmpty())
        {
            return;
        }

        QFile saveFile{previousSaveLocation};

        if(saveFile.exists())
        {
            saveFile.remove();
        }

        try
        {
            ui->loadedTextures->saveLoadedTextures(previousSaveLocation);

            ui->atlasWidget->saveAtlases(previousSaveLocation);
        }
        catch(std::runtime_error &e)
        {
            QMessageBox::critical(this, "Error Saving Project", e.what(), QMessageBox::Ok);
        }
    }

    void MainWindow::saveProject()
    {
        if(previousSaveLocation.isEmpty())
        {
            saveAsProject();

            return;
        }

        QFile saveFile{previousSaveLocation};

        if(saveFile.exists())
        {
            saveFile.remove();
        }

        try
        {
            ui->loadedTextures->saveLoadedTextures(previousSaveLocation);

            ui->atlasWidget->saveAtlases(previousSaveLocation);
        }
        catch(std::runtime_error &e)
        {
            QMessageBox::critical(this, "Error Saving Project", e.what(), QMessageBox::Ok);
        }
    }

    // Beginning of private functions

    void MainWindow::closeProject()
    {
        if(ui->atlasWidget->getUnsavedAtlases() || ui->loadedTextures->getUnsavedTextureButtonAreas())
        {
            int response = QMessageBox::warning(this, "Unsaved Changes", "Would you like to save changes made?", QMessageBox::Yes | QMessageBox::No);

            if(response == QMessageBox::Yes)
            {
                QString saveDestination = QFileDialog::getSaveFileName(this, "Save Project",  QDir::homePath(),  "*." + projectExtension);

                saveProject();
            }
        }

        textureBank->textureSelected(nullptr);

        ui->atlasWidget->closeAllTabs();

        ui->loadedTextures->closeAllTabs();

        // Don't automatically overwrite previously saved location if there is one
        previousSaveLocation.clear();
    }

    QString MainWindow::createPathToTexture(QString loadedPath)
    {
        // For testing purposes, it is assumed that test project files can be loaded into the program.
        // To ensure that assets are available, the texture locations have to be specified relative to this program folder.

        #ifdef QT_DEBUG

                if(loadedPath.startsWith('/'))
                {
                    QDir executableDirectory = QDir::current();

                    if(!executableDirectory.cdUp())
                    {
                        Q_ASSERT_X(false, __PRETTY_FUNCTION__, "\n\nFailure to get to test asset folder.\n\n");
                    }

                    if(!executableDirectory.cd("Assets/Test"))
                    {
                        Q_ASSERT_X(false, __PRETTY_FUNCTION__, "\n\nFailure to get to test asset folder.\n\n");
                    }

                    loadedPath = executableDirectory.path() + loadedPath;
                }

        #endif

        return loadedPath;
    }

    void MainWindow::showPercentageUsed(::Atlas::AtlasInformationBundle atlasInformationBundle)
    {
        ui->atlasFormatLabel->setText("Atlas Format: " + TextureHelperFunctions::convertToString(atlasInformationBundle.textureFormat));
        ui->numberTexturesLabel->setText("Number of Textures in Atlas: " + QString::number(atlasInformationBundle.numberTexturesUsed));
        ui->percentageAtlasUsed->setText("Percentage Atlas Used: " + QString::number(atlasInformationBundle.percentageAtlasUsed));
    }
}
