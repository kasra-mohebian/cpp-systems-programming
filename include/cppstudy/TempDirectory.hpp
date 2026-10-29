#pragma once
#include "cppstudy/SystemCalls.hpp"
#include <memory>
#include <string>
namespace raii {

class TempDirectory {
  public:
    struct DirState {
        std::string path;
        std::string name;
        int fd = -1;
        int parentFd = -1;
        bool isRoot = false;
        std::shared_ptr<DirState> parentHold;
    };

    TempDirectory() = delete;

    explicit TempDirectory(std::string absPath);
    TempDirectory(const TempDirectory &parent, std::string name);

    TempDirectory(const TempDirectory &) = delete;
    TempDirectory &operator=(const TempDirectory &) = delete;

    TempDirectory(TempDirectory &&) = delete;
    TempDirectory &operator=(TempDirectory &&) = delete;

    ~TempDirectory() = default;

    int fd() const noexcept;
    const std::string &path() const noexcept;

  private:
    std::shared_ptr<DirState> state;

    friend class TempFile;
};

} // namespace raii
