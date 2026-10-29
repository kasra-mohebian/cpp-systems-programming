#include "cppstudy/TempDirectory.hpp"

#include <utility>

namespace raii {

static void destroyDir(TempDirectory::DirState *s) {
    if (!s)
        return;

    if (s->fd >= 0) {
        system_calls::closeFd(s->fd);
        s->fd = -1;
    }

    if (s->isRoot) {
        system_calls::removeDir(s->path.c_str());
    } else {
        system_calls::removeSubdir(s->parentFd, s->name.c_str());
    }

    delete s;
}

TempDirectory::TempDirectory(std::string absPath) {
    auto holder = std::make_unique<DirState>();
    auto *raw = holder.get();
    raw->isRoot = true;
    if (absPath.empty() || absPath.front() != '/' || absPath.find('\0') != std::string::npos)
        throw std::invalid_argument("expected an absolute path without NUL bytes");
    raw->path = std::move(absPath);

    system_calls::createDir(raw->path.c_str());
    try {
        raw->fd = system_calls::openDir(raw->path.c_str());
    } catch (...) {
        system_calls::removeDir(raw->path.c_str());
        throw;
    }

    state = std::shared_ptr<DirState>(holder.release(), destroyDir);
}

TempDirectory::TempDirectory(const TempDirectory &parent, std::string name) {
    auto holder = std::make_unique<DirState>();
    auto *raw = holder.get();
    raw->isRoot = false;
    system_calls::validateName(name);
    raw->name = std::move(name);
    raw->parentFd = parent.fd();
    raw->path = parent.path() + "/" + raw->name;
    raw->parentHold = parent.state;

    system_calls::createSubdir(raw->parentFd, raw->name.c_str());
    try {
        raw->fd = system_calls::openSubdir(raw->parentFd, raw->name.c_str());
    } catch (...) {
        system_calls::removeSubdir(raw->parentFd, raw->name.c_str());
        throw;
    }

    state = std::shared_ptr<DirState>(holder.release(), destroyDir);
}

int TempDirectory::fd() const noexcept { return state ? state->fd : -1; }

const std::string &TempDirectory::path() const noexcept {
    static const std::string empty;
    return state ? state->path : empty;
}

} // namespace raii
