#include "cppstudy/TempFile.hpp"

#include <utility>

namespace raii {

TempFile::TempFile(const TempDirectory &dir, std::string name) {
    system_calls::validateName(name);
    st.dir = dir.state;
    st.name = std::move(name);
    st.path = dir.path() + "/" + st.name;

    st.fd = system_calls::openFile(dir.fd(), st.name.c_str());
    st.active = true;
}

TempFile::~TempFile() { cleanup(); }

TempFile::TempFile(TempFile &&other) noexcept {
    st = std::move(other.st);
    other.st.active = false;
    other.st.fd = -1;
}

TempFile &TempFile::operator=(TempFile &&other) noexcept {
    if (this == &other)
        return *this;
    cleanup();
    st = std::move(other.st);
    other.st.active = false;
    other.st.fd = -1;
    return *this;
}

void TempFile::cleanup() noexcept {
    if (!st.active)
        return;
    st.active = false;

    if (st.dir) {
        system_calls::removeFile(st.dir->fd, st.name.c_str());
    }

    if (st.fd >= 0) {
        system_calls::closeFd(st.fd);
        st.fd = -1;
    }
}

const std::string &TempFile::path() const noexcept { return st.path; }

} // namespace raii
