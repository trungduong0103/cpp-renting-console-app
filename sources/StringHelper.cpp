//
// Created by ryanz on 5/20/2021.
//
#include "../headers/StringHelper.h"

void to_lowercase(std::string string) {
    for (char & pointer : string) {
        pointer = tolower(pointer);
    }
}

bool check_field_contains(std::string target, std::string query) {
    to_lowercase(target);
    to_lowercase(query);

    return target.find(query) != std::string::npos;
}
