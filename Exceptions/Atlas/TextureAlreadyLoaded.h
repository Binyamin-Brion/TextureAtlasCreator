//
// Created by BinyBrion on 2019-09-19.
//

#ifndef TEXTUREATLASCREATOR_TEXTUREALREADYLOADED_H
#define TEXTUREATLASCREATOR_TEXTUREALREADYLOADED_H

#include <exception>
#include <string>

namespace Exceptions
{
    namespace Atlas
    {
        class TextureAlreadyLoaded : public std::exception
        {
            public:
                explicit TextureAlreadyLoaded(std::string message) : _message{message} {}
                explicit TextureAlreadyLoaded(const char *message) : _message{message} {}

                const char* what() const noexcept override
                {
                    return _message.c_str();
                }

            private:
                std::string _message;
        };

    }
}

#endif //TEXTUREATLASCREATOR_TEXTUREALREADYLOADED_H
