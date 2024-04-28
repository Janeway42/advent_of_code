#! /bin/bash

#step 1 - make the basics.a library 
cd ../basics
make 
cd ../day3

#step 2 - compile and run part 1
gcc day3part1.c ../basics/basics.a -fsanitize=address && ./a.out input.txt

#step 3 - compile and run part 2
gcc day3part2.c ../basics/basics.a -fsanitize=address && ./a.out input.txt

#step 4 - clean up
rm a.out
cd ../basics
make fclean 
cd ../day3 