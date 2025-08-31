#include "clitools/argparse.hpp"
#include <string>

namespace clitools {

// Parse raw argv into the parsed map
ArgParser::ArgParser(int argc, char* argv[]) {
  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];
    if (arg.rfind(FLAG_PREFIX, 0) == 0) {
      std ::string key = arg.substr(FLAG_PREFIX.size());
      std::string value = "true";

      // --name Alice -> parsed["--name"] = "Alice"
      // --debug      -> parsed["--debug"] = "true"
      if (i + 1 < argc) { 
        std::string next = argv[i + 1];
        if (next.rfind(FLAG_PREFIX, 0) != 0) {
          value = next;
          ++i; 
        }
      }
      parsed[key] = value;
    }
  }
}

void ArgParser::add_option(const std::string& flag, const std::string& desc) {
  options[flag] = desc;
}

bool ArgParser::has(const std::string& flag) const {
  return parsed.find(flag) != parsed.end();
}

std::string ArgParser::get(const std::string& flag, const std::string& def) const {
  auto it = parsed.find(flag);
  return it != parsed.end() ? it->second : def;
}

void ArgParser::help() const {
  std::cout << "Available options:\n";
  for (const auto& [flag, desc] : options) {
    std::cout << " " << flag << "\it" << desc << "\n";
  }
}

} // namespace clitools
