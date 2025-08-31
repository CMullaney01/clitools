#pragma once
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

#include "argparser.hpp"

namespace clitools {

class Command {
 public:
  using CommandFn = std::function<void(const std::vector<std::string> &args)>;

  Command() = default;
  Command(const std::string& desc, CommandFn fn = nullptr)
      : description(desc), fn(fn) {}

  void set_function(CommandFn f) { fn = f; }
  void set_description(const std::string& desc) { description = desc; }

  void add_command(const std::string &name, Command cmd);
  void run(int argc, char *argv[]);
  void print_help() const {
    std::cout << (description.empty() ? "No description\n"
                                      : description + "\n");
    if (!commands.empty()) {
      std::cout << "\nAvailable subcommands:\n";
      for (auto &[name, cmd] : commands) {
        std::cout << " " << name;
        if (!cmd.description.empty()) {
          std::cout << " - " << cmd.description;
          std::cout << std::endl;
        }
      }
    }
  }

 private:
  std::string description;
  CommandFn fn;
  ArgParser argparser;
  std::unordered_map<std::string, Command> commands;
};
}  // namespace clitools
