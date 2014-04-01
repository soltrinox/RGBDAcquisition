#!/bin/bash
valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes ./Renderer $@ 2>error.txt

exit $?
