#include "include/error.h"
#include "include/hue.h"
#include "include/database.h"

namespace error {
    void raise(const code c, const char* buff) {
        hue::print("[Error] ", hue::color::RED);

        switch (c) {
        case code::NONE:
            hue::println("no error...");
            break;
        case code::CANNOT_OPEN_FOLDER:
            hue::println("failed to open associated folder");
            break;
        case code::DATABASE_DOES_NOT_EXIST:
            hue::println("database does not exist");
            break;
        case code::CANNOT_CREATE_DATABASE:
            hue::println("unable to create the database");
            break;
        case code::CANNOT_OPEN_DATABASE:
            hue::println("unable to open the database");
            break;
        case code::CANNOT_PREPARE_QUERY:
            hue::print("query error: ");
            hue::println(Database::err_msg());
            break;
        case code::SQL:
            hue::print("sql error: ");
            hue::println(Database::err_msg());
            break;
        case code::UNKNOWN_COMMAND:
            hue::print("unknown command: ");
            hue::println(buff);
            break;
        case code::UNKNOWN_PARAMETER:
            hue::print("unknown ");
            hue::print(buff);
            hue::println(" parameter");
            break;
        case code::MISSING_PARAMETER:
            hue::print("missing ");
            hue::print(buff);
            hue::println(" parameter(s)");
            break;
        case code::INVALID_ID:
            hue::println("invalid id");
            break;
        case code::UNKNOWN:
        default:
            hue::println("unknown behavior");
            break;
        };
    };

    void raise(const char* msg) {
        hue::print("[Error] ", hue::color::RED);
        hue::println(msg);
    }
}
