
#pragma once

#include <stdexcept>
#include <string>

namespace bdf
{
    class parse_error : public std::runtime_error
    {
    public:
        using base = std::runtime_error;

        explicit parse_error(const std::string& _Message)
            : base(_Message.c_str())
        {}

        explicit parse_error(const char* _Message)
            : base(_Message)
        {}
    };
}

