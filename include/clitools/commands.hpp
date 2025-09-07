#pragma once
#include <functional>
#include <stdexcept>
#include <string>
#include <unordered_map>

namespace clitools {

class Command {
 public:
  using CommandFn = std::function<void(Command &)>;

  // Root command: contains subcommands
  Command(const std::string &name, const std::string &desc);

  // Leaf command: has a function and options
  Command(const std::string &name, const std::string &desc, CommandFn fn);

  void set_function(CommandFn f);
  void set_description(const std::string &desc);
  void add_command(Command cmd);

  // Parse argv into subcommand/option handling
  void parse(int argc, char *argv[]);

  // Run the command handler
  void run(int argc, char *argv[]);

  // Help
  void print_help() const;

  // Option accessors -- keep options as strings for now. TODO:
  // get_option_bool,get_option_string,get_option_vec.
  std::string get_option(const std::string &key) const;
  std::string get_option_or(const std::string &key,
                            const std::string &default_val) const;

  // Require option (throws if missing)
  std::string require_option(const std::string &key) const;

 private:
  std::string name;
  std::string description;
  CommandFn fn;
  std::unordered_map<std::string, Command> commands;
  std::unordered_map<std::string, std::string> options;
};

}  // namespace clitools
