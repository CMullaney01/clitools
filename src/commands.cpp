
#include "clitools/commands.hpp"
#include <iostream>

namespace clitools {

void Command::add_command(const std::string& name, Command cmd) {
  commands[name] = std::move(cmd);
}

void Command::run(int argc, char* argv[]) {
  if (argc < 2) {
    print_help();
    return;
  }
  std::string arg = argv[1];
  if (arg.rfind(FLAG_PREFIX, 0) == 0) {
    // Option, not a subcommand
    argparser = ArgParser(argc, argv);
    if (fn) {
      std::vector<std::string> args(argv + 1, argv + argc);
      fn(args);
    } else {
      print_help();
    }
    return;
  }
  auto it = commands.find(arg);
  if (it != commands.end()) {
    it->second.run(argc - 1, argv + 1);
  } else {
    std::cout << "Unknown command: " << arg << std::endl;
    print_help();
  }
}

} // namespace clitools
