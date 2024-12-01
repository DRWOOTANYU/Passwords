cmake_minimum_required(VERSION 3.10)
project(PasswordsGenerator)

add_executable(
    ${PROJECT_NAME}
        src/main.cpp
        src/init.cpp
        src/generation.cpp
)