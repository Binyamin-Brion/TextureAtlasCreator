//
// Created by BinyBrion on 10/11/2019.
//

#ifndef TEXTUREATLASCREATOR_TEXTUREFORMATS_H
#define TEXTUREATLASCREATOR_TEXTUREFORMATS_H

#include <QString>
#include <QtGui/qimage.h>

namespace GUI
{
    namespace TextureHelperFunctions
    {
        struct InternalFormatPair
        {
            QImage::Format format;
            QString formatStringRepresentation;
        };

        std::vector<InternalFormatPair> internalFormatPairRepresentations();
        std::vector<QString> listImageExtensions();
    }
}


#endif //TEXTUREATLASCREATOR_TEXTUREFORMATS_H
