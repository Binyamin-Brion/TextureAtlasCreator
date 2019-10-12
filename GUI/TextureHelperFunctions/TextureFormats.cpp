//
// Created by BinyBrion on 10/11/2019.
//

#include "TextureFormats.h"

namespace GUI
{
    namespace TextureHelperFunctions
    {
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
    }
}