#pragma once
#include <nlohmann/json.hpp>
#include <fstream>
#include "components.h"

const float PIXELS_TO_METERS = 1 / 100;

plat::Storage load_lvl(std::string path);
