#include "clitools/commands.hpp"
#include "clitools/constants.hpp"

#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>

namespace clitools {

Command::Command(const std::string &name, const std::string &desc)
    : name(name), description(desc), fn(nullptr), commands() {}

Command::Command(const std::string &name, const std::string &desc, CommandFn fn)
    : name(name), description(desc), fn(fn), commands() {}

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
    throw std::runtime_error("Missing required option: " + clitools::FLAG_PREFIX + key);
  }
  return it->second;
}

} // namespace clitools
