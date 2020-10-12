#pragma once

namespace mipt {
    enum class Status {
        OK = 0,
        INVALID = 1,
        CORRUPTED_STRUCT = 2,
        CORRUPTED_DATA = 3,
        NULL_PTR = 4,
        NULL_DATA = 5,
        CORRUPTED_HASH = 6
    };

    constexpr const char *statusToStr(Status st) {
        switch (st) {
            case Status::OK:
                return "OK";
            case Status::INVALID:
                return "INVALID";
            case Status::CORRUPTED_STRUCT:
                return "CORRUPTED STRUCT";
            case Status::CORRUPTED_DATA:
                return "CORRUPTED DATA";
            case Status::NULL_PTR:
                return "NULL POINTER";
            case Status::NULL_DATA:
                return "DATA NULL POINTER";
            case Status::CORRUPTED_HASH:
                return "CORRUPTED HASH";
            default:
                return "UNKNOWN";
        }
    }
}