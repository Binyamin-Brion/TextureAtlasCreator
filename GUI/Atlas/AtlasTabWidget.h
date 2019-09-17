//
// Created by BinyBrion on 2019-09-16.
//

#ifndef TEXTUREATLASCREATOR_ATLASTABWIDGET_H
#define TEXTUREATLASCREATOR_ATLASTABWIDGET_H

#include "AccessRestriction/PassKey.h"

namespace TextureLogic
{
    class TextureBank;
}

namespace GUI
{
    namespace Atlas
    {
        class AtlasTabWidget
        {
            public:
                void updateTextureReferences(AccessRestriction::PassKey<TextureLogic::TextureBank>);

            private:
        };
    }
}

#endif //TEXTUREATLASCREATOR_ATLASTABWIDGET_H
