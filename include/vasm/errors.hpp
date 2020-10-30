#pragma once

#include <stdexcept>

namespace mipt::vasm {
    struct parse_error : std::logic_error {
        using std::logic_error::logic_error;
    };
}
