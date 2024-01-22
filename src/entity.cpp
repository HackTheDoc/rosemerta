#include "include/entity.h"

Entity::Status to_status(const int s) {
    switch (s) {
    case 0:
        return Entity::Status::ALIVE;
    case 1:
        return Entity::Status::DEAD;
    default:
        return Entity::Status::UNKNOWN;
    }
}
