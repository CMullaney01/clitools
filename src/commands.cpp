#include "clitools/commands.hpp"
#include "clitools/constants.hpp"

#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace clitools {

// Root command: commands subcommands
Command::Command(const std::string &name, const std::string &desc)
    : name(name), description(desc), fn(nullptr), commands() {}

// Leaf command: has a function
Command::Command(const std::string &name, const std::string &desc, CommandFn fn)
    : name(name), description(desc), fn(fn), commands() {}

// Leaf command add function with predeclared options
Command::Command(const std::string &name, const std::string &desc, CommandFn fn,
                 std::vector<Command::Option> initial_options)
    : name(name), description(desc), fn(fn), commands() {
  for (const auto &it : initial_options) {
    option_metadata[it.key] = it;
  }
}

void Command::set_function(CommandFn f) { fn = f; }

void Command::set_description(const std::string &desc) { description = desc; }

void Command::add_command(Command cmd) {
  if (cmd.name.empty()) {
    throw std::invalid_argument("Command name cannot be empty.");
  }
  if (commands.count(cmd.name)) {
    std::cerr << "Warning: Command '" << cmd.name
              << "' already exists. Overwriting.\n";
  }
  commands.emplace(cmd.name, std::move(cmd));
}

void Command::parse(int argc, char *argv[]) {
  if (argc < 2) {
    run(argc, argv);
    return;
  }

  std::string arg1 = argv[1];

  // If it matches a subcommand, delegate
  auto it = commands.find(arg1);
  if (it != commands.end()) {
    it->second.parse(argc - 1, argv + 1);
    return;
  }

  // Otherwise, parse options for this command
  options.clear();
  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];

    if (arg.rfind(clitools::FLAG_PREFIX, 0) == 0) {
      std::string key = arg.substr(clitools::FLAG_PREFIX.size());
      std::string value = "true";
   
      if (clitools::HELP_OPTION == key) {
        print_help();
        return;
      }

      if (i + 1 < argc) {
        std::string next = argv[i + 1];
        if (next.rfind(clitools::FLAG_PREFIX, 0) != 0) {
          value = next;
          ++i;
        }
      }
      options[key] = value;
    }
  }

  run(argc, argv);
}

void Command::run(int /*argc*/, char*[] /*argv*/) {
  if (fn) {
    try {
      fn(*this);  // pass self to the handler
    } catch (const std::exception& e) {
      std::cerr << "Error: " << e.what() << "\n";
    }
  } else {
    print_help();
  }
}

void Command::print_help() const {
  std::cout << (description.empty() ? "No description\n"
                                    : description + "\n");

  if (!commands.empty()) {
    std::cout << "\nAvailable subcommands:\n";
    for (auto &kv : commands) {
      std::cout << "  " << kv.first;
      if (!kv.second.description.empty()) {
        std::cout << " - " << kv.second.description;
      }
      std::cout << "\n";
    }
  }

  if (!option_metadata.empty()) {
    std::cout << "\nOptions:\n";
    for (const auto &kv : option_metadata) {
      std::cout << "  --" << kv.first;
      if (!kv.second.description.empty()) {
        std::cout << " : " << kv.second.description;
      }
      if (!kv.second.default_value.empty()) {
        std::cout << " (default: " << kv.second.default_value << ")";
      }
      std::cout << "\n";
    }
  }
}

void Command::add_option(const std::string &key, 
                         const std::string &desc, 
                         const std::string &default_val) {
    option_metadata[key] = Option{
        .key = key,
        .description = desc,
        .default_value = default_val
    };
}
std::string Command::get_option(const std::string &key) const {
  auto it = options.find(key);
  if (it != options.end()) {
    return it->second;
  }
  return {};
}

std::string Command::get_option_or(const std::string &key,
                                   const std::string &default_val) const {
  auto it = options.find(key);
  return it != options.end() ? it->second : default_val;
}

std::string Command::require_option(const std::string &key) const {
  auto it = options.find(key);
  if (it == options.end() || it->second.empty()) {
    print_help();
    throw std::runtime_error("Missing required option: " + clitools::FLAG_PREFIX + key);
  }
  return it->second;
}

} // namespace clitools
