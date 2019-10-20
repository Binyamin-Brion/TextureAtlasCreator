//
// Created by BinyBrion on 10/20/2019.
//

#ifndef TEXTUREATLASCREATOR_ATLASINFORMATIONBUNDLE_H
#define TEXTUREATLASCREATOR_ATLASINFORMATIONBUNDLE_H

#include <QImage>

namespace Atlas
{
    struct AtlasInformationBundle
    {
        AtlasInformationBundle(QImage::Format textureFormat, unsigned int numberTexturesUsed, unsigned int percentageAtlasUsed)
                              : textureFormat{textureFormat}, numberTexturesUsed{numberTexturesUsed}, percentageAtlasUsed{percentageAtlasUsed}
        {

        }

        const QImage::Format textureFormat;
        const unsigned int numberTexturesUsed;
        const unsigned int percentageAtlasUsed;
    };
}

#endif //TEXTUREATLASCREATOR_ATLASINFORMATIONBUNDLE_H
