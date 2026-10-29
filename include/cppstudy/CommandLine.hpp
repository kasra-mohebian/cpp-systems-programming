#pragma once
#include "cppstudy/TempDirectory.hpp"
#include "cppstudy/TempFile.hpp"
#include <string>
namespace raii {
class CommandLine {
  public:
    void run(std::string basedir);
};
} // namespace raii
