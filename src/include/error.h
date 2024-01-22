#pragma once

namespace error {
    enum code {
        NONE,
        UNKNOWN,

        DATABASE_DOES_NOT_EXIST,
        CANNOT_CREATE_DATABASE,

        CANNOT_OPEN_DATABASE,
        CANNOT_PREPARE_QUERY,
        SQL,

        UNKNOWN_COMMAND,
        MISSING_PARAMETER,

        INVALID_ID
    };

    void raise(const code c, const char* buff = "");
    void raise(const char* msg);
}
