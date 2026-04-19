//
// Created by stefanspitse on 4/19/26.
//

#ifndef LUACHESS_UTILS_H
#define LUACHESS_UTILS_H
#include <array>
#include <cstdint>
#include <string>

class utils {
    public:
        static std::array<uint8_t, 16> generate_id();
        static std::string id_to_hex(std::array<uint8_t, 16> id);
        static std::string generate_hex_id();
};



#endif //LUACHESS_UTILS_H
