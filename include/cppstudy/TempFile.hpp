#pragma once
#include "cppstudy/TempDirectory.hpp"

#include <string>
namespace raii {

class TempFile {
  public:
    TempFile() = delete;

    TempFile(const TempDirectory &dir, std::string name);

    TempFile(const TempFile &) = delete;
    TempFile &operator=(const TempFile &) = delete;

    TempFile(TempFile &&other) noexcept;
    TempFile &operator=(TempFile &&other) noexcept;

    ~TempFile();

    const std::string &path() const noexcept;

  private:
    struct FileState {
        std::shared_ptr<TempDirectory::DirState> dir;
        std::string name;
        std::string path;
        int fd = -1;
        bool active = true;
    };

    FileState st;

    void cleanup() noexcept;
};

} // namespace raii
