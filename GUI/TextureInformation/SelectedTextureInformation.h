//
// Created by BinyBrion on 10/3/2019.
//

#ifndef TEXTUREATLASCREATOR_SELECTEDTEXTUREINFORMATION_H
#define TEXTUREATLASCREATOR_SELECTEDTEXTUREINFORMATION_H

#include <QtWidgets/QWidget>
#include "ui_selectedTextureInformation.h"
#include "GUI/TextureHelperFunctions/TextureFormats.h"

namespace TextureLogic
{
    class Texture;
}

namespace GUI
{
    namespace TextureInformation
    {
        class SelectedTextureInformation : public QWidget
        {
            Q_OBJECT

            public:
                explicit SelectedTextureInformation(QWidget *parent = nullptr);
                QImage::Format getSelectedTextureFormat() const;
                void selectedTextureModified();
                void setTexture(const TextureLogic::Texture *texture);

            signals:
                void reuploadTexture(const QString&, const TextureLogic::Texture*);

            private:
                void resetDefaultLabels();
                void saveImage(const QString &newFileLocation);

                Ui::SelectedTextureInformation *ui = nullptr;
                TextureLogic::Texture *texture = nullptr;

                std::vector<GUI::TextureHelperFunctions::InternalFormatPair> internalFormatPairs;
                std::vector<QString> imageExtensions;

                QImage::Format selectedTextureFormat;
        };
    }
}

#endif //TEXTUREATLASCREATOR_SELECTEDTEXTUREINFORMATION_H
