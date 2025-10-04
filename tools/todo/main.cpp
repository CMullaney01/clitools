#include "clitools/commands.hpp"
#include <string>

using namespace clitools;

// TODO app;
// Given a task this app will store the todo in a json file
// [
//  {
//    "task": "name",               // string
//    "desc": "short description",  // string
//    "priority": 1,                // int64
//    "deadline": 179875737624354,  // timestamp unixnano
//  }
// ]

const std::string default_path = "~/TODO";

int main(int argc, char* argv[]) {
  Command root("todo", "TODO command line entry");
  
  
  Command settings_cmd("settings", "access settings e.g. path", 
    [](Command& cmd) { 
      // access todo settings contains subcommands to view and update them
    });

  Command list_cmd("list", "List current TODO",
    [](Command& cmd) {
       // read the path and visualise the current tasks
    }); 
  
  Command add_cmd("add", "add todo to the app",
    [](Command& cmd) {
       // read the path and visualise the current tasks
    });

  Command remove_cmd("remove", "remove a todo",
    [](Command& cmd) {
      // remove a todo from the store
    });

  Command modify_cmd("modify", "modify a current todo", 
    [](Command& cmd) { 
      // read the path and visualise the current tasks
    });
}
