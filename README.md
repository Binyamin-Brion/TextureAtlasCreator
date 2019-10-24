Texture Atlas Creator
===

## Overview

This is a project to create a texture atlas for used in 3D programs. It was made as a personal project
to do three things:

* Have an alternative program to create texture atlases
* Gain experience with C++ and Qt
* Learn how to deal with codebase that is constantly being modified and growing

A sample view of how it looks can be seen below:

![alt-text](https://github.com/Binyamin-Brion/TextureAtlasCreatorV2/blob/master/TextureCreatorSample.PNG)

## Features

* Create atlases of arbitary sizes and many different image formats
* Ability to move textures around once placed to optimize available space
* Specify borders around images within the atlases to deal with issues
  that pop up when mipmapping texture atlases
* Apply simple edits (and undo them) to a texture within an atlas
* Save edited textures as new textures for later use
* View information about textures, such as their format, bit depth and size in bytes
* Zoom to make it easier to view large or small atlases

_Note: While any textures can be used with this program, the built-in texture editor is intended for making small 
changes to textures used in Minecraft. Serious alterations to textures should be done with an external program._

## Technologies Used

* C++14
* Qt5.9.8
* CMake

_Dark theme obtained from: http://www.yasinuludag.com/darkorange.stylesheet_
