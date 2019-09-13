cmake_minimum_required(VERSION 3.14)
project(TextureAtlasCreator)

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -Wextra")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_PREFIX_PATH C:\\Qt\\Qt5.9.8\\5.9.8\\mingw53_32)

find_package(Qt5Widgets REQUIRED)
find_package(Qt5Test REQUIRED)

include_directories(${Qt5Widgets_INCLUDES})
include_directories(C:\\glm\\glm)

add_executable(TextureAtlasCreator main.cpp TextureLogic/ScaledTexture.cpp TextureLogic/ScaledTexture.h TextureLogic/Zoom.h
        TextureLogic/TextureBank.cpp TextureLogic/TextureBank.h TextureLogic/Texture.cpp TextureLogic/Texture.h
        TextureLogic/TextureBorder/Border.h TextureLogic/TextureBorder/SurroundingBorder.cpp TextureLogic/TextureBorder/SurroundingBorder.h
        AccessRestriction/PassKey.h)

target_link_libraries(TextureAtlasCreator ${Qt5Widgets_LIBRARIES} ${Qt5Test_LIBRARIES})