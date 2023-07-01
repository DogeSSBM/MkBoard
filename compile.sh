#!/bin/sh
clear
set -ex
output="main.out"
# flags="-std=c11 -lpthread -Wall -Wextra -Wpedantic -Werror -g"
flags="-std=c11 -lpthread -Wall -Wextra -Wpedantic -Werror -Ofast"
gcc main.c $flags -o $output
