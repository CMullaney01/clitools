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

    // 'math' subcommand with nested 'add'
    Command math_cmd("math", "Enables simple math operations");
    Command add_cmd("add", "Add two numbers",
    [](Command& cmd) {
        int a = std::stoi(cmd.require_option("a"));
        int b = std::stoi(cmd.require_option("b"));
        std::cout << (a + b) << std::endl;
    });

    math_cmd.add_command(add_cmd);

    // Add subcommands to root
    root.add_command(greet_cmd);
    root.add_command(math_cmd);

    // Run the CLI
    root.parse(argc, argv);

    return 0;
}

