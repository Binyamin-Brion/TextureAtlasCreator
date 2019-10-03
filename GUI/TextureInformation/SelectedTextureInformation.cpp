//
// Created by BinyBrion on 10/3/2019.
//

#include "SelectedTextureInformation.h"

namespace GUI
{
    namespace TextureInformation
    {
        SelectedTextureInformation::SelectedTextureInformation(QWidget *parent) : QWidget{parent}, ui{new Ui::SelectedTextureInformation}
        {
            ui->setupUi(this);
        }
    }
}