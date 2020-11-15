#pragma once

#include <stdexcept>

namespace mipt {
    struct parse_error : std::logic_error {
        using std::logic_error::logic_error;
    };
}
