//
// Created by BinyBrion on 2019-09-12.
//

#ifndef TEXTUREATLAS_PASSKEY_H
#define TEXTUREATLAS_PASSKEY_H

namespace AccessRestriction
{
    template<typename T>
    class PassKey
    {
        friend T;

        PassKey() {}
        PassKey(const PassKey&) {}
        PassKey& operator=(const PassKey&) = delete;
    };
}

#endif //TEXTUREATLAS_PASSKEY_H
