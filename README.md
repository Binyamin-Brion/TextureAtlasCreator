Texture Atlas Creator
===

## Overview

This is a project to create a texture atlas for used in 3D programs. It was made as a personal project
to do three things:

* Have an alternative program to create texture atlases
* Gain experience with C++ and Qt
* Learn how to deal with codebase that is constantly being modified and growing

## Features

* Create atlases of arbitrary sizes and many different image formats
* Ability to move textures around once placed to optimize available space
* Specify borders around images within the atlases to deal with issues
  that pop up when mipmapping texture atlases
* Apply simple edits (and undo them) to a texture within an atlas, 
  along with the texture's associated specular texture
* View information about textures, such as their format, bit depth and size in bytes
* Zoom to make it easier to view large or small atlases
* Save project and work on it later

_Note: While any textures can be used with this program, the built-in texture editor is intended for making small 
changes to textures used in Minecraft. Serious alterations to textures should be done with an external program._

## User Guide

A user guide in both PDF and docx form is provided in the 'User Guide' folder, which is itself located in the root folder.

## Technologies Used

* C++14
* Qt5.9.8
* CMake

## Compilation Note

In addition to needing Qt5.9.8 and a C++14 compliant compiler, the location to the Qt installation will need to be updated
in the CMake file to match where it is installed on the machine being used for compilation.

## External Resource

_Dark theme obtained from: http://www.yasinuludag.com/darkorange.stylesheet_

## Sample Project

A sample view of how it looks can be seen below:

![alt-text](https://github.com/Binyamin-Brion/TextureAtlasCreator/blob/master/Assets/Sampe%20Project%20%20Output/Sample%20Project%20View.png)

The exported atlas from the above sample project, along with the associated specular atlas:

![alt-text](https://github.com/Binyamin-Brion/TextureAtlasCreator/blob/master/Assets/Sampe%20Project%20%20Output/Sample%20Atlas%20Export.png)

![alt-text](https://github.com/Binyamin-Brion/TextureAtlasCreator/blob/master/Assets/Sampe%20Project%20%20Output/Sample%20Atlas%20Export%20Specular.png)
