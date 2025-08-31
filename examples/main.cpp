
#include "clitools/commands.hpp"
#include <iostream>

using namespace clitools;

int main(int argc, char* argv[]) {
	// Root command
	Command root("Root command for clitools example");

	// 'greet' subcommand
	Command greet_cmd("Greet someone", [](const std::vector<std::string>& args) {
		std::string name = "World";
		if (!args.empty()) name = args[0];
		std::cout << "Hello, " << name << "!\n";
	});

	// 'math' subcommand with nested 'add'
	Command math_cmd("Math operations");
	Command add_cmd("Add two numbers", [](const std::vector<std::string>& args) {
		if (args.size() < 2) {
			std::cout << "Usage: math add <a> <b>\n";
			return;
		}
		int a = std::stoi(args[0]);
		int b = std::stoi(args[1]);
		std::cout << (a + b) << std::endl;
	});
	math_cmd.add_command("add", add_cmd);

	// Add subcommands to root
	root.add_command("greet", greet_cmd);
	root.add_command("math", math_cmd);

	// Run the CLI
	root.run(argc, argv);
	return 0;
}
