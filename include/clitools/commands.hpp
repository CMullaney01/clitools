#pragma once
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

namespace clitools {

class Command {
 public:
  struct Option {
    std::string key;
    std::string description;
    std::string default_value;
  };
  using CommandFn = std::function<void(Command &)>;

  // Root command: contains subcommands
  Command(const std::string &name, const std::string &desc);

  // Leaf command: has a function
  Command(const std::string &name, const std::string &desc, CommandFn fn);

  // Leaf command add a function and predelcared options
  Command(const std::string &name, const std::string &desc, CommandFn fn,
          std::vector<Command::Option> initial_options);

  void set_function(CommandFn f);
  void set_description(const std::string &desc);
  void add_command(Command cmd);
  // Parse argv into /option handling
  void parse(int argc, char *argv[]);

  // Run the command handler
  void run();

  // Help
  void print_help() const;

  // Option accessors -- keep options as strings for now. TODO:
  // get_option_bool,get_option_string,get_option_vec.
  void add_option(const std::string &key, const std::string &desc,
                  const std::string &default_val = "");
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
  std::unordered_map<std::string, Option> option_metadata;
};

}  // namespace clitools
