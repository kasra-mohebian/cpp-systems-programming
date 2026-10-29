#pragma once
#include <cerrno>
#include <fcntl.h>
#include <stdexcept>
#include <string_view>
#include <sys/stat.h>
#include <system_error>
#include <unistd.h>
namespace system_calls {
inline void check(int result, const char *what) {
    if (result < 0)
        throw std::system_error(errno, std::generic_category(), what);
}
inline void validateName(std::string_view name) {
    if (name.empty() || name == "." || name == ".." || name.find('/') != name.npos ||
        name.find('\0') != name.npos)
        throw std::invalid_argument("expected a single directory entry name");
}
inline void createDir(const char *path) { check(::mkdir(path, 0700), "mkdir"); }
inline int openDir(const char *path) {
    int fd = ::open(path, O_RDONLY | O_DIRECTORY | O_CLOEXEC | O_NOFOLLOW);
    check(fd, "open directory");
    return fd;
}
inline void createSubdir(int parent, const char *name) {
    validateName(name);
    check(::mkdirat(parent, name, 0700), "mkdirat");
}
inline int openSubdir(int parent, const char *name) {
    int fd = ::openat(parent, name, O_RDONLY | O_DIRECTORY | O_CLOEXEC | O_NOFOLLOW);
    check(fd, "openat directory");
    return fd;
}
inline int openFile(int parent, const char *name) {
    validateName(name);
    int fd = ::openat(parent, name, O_RDWR | O_CREAT | O_EXCL | O_CLOEXEC | O_NOFOLLOW, 0600);
    check(fd, "create file");
    return fd;
}
// Cleanup deliberately never throws. The owner must finish using the descriptors first.
inline void closeFd(int fd) noexcept { (void)::close(fd); }
inline void removeDir(const char *path) noexcept { (void)::rmdir(path); }
inline void removeSubdir(int parent, const char *name) noexcept {
    (void)::unlinkat(parent, name, AT_REMOVEDIR);
}
inline void removeFile(int parent, const char *name) noexcept { (void)::unlinkat(parent, name, 0); }
} // namespace system_calls
