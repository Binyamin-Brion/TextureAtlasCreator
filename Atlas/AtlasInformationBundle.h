//
// Created by BinyBrion on 10/20/2019.
//

#ifndef TEXTUREATLASCREATOR_ATLASINFORMATIONBUNDLE_H
#define TEXTUREATLASCREATOR_ATLASINFORMATIONBUNDLE_H

#include <QImage>

namespace Atlas
{
    /**
     *  Combines information regarding some statistics about the current texture atlas into one logical unit.
     *  The information used is stored in TextureAtlas and is used in several QLabels in the MainWindow
     */

    struct AtlasInformationBundle
    {
        /**
         * Initializes the all of the members of the class with the passed in variables.
         * Once initialized, the members will always hold the values passed in as they are const.
         *
         * @param textureFormat the texture format of the atlas this bundle represents
         * @param numberTexturesUsed the number of textures in the atlas this bundle represents
         * @param percentageAtlasUsed the percentage of total area used in the atlas
         */
        AtlasInformationBundle(QImage::Format textureFormat, unsigned int numberTexturesUsed, unsigned int percentageAtlasUsed);

        // What these variables represent can be seen in the above constructor documentation
        const QImage::Format textureFormat;
        const unsigned int numberTexturesUsed;
        const unsigned int percentageAtlasUsed;
    };
}

#endif //TEXTUREATLASCREATOR_ATLASINFORMATIONBUNDLE_H
