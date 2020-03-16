//
// Created by BinyBrion on 2019-09-12.
//

#ifndef TEXTUREATLAS_PASSKEY_H
#define TEXTUREATLAS_PASSKEY_H

namespace AccessRestriction
{
    /**
     *  Idiom to allow only certain classes to call a function, even if it is public.
     *
     * @tparam T the class to allow calling this function
     */

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
