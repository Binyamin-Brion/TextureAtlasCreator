//
// Created by BinyBrion on 10/11/2019.
//

#include <QtWidgets/QMessageBox>
#include "HelperFunctions.h"
#include <QApplication>

namespace GUI
{
    namespace TextureHelperFunctions
    {
        QImage::Format convertToFormat(QString imageFormat)
        {
            auto internalFormats = internalFormatPairRepresentations();

            auto requestedFormatLocation = std::find_if(internalFormats.begin(), internalFormats.end(), [imageFormat]
                    (const InternalFormatPair &internalFormatPair)
            {
                return internalFormatPair.formatStringRepresentation == imageFormat;
            });

            if(requestedFormatLocation == internalFormats.end())
            {
                // If this fail then the GUI won't display some information. User can continue.
                throw std::runtime_error("Failed to find the requested image format: " + imageFormat.toStdString() + ". Location of error: \n" + __PRETTY_FUNCTION__);
            }

            return requestedFormatLocation->format;
        }

        QString convertToString(QImage::Format imageFormat)
        {
            auto internalFormats = internalFormatPairRepresentations();

            auto requestedFormatLocation = std::find_if(internalFormats.begin(), internalFormats.end(), [imageFormat]
                    (const InternalFormatPair &internalFormatPair)
            {
                return internalFormatPair.format == imageFormat;
            });

            if(requestedFormatLocation == internalFormats.end())
            {
                // If this fail then the GUI won't display some information. User can continue (though the project file cannot be saved- user will have to modify manually).
                throw std::runtime_error{"Failed to find the requested image format: " + QString::number(imageFormat).toStdString() + " (this is the the enum value). Location of error: \n" + __PRETTY_FUNCTION__ };
            }

            return requestedFormatLocation->formatStringRepresentation;
        }

        QString getImageFormat(QString textureLocation)
        {
            textureLocation = textureLocation.replace('\\', '/');

            // File type (and therefore texture type here) is everything after the period in a file location.
            return  textureLocation.right(textureLocation.size() - textureLocation.lastIndexOf('.') - 1);
        }

        QString getImageName(QString textureLocation)
        {
            // To ensure everything works as expected on Windows and Linux, ensure only forward slashes in file system locations.
            textureLocation = textureLocation.replace('\\', '/');

            // The texture name is any text after the last slash, minus text after the period denoting the file type.
            QString textureName = textureLocation.right(textureLocation.size() - textureLocation.lastIndexOf('/') - 1);

            // File type (and therefore texture type here) is everything after the period in a file location.
            QString textureFormat = textureName.right(textureLocation.size() - textureLocation.lastIndexOf('.') - 1);

            // File type not part of texture name.
            textureName.chop(textureFormat.size() + 1);

            return textureName;
        }

        std::vector<InternalFormatPair> internalFormatPairRepresentations()
        {
            // There is a need to use both the actual enum for the format of the string (as when saving) and its string
            // representation (for use with the GUI). To ease this conversion, all of the supported Image formats are pushed
            // to this vector along with its string representation. Whenever there is need to convert from one to the other,
            // search the vector using the information available. For example, if the string "Mono" is known, search the
            // vector for the word "Mono", which will yield the respective enum.

            std::vector<InternalFormatPair> internalFormatPairs;

            internalFormatPairs.push_back(InternalFormatPair{QImage::Format_Invalid, "Invalid"});
            internalFormatPairs.push_back(InternalFormatPair{QImage::Format_Mono, "Mono"});
            internalFormatPairs.push_back(InternalFormatPair{QImage::Format_MonoLSB, "MonoLSB"});
            internalFormatPairs.push_back(InternalFormatPair{QImage::Format_Indexed8, "Indexed8"});
            internalFormatPairs.push_back(InternalFormatPair{QImage::Format_RGB32, "RGB32"});
            internalFormatPairs.push_back(InternalFormatPair{QImage::Format_ARGB32, "ARGB32"});
            internalFormatPairs.push_back(InternalFormatPair{QImage::Format_ARGB32_Premultiplied, "ARGB32_Premultiplied"});
            internalFormatPairs.push_back(InternalFormatPair{QImage::Format_RGB16, "RGB16"});
            internalFormatPairs.push_back(InternalFormatPair{QImage::Format_ARGB8565_Premultiplied, "ARGB8565_Premultiplied"});
            internalFormatPairs.push_back(InternalFormatPair{QImage::Format_RGB666, "RGB666"});
            internalFormatPairs.push_back(InternalFormatPair{QImage::Format_ARGB6666_Premultiplied, "ARGB666_Premultiplied"});
            internalFormatPairs.push_back(InternalFormatPair{QImage::Format_RGB555, "RGB555"});
            internalFormatPairs.push_back(InternalFormatPair{QImage::Format_ARGB8555_Premultiplied, "ARGB8555_Premultiplied"});
            internalFormatPairs.push_back(InternalFormatPair{QImage::Format_RGB888, "RGB888"});
            internalFormatPairs.push_back(InternalFormatPair{QImage::Format_RGB444, "RGB444"});
            internalFormatPairs.push_back(InternalFormatPair{QImage::Format_ARGB4444_Premultiplied, "ARGB444_Premultiplied"});
            internalFormatPairs.push_back(InternalFormatPair{QImage::Format_RGBX8888, "RGBX888"});
            internalFormatPairs.push_back(InternalFormatPair{QImage::Format_RGBA8888, "RGBA8888"});
            internalFormatPairs.push_back(InternalFormatPair{QImage::Format_RGBA8888_Premultiplied, "RGBA8888_Premultiplied"});
            internalFormatPairs.push_back(InternalFormatPair{QImage::Format_BGR30, "BGR30"});
            internalFormatPairs.push_back(InternalFormatPair{QImage::Format_RGB30, "RGB30"});
            internalFormatPairs.push_back(InternalFormatPair{QImage::Format_A2RGB30_Premultiplied, "A2RGB30_Premultiplied"});
            internalFormatPairs.push_back(InternalFormatPair{QImage::Format_A2BGR30_Premultiplied, "A2BGR30_Premultiplied"});
            internalFormatPairs.push_back(InternalFormatPair{QImage::Format_Alpha8, "Alpha8"});
            internalFormatPairs.push_back(InternalFormatPair{QImage::Format_Grayscale8, "Grayscale8"});

            return internalFormatPairs;
        }

        std::vector<QString> listImageExtensions()
        {
            // This is for convenience- whenever a new image extension needs to be added, just add it to this
            // vector which will ensure it is shown in the GUI
            std::vector<QString> imageExtensions;

            imageExtensions.push_back("png");
            imageExtensions.push_back("jpg");
            imageExtensions.push_back("bmp");
            imageExtensions.push_back("gif");
            imageExtensions.push_back("svg");

            return imageExtensions;
        }

        unsigned int indexFormat(QImage::Format format, bool ignoreInvalidFormat)
        {
            auto internalFormats = internalFormatPairRepresentations();

            auto requestedFormatLocation = std::find_if(internalFormats.begin(), internalFormats.end(), [format]
                    (const InternalFormatPair &internalFormatPair)
            {
                return internalFormatPair.format == format;
            });

            if(requestedFormatLocation == internalFormats.end())
            {
                // Considering where this function is used (to index into arrays), this is fatal- if this fails
                // then the program may not function correctly.

                QString errorMessage = "Failed to find the requested image format: " + QString::number(format) + "(this is the enum value). Location of error: \n" + __PRETTY_FUNCTION__;
                errorMessage += "\nThis error is not recoverable. The program will terminate.";

                QMessageBox::critical(nullptr, "Fatal Internal Error", errorMessage, QMessageBox::Ok);

                qApp->quit();
            }

            return std::distance(internalFormats.begin(), requestedFormatLocation) - ignoreInvalidFormat;
        }
    }
}