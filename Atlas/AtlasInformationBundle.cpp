//
// Created by BinyBrion on 2020-02-15.
//

#include "AtlasInformationBundle.h"

namespace Atlas
{
    AtlasInformationBundle::AtlasInformationBundle(QImage::Format textureFormat, unsigned int numberTexturesUsed, unsigned int percentageAtlasUsed)
                            :
                                textureFormat{textureFormat},
                                numberTexturesUsed{numberTexturesUsed},
                                percentageAtlasUsed{percentageAtlasUsed}
    {

    }
}