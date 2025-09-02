#pragma once
#include <string>
#include <unordered_map>

namespace clitools {

class ArgParser {
 public:
    ArgParser(int argc, char *argv[]);

    void add_option(const std::string &flag, const std::string &desc);
    bool has(const std::string &flag) const;
    std::string get(const std::string &flag, const std::string &def = "") const;

    void help() const;

   private:
    std::unordered_map<std::string, std::string> options;
    std::unordered_map<std::string, std::string> parsed;
};

}  // namespace clitools
