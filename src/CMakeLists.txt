add_library(clitools
    commands.cpp
)

target_include_directories(clitools
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/../include
)
