//
// Created by BinyBrion on 10/3/2019.
//

#ifndef TEXTUREATLASCREATOR_SELECTEDTEXTUREINFORMATION_H
#define TEXTUREATLASCREATOR_SELECTEDTEXTUREINFORMATION_H

#include <QtWidgets/QWidget>
#include "ui_selectedTextureInformation.h"

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
                void selectedTextureModified();
                void setTexture(const TextureLogic::Texture *texture);

            signals:
                void reuploadTexture(const QString&, const TextureLogic::Texture*);

            private:
                void resetDefaultLabels();
                void saveImage(const QString &newFileLocation);

                Ui::SelectedTextureInformation *ui = nullptr;
                TextureLogic::Texture *texture = nullptr;

                struct InternalFormatPair
                {
                    QImage::Format format;
                    QString formatStringRepresentation;
                };

                std::vector<InternalFormatPair> internalFormatPairs;
                std::vector<QString> imageExtensions;
        };
    }
}

#endif //TEXTUREATLASCREATOR_SELECTEDTEXTUREINFORMATION_H
