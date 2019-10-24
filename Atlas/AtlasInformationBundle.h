//
// Created by BinyBrion on 10/20/2019.
//

#ifndef TEXTUREATLASCREATOR_ATLASINFORMATIONBUNDLE_H
#define TEXTUREATLASCREATOR_ATLASINFORMATIONBUNDLE_H

#include <QImage>

namespace Atlas
{
    /*
     *  Combines information regarding some statistics about the current texture atlas into one logical unit.
     *  The information used is stored in TextureAtlas and is used in several QLabels the MainWindow
     */

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
