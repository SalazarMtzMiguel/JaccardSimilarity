cmake_minimum_required(VERSION 3.29)
project(Jaccard C)

set(CMAKE_C_STANDARD 11)

# Incluir la raíz del proyecto en lugar de carpetas
include_directories(include)

# Incluir directamente los archivos .c en la raíz
file(GLOB SRC_FILES "src/*.c")

add_executable(Jaccard ${SRC_FILES})
