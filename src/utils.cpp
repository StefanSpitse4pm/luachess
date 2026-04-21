/*
MIT License

Copyright (c) 2026 Stefan Spitse

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

//
// Created by stefanspitse on 4/19/26.
//

#include "utils.h"

#include <iomanip>
#include <random>
#include <sstream>

std::array<uint8_t, 16> utils::generate_id()
{
    thread_local std::mt19937_64 rng(std::random_device{}());
    std::array<uint8_t, 16> id{};

    for (auto &b : id) {
        b = static_cast<uint8_t>(rng() & 0xFF);
    }
    return id;
}

std::string utils::id_to_hex(std::array<uint8_t, 16> id)
{
    std::ostringstream oss;
    for (auto b : id) {
        oss << std::hex << std::uppercase << std::setw(2) << std::setfill('0') << (int)b;
    }
    return oss.str();
}

std::string utils::generate_hex_id()
{
    return id_to_hex(generate_id());
}
