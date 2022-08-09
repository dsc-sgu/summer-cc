#pragma once
#include <nlohmann/json.hpp>
#include <fstream>
#include "components.h"
#include "animation.h"

plat::Storage load_lvl(std::string path);
