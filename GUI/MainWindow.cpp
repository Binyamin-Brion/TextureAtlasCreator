//
// Created by BinyBrion on 2019-09-15.
//

#include <QtWidgets/QShortcut>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QFileDialog>
#include <QtCore/QTextStream>
#include <QApplication>
#include "MainWindow.h"
#include "TextureLogic/TextureBank.h"
#include "ProjectLoader/ProjectParser.h"
#include "NameForm.h"
#include "ui_mainwindow.h"

namespace GUI
{
    MainWindow::MainWindow(QWidget *parent)
                :
                    QMainWindow{parent},
                    ui{new Ui::MainWindow},
                    nameFormUi{new NameForm{this}},
                    textureBank{std::make_unique<TextureLogic::TextureBank>(false)}
    {
        ui->setupUi(this);

        setWindowTitle("Atlas Creator");

        ui->selectedTextureInformation->setCentralWidget();

        // Remember that all of the widgets that deal with textures share the same resource (ie the texture bank).

        ui->loadedTextures->setTextureBankReference(textureBank.get());

        ui->atlasWidget->setTextureBankReference(textureBank.get());

        ui->currentTexture->setTextureBankReference(textureBank.get());

        ui->selectedTextureInformation->setTextureBankReference(textureBank.get());

        ui->specularTextureDisplay->setTextureBankReference(textureBank.get());

        // Setup texture bank with the required widget references

        textureBank->setAtlasTabWidgetReference(ui->atlasWidget);

        textureBank->setSelectedTextureTabWidgetReference(ui->currentTexture);

        textureBank->setTextureInfoScrollAreaReference(ui->selectedTextureInformation);

        textureBank->setSpecularTabWidgetReference(ui->specularTextureDisplay);

        ui->brushSettings->setBrushReference(*ui->currentTexture->getBrush());

        textureBank->setLoadedTextures(ui->loadedTextures);

        // By default the brush has a default size and cannot currently paint on anything as no texture is selected
        ui->brushSettings->updateSelectedTextureSize(QSize{-1, 1}, QSize{25, 25});

        // Below maximum horizontal size and things look weird and weird layout issues occur between the loaded texture
        // and the texture information widget. Best to have vertical size scale with horizontal stretch so that everything
        // doesn't look weird.
        showMaximized();

        setMinimumSize(width(), height());

        // Initialize all of the connections for the program

        initializeAtlasActionsConnections();

        initializeFileActionConnections();

        initializeKeyboardShortcutsConnections();

        initializeTextureButtonAreaActionsConnections();

        initializeWidgetConnections();
    }

    void MainWindow::closeEvent(QCloseEvent *event)
    {
        if(ui->atlasWidget->getUnsavedAtlases() || ui->loadedTextures->getUnsavedTextureButtonAreas() || ui->currentTexture->getUnsavedChanges())
        {
            int response = QMessageBox::information(this, "Unsaved Changes", "Would you like to save your changes before exiting the application?", QMessageBox::Yes | QMessageBox::No);

            if(response == QMessageBox::Yes)
            {
                saveProject();
            }
        }

        QMainWindow::closeEvent(event);
    }

    void MainWindow::setTheme()
    {
        QDir dir = QDir::current();

        #ifdef QT_DEBUG
            dir.cdUp();
            dir.cd("Assets");
        #endif

        QFile f(dir.path() + "/darkTheme.txt");

        if (!f.exists())
        {
            #ifdef QT_DEBUG

                QMessageBox::warning(this, "Unable To Theme", "The theme for this program located in file ProjectRoot/Assets/darkTheme.txt was not found.\n"
                                                              "The program will use the default theme.", QMessageBox::Ok);

            #else

                QMessageBox::warning(this, "Unable To Theme", "The theme for this program located in folder as the executable was not found.\n"
                                                                  "The program will use the default theme.", QMessageBox::Ok);

            #endif
        }
        else
        {
            f.open(QFile::ReadOnly | QFile::Text);
            QTextStream textStream(&f);
            qApp->setStyleSheet(textStream.readAll());
        }
    }

    MainWindow::~MainWindow()
    {
        // For some reason, this destructor is required in order for the unique_ptr in use with the TextureBank to compile.
    }

    // Beginning of private slots

    void MainWindow::initializeAtlasActionsConnections()
    {
        connect(ui->actionNew_Atlas, SIGNAL(triggered()), ui->atlasWidget, SLOT(showAddNewAtlasTab()));

        connect(ui->actionMove_Current_Atlas_Left, SIGNAL(triggered()), ui->atlasWidget, SLOT(moveTabLeft()));

        connect(ui->actionMove_Current_Atlas_Right, SIGNAL(triggered()), ui->atlasWidget, SLOT(moveTabRight()));

        connect(ui->actionRename_Atlas, SIGNAL(triggered()), ui->atlasWidget, SLOT(showRenameTabDialog()));

        connect(ui->actionDelete_Current_Atlas, SIGNAL(triggered()), ui->atlasWidget, SLOT(deleteCurrentTab()));
    }

    void MainWindow::initializeFileActionConnections()
    {
        connect(ui->actionExport_Current_Atlas, SIGNAL(triggered()), ui->atlasWidget, SLOT(exportTexture()));

        connect(ui->actionNew_Project, SIGNAL(triggered()), this, SLOT(newProject()));

        connect(ui->actionOpen_Project, SIGNAL(triggered()), this, SLOT(openProject()));

        connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(saveProject()));

        connect(ui->actionSave_As, SIGNAL(triggered()), this, SLOT(saveAsProject()));

    }

    void MainWindow::initializeTextureButtonAreaActionsConnections()
    {
        connect(ui->actionNew_Texutre_Button_Area, SIGNAL(triggered()), ui->loadedTextures, SLOT(showAddTabDialog()));

        connect(ui->actionLoad_Texture, SIGNAL(triggered()), ui->loadedTextures, SLOT(showLoadTextureDialog()));

        connect(ui->actionMove_Current_Button_Area_Left, SIGNAL(triggered()), ui->loadedTextures, SLOT(moveTabLeft()));

        connect(ui->actionMove_Current_Button_Area_Right, SIGNAL(triggered()), ui->loadedTextures, SLOT(moveTabRight()));

        connect(ui->actionRename_Button_Area, SIGNAL(triggered()), ui->loadedTextures, SLOT(showRenameTabDialog()));

        connect(ui->actionDelete_Current_Button_Area, &QAction::triggered, [this]() { ui->loadedTextures->deleteCurrentTab(false); });
    }

    void MainWindow::initializeKeyboardShortcutsConnections()
    {
        new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_L), ui->loadedTextures, SLOT(showLoadTextureDialog()));

        new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_E), ui->atlasWidget, SLOT(exportTexture()));

        new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_S), this, SLOT(saveProject()));

        new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_O), this, SLOT(openProject()));

        new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_N), this, SLOT(newProject()));
    }

    void MainWindow::initializeWidgetConnections()
    {
        connect(ui->currentTexture, SIGNAL(repaintSelectedTexture()), ui->atlasWidget, SLOT(repaintSelectedTexture()));

        connect(ui->currentTexture, SIGNAL(repaintSelectedTexture()), ui->specularTextureDisplay, SLOT(repaintSpecularTexture()));

        connect(ui->currentTexture, SIGNAL(repaintSelectedTexture()), ui->loadedTextures, SLOT(paintTextureButtons()));

        connect(ui->currentTexture, SIGNAL(selectedTextureChanged(QSize, QSize)), ui->brushSettings, SLOT(updateSelectedTextureSize(QSize, QSize)));

        connect(ui->specularTextureDisplay, SIGNAL(zoomChanged(::TextureLogic::Zoom)), ui->brushSettings, SLOT(zoomChangedDisplayArea(::TextureLogic::Zoom)));

        connect(ui->currentTexture, SIGNAL(zoomChanged(::TextureLogic::Zoom)), ui->brushSettings, SLOT(zoomChangedPaintArea(::TextureLogic::Zoom)));

        connect(ui->atlasWidget, SIGNAL(currentAtlasInformationChanged(::Atlas::AtlasInformationBundle)), this, SLOT(showPercentageUsed(::Atlas::AtlasInformationBundle)));

        connect(ui->atlasWidget, SIGNAL(atlasDimensionChanged(::Atlas::AtlasInformationBundle)), this, SLOT(showPercentageUsed(::Atlas::AtlasInformationBundle)));
    }

    void MainWindow::newProject()
    {
        closeProject();

        ui->loadedTextures->addTextureButtonArea("Default");

        ui->atlasWidget->addAtlasWidget("Default", QSize{1920, 1080}, QImage::Format_RGB32);

        // Don't automatically overwrite previously saved location if there is one
        previousSaveLocation.clear();
        projectName.clear();
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

        // The location of the textures that are saved are stored as a relative path to the folder they exist in.
        // So instead of say /home/project/image.png, the location is stored as /image.png.
        // Thus if the project folder is moved, the '/home/project' part is changed to the folder the user selected.
        // This allows the project folder to be moved around but still be able to locate the textures within that folder.
        QString openFolderLocation = openProjectLocation;
        openFolderLocation.chop(openFolderLocation.size() - openFolderLocation.lastIndexOf('/'));

        // The selection and intersection border width are stored in the texture button area part of the project file.
        // When reading the results of the atlas project file, there has to be a way to reference the border widths for the
        // current texture. This variable does that.
        QMap<QString, std::pair<int, int>> loadedTexturesBorderWidths;

        for(const auto &i : projectParser.getTextureButtonAreas())
        {
            ui->loadedTextures->addTextureButtonArea(i.areaName);

            for(const auto &j : i.textures)
            {
                ui->loadedTextures->openTexture(i.areaName, openFolderLocation + j.textureLocation, j.intersectionWidth, j.selectionWidth, true);

                loadedTexturesBorderWidths.insert(j.textureLocation, std::make_pair(j.intersectionWidth, j.selectionWidth));
            }
        }

        for(const auto &i : projectParser.getTextureAtlases())
        {
            static int index = 0;

            try
            {
                ui->atlasWidget->addAtlasWidget(i.atlasName, i.atlasSize, TextureHelperFunctions::convertToFormat(i.format));
            }
            catch(std::runtime_error &e)
            {
                QMessageBox::critical(this, "Fatal Internal Error", e.what(), QMessageBox::Ok);
            }

            ui->atlasWidget->setCurrentIndex(index++);

            for(const auto &j : i.textures)
            {
                // When loading the textures, the relative paths have to be replaced by an absolute path for loading them
                QString absoluteTexturePath = openFolderLocation + j.textureLocation;

                // Certain logic is required when adding a texture to an atlas. Therefore go through the process program would take to add a texture to an atlas
                // during runtime when user clicks a texture button.
                textureBank->textureButtonPressedMainWindow(i.atlasName, absoluteTexturePath, loadedTexturesBorderWidths[absoluteTexturePath].first, loadedTexturesBorderWidths[absoluteTexturePath].second, j.position, {});
            }
        }

        // Prevent issue that occurs that a tab has not been selected yet as they were just loaded, meaning index is not a valid
        // number. The index 0 is always valid as there is always an atlas tab in the program
        ui->atlasWidget->setCurrentIndex(0);

        showPercentageUsed(ui->atlasWidget->getCurrentAtlasInformation());

        // Don't automatically overwrite previously saved location if there is one
        previousSaveLocation.clear();

        projectName.clear();
    }

    void MainWindow::saveAsProject()
    {
        nameFormUi->exec();

        projectName = nameFormUi->getChosenName();

        if(projectName.isEmpty())
        {
            QMessageBox::critical(this, "Invalid Project Name", "A name must be entered for the project.", QMessageBox::Ok);

            return;
        }

        previousSaveLocation = QFileDialog::getExistingDirectory(this, "Save Project As", QDir::homePath(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

        // User closed the file selection dialog without choosing anything
        if(previousSaveLocation.isEmpty())
        {
            return;
        }

        previousSaveLocation += '/' + projectName;

        QDir directoryMaker;

        if(directoryMaker.exists(previousSaveLocation))
        {
            int response = QMessageBox::information(this, "Save Location Already Exists", "Overwrite it?",
                                                    QMessageBox::Yes | QMessageBox::Cancel);

            if(response != QMessageBox::Yes)
            {
                return;
            }

            directoryMaker.remove(previousSaveLocation);
        }

        // If this fails, the original save location is lost if it existed. However, used already indicated they don't care
        // if it gets removed in the above warning.
        if(!directoryMaker.mkdir(previousSaveLocation))
        {
            QMessageBox::critical(this, "Unable To Create Save Directory", "An unknown error occurred.", QMessageBox::Ok);

            return;
        }

        previousSaveLocation += '/' + projectName + projectExtension;

        try
        {
            ui->loadedTextures->saveLoadedTextures(previousSaveLocation);

            ui->atlasWidget->saveAtlases(previousSaveLocation);

            ui->currentTexture->changesSaved();
        }
        catch(std::runtime_error &e)
        {
            QMessageBox::critical(this, "Error Saving Project", e.what(), QMessageBox::Ok);
        }
    }

    void MainWindow::saveProject()
    {
        if(previousSaveLocation.isEmpty() || projectName.isEmpty())
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

            ui->currentTexture->changesSaved();
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

    void MainWindow::showPercentageUsed(::Atlas::AtlasInformationBundle atlasInformationBundle)
    {
        try
        {
            ui->atlasFormatLabel->setText("Atlas Format: " + TextureHelperFunctions::convertToString(atlasInformationBundle.textureFormat));
        }
        catch(std::runtime_error &e)
        {
            QMessageBox::critical(this, "Fatal Internal Error", e.what() + QString{"\nThe format of the current atlas will not be shown."}, QMessageBox::Ok);

            ui->atlasFormatLabel->setText("Unknown");
        }

        ui->numberTexturesLabel->setText("Number of Textures in Atlas: " + QString::number(atlasInformationBundle.numberTexturesUsed));
        ui->atlasDimensionLabel->setText("Atlas Dimensions (100% Zoom): " + QString::number(atlasInformationBundle.atlasDimensions.width()) + " x " + QString::number(atlasInformationBundle.atlasDimensions.height()));
        ui->percentageAtlasUsed->setText("Percentage Atlas Used: " + QString::number(atlasInformationBundle.percentageAtlasUsed));
    }
}
