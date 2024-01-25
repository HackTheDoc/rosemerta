#pragma once

namespace error {
    enum code {
        NONE,
        UNKNOWN,

        CANNOT_OPEN_FOLDER,

        DATABASE_DOES_NOT_EXIST,
        CANNOT_CREATE_DATABASE,

        CANNOT_OPEN_DATABASE,
        CANNOT_PREPARE_QUERY,
        SQL,

        UNKNOWN_COMMAND,
        UNKNOWN_PARAMETER,
        MISSING_PARAMETER,

        EXISTING_ADDRESS,
        INVALID_ID
    };

    void raise(const code c, const char* buff = "");
    void raise(const char* msg);
}
