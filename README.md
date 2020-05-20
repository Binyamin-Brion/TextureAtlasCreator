Texture Atlas Creator
===

## Overview

A texture atlas is a texture that is itself made of several textures. This provides benefits in terms of rendering efficiency in a 3D enivornment, depending on how the rendering is done. Alternatively, it can be used when using separate textures results in too many texture buffers for a given platform, by allowing to effectively use several textures with one texture buffer.

This project provides a way of creating texture atlases, along with the associated specular texture atlases. It does this while providing the means to help avoid issues that can appear while using texture atlases, chiefly sub-textures bleeding into each other during mipmapping.

## Features

* Create atlases of arbitrary sizes and many different image formats
* Ability to move textures around once placed to optimize available space
* Specify borders around images within the atlases to help deal with issues
  that pop up when mipmapping texture atlases
* Apply simple edits (and undo them) to a texture within an atlas, 
  along with the texture's associated specular texture
* View information about textures, such as their format, bit depth and size in bytes
* Zoom to make it easier to view large or small atlases
* Save project and work on it later

_Note: While any textures can be used with this program, the built-in texture editor is intended for making small 
changes to textures used in Minecraft. Serious alterations to textures should be done with an external program._

## User Guide

A user guide in both PDF and docx form is provided in the 'User Guide' folder, which is itself located in the Docs folder.

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
