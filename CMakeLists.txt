cmake_minimum_required(VERSION 3.14)
project(TextureAtlasCreator)

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -Wextra")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# If compiling from scratch, the location to the Qt installation will have to be updated to match where it is installed
# on the machine being used for compilation

if(WIN32)
    set(CMAKE_PREFIX_PATH C:\\Qt\\Qt5.9.8\\5.9.8\\mingw53_32)
endif(WIN32)

if(UNIX)
    set(CMAKE_PREFIX_PATH /home/binybrion/Qt5.9.8/5.9.8/gcc_64)
endif(UNIX)

find_package(Qt5Widgets REQUIRED)
find_package(Qt5Test REQUIRED)

include_directories(${Qt5Widgets_INCLUDES})

add_executable(TextureAtlasCreator main.cpp TextureLogic/ScaledTexture.cpp TextureLogic/ScaledTexture.h TextureLogic/Zoom.h
        TextureLogic/TextureBank.cpp TextureLogic/TextureBank.h TextureLogic/Texture.cpp TextureLogic/Texture.h
        Atlas/TextureBorder/Border.h Atlas/TextureBorder/SurroundingBorder.cpp Atlas/TextureBorder/SurroundingBorder.h
        HelperUtilities/PassKey.h Tests/TestSuite.cpp Tests/TestSuite.h Tests/SurroundingBorder/TestSurroundingBorder.cpp
        Tests/SurroundingBorder/TestSurroundingBorder.h Tests/EnableTests.h Atlas/TextureAtlas.cpp Atlas/TextureAtlas.h
        Atlas/TextureDrawingPosition.h Atlas/SelectedTexture.cpp
        Atlas/SelectedTexture.h GUI/MainWindow.cpp GUI/MainWindow.h GUI/LoadResults/LoadedTextures.cpp GUI/LoadResults/LoadedTextures.h
        GUI/LoadResults/ScrollArea.cpp GUI/LoadResults/ScrollArea.h GUI/LoadResults/TextureButtonArea.cpp GUI/LoadResults/TextureButtonArea.h
        GUI/LoadResults/TextureButton.cpp GUI/LoadResults/TextureButton.h GUI/Dialogs/ChooseTexture.cpp GUI/Dialogs/ChooseTexture.h GUI/Atlas/AtlasTabWidget.cpp
        GUI/Atlas/AtlasTabWidget.h GUI/Atlas/ScrollArea.cpp GUI/Atlas/ScrollArea.h GUI/Atlas/AtlasWidget.cpp GUI/Atlas/AtlasWidget.h
        GUI/CurrentTexture/CurrentTextureTabWidget.cpp GUI/CurrentTexture/CurrentTextureTabWidget.h GUI/CurrentTexture/PaintArea.cpp GUI/CurrentTexture/PaintArea.h
        PaintFunctions/Brush.cpp PaintFunctions/Brush.h PaintFunctions/PaintHistoryCommand.cpp PaintFunctions/PaintHistoryCommand.h
        PaintFunctions/PaintedArea.h GUI/BrushSettings/CurrentBrushSettings.cpp GUI/BrushSettings/CurrentBrushSettings.h GUI/CurrentTexture/ScrollArea.cpp
        GUI/CurrentTexture/ScrollArea.h GUI/TextureInformation/TextureInfoScrollArea.cpp GUI/TextureInformation/TextureInfoScrollArea.h GUI/TextureInformation/SelectedTextureInformation.cpp
        GUI/TextureInformation/SelectedTextureInformation.h GUI/LoadResults/OptionsMenu.cpp
        GUI/LoadResults/OptionsMenu.h GUI/Dialogs/AddNewTab.cpp GUI/Dialogs/AddNewTab.h GUI/Atlas/AtlasTabOptionsMenu.cpp GUI/Atlas/AtlasTabOptionsMenu.h GUI/Dialogs/AddNewAtlasTab.cpp
        GUI/Dialogs/AddNewAtlasTab.h GUI/TextureHelperFunctions/HelperFunctions.cpp GUI/TextureHelperFunctions/HelperFunctions.h TextureLogic/Zoom.cpp Atlas/AtlasInformationBundle.h
        Atlas/AtlasInformationBundle.cpp ProjectLoader/TextureAtlasLoader.h ProjectLoader/TextureButtonAreaLoader.h ProjectLoader/ProjectParser.cpp ProjectLoader/ProjectParser.h
        Tests/ProjectLoader/TestProjectLoader.cpp Tests/ProjectLoader/TestProjectLoader.h GUI/CurrentTexture/SpecularTexture/SpecularTextureTabWidget.cpp GUI/CurrentTexture/SpecularTexture/SpecularTextureTabWidget.h
        GUI/CurrentTexture/SpecularTexture/ScrollArea.cpp GUI/CurrentTexture/SpecularTexture/ScrollArea.h GUI/CurrentTexture/SpecularTexture/DisplayArea.cpp GUI/CurrentTexture/SpecularTexture/DisplayArea.h
        GUI/NameForm.cpp GUI/NameForm.h HelperUtilities/Assert.h GUI/Dialogs/ResizeAtlas.cpp GUI/Dialogs/ResizeAtlas.h)

target_link_libraries(TextureAtlasCreator ${Qt5Widgets_LIBRARIES} ${Qt5Test_LIBRARIES})