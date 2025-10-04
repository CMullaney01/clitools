#include "clitools/commands.hpp"
#include <iostream>

using namespace clitools;


// ./fctl greet
// # Hello, World!

// ./fctl greet --name Alice
// # Hello, Alice!

// ./fctl math add --a 5 --b 7
// # 12

int main(int argc, char* argv[]) {
  // Root command
  Command root("fctl", "Root command for clitools example");

  // 'greet' subcommand
  Command greet_cmd("greet", "Greet someone",
    [](Command& cmd) {
      // Get option --name, default to "World"
      std::string name = cmd.get_option_or("name", "World");
      std::cout << "Hello, " << name << "!\n";
    });

	greet_cmd.add_option("name", "Name to greet", "World");

  // 'math' subcommand with nested 'add'
  Command math_cmd("math", "Enables simple math operations");
  Command add_cmd("add", "Add two numbers",
    [](Command& cmd) {
      auto a_str = cmd.require_option("a");
      auto b_str = cmd.require_option("b");

      int a = std::stoi(a_str);
      int b = std::stoi(b_str);
      std::cout << (a + b) << std::endl;
    });

  add_cmd.add_option("a", "first number to add");
  // if we don't add b, it will not show up in help
	
  math_cmd.add_command(add_cmd);

  // create a command with initial options
  const std::vector<Command::Option> multiply_options = {
    {.key = "x", .description = "first number to multiply", .default_value = "1"},
    {.key = "y", .description = "second number to multiply", .default_value = "1"}
	};

	Command multiply_cmd("multiply", "Multiply two numbers",
		[](Command& cmd) {
			auto x_str = cmd.get_option_or("x", "1");
			auto y_str = cmd.get_option_or("y", "1");
			int x = std::stoi(x_str);
			int y = std::stoi(y_str);
			std::cout << (x * y) << std::endl;
		},
		multiply_options
	);

  math_cmd.add_command(multiply_cmd);

  // Add subcommands to root
  root.add_command(greet_cmd);
  root.add_command(math_cmd);

  // Run the CLI
  root.parse(argc, argv);

  return 0;
}

