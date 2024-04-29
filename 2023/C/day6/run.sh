#! /bin/bash

#step 1 - make the basics.a library 
cd ../basics
make 
cd ../day6

#step 2 - compile and run part 1
gcc day6part1.c ../basics/basics.a -g -fsanitize=address && ./a.out input.txt
# gcc day6part1.c ../basics/basics.a -g && valgrind -s --leak-check=yes ./a.out input.txt

#step 3 - compile and run part 2 
gcc day6part2.c ../basics/basics.a -g -fsanitize=address && ./a.out input.txt

#step 4 - clean up
rm a.out
cd ../basics
make fclean 
cd ../day6 
