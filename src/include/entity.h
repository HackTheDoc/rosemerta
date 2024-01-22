#pragma once

#include <string>
#include <vector>

struct Entity {
    enum Status {
        ALIVE,
        DEAD,
        UNKNOWN
    };

    std::string formatted_name{ "" };
    std::string firstname{ "" };
    std::string lastname{ "" };
    std::string username{ "" };
    int age{ -1 };
    std::string birthday{ "" };
    Status status{ Status::UNKNOWN };
};

Entity::Status to_status(const int s);
