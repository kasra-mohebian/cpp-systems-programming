#include "cppstudy/CommandLine.hpp"

#include <cstddef>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

namespace raii {

void CommandLine::run(std::string basedir) {
    std::vector<std::unique_ptr<TempDirectory>> dirStack;
    dirStack.push_back(std::make_unique<TempDirectory>(std::move(basedir)));

    std::vector<TempFile> openFiles;

    std::size_t dirCounter = 0;
    std::size_t fileCounter = 0;

    std::string line;
    while (true) {
        std::cout << "> " << std::flush;
        if (!std::getline(std::cin, line)) {
            break;
        }

        std::istringstream iss(line);
        std::string cmd;
        iss >> cmd;
        if (cmd.empty()) {
            continue;
        }

        TempDirectory &cwd = *dirStack.back();

        if (cmd == "current") {
            std::cout << "current directory: " << cwd.path() << "\n";
        } else if (cmd == "enter") {
            const std::string name = "dir" + std::to_string(dirCounter++);
            auto nextDir = std::make_unique<TempDirectory>(cwd, name);
            std::cout << "entering directory " << nextDir->path() << "\n";
            dirStack.push_back(std::move(nextDir));
        } else if (cmd == "leave") {
            std::cout << "leaving directory " << cwd.path() << "\n";
            if (dirStack.size() == 1) {
                break;
            }
            dirStack.pop_back();
        } else if (cmd == "create") {
            const std::string name = "file" + std::to_string(fileCounter++);
            TempFile f(cwd, name);
            std::cout << "created file " << f.path() << "\n";
            openFiles.push_back(std::move(f));
        } else if (cmd == "list") {
            for (std::size_t i = 0; i < openFiles.size(); ++i) {
                std::cout << "[" << i << "] " << openFiles[i].path() << "\n";
            }
        } else if (cmd == "remove") {
            std::size_t n = 0;
            if (!(iss >> n)) {
                continue;
            }
            if (n >= openFiles.size()) {
                continue;
            }
            std::cout << "removed file " << openFiles[n].path() << "\n";
            openFiles.erase(openFiles.begin() + static_cast<std::ptrdiff_t>(n));
        } else if (cmd == "quit") {
            std::cout << "quitting\n";
            openFiles.clear();
            dirStack.clear();
            break;
        }
    }

    openFiles.clear();
    dirStack.clear();
}

} // namespace raii
