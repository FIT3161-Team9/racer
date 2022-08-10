#pragma once

#include <cstdint>
#include <filesystem>

namespace utils
{

uint8_t u8(auto n) { return static_cast<uint8_t>(n); }
uint32_t u32(auto n) { return static_cast<uint32_t>(n); }

const std::string INTER_BLACK = std::filesystem::path("./assets/Inter-Black.ttf").string();
const std::string INTER_SEMI_BOLD = std::filesystem::path("./assets/Inter-SemiBold.ttf").string();

};// namespace utils
