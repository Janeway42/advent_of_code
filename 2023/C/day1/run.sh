#! /bin/bash

#step 1 - make the basics.a library 
cd ../basics
make 
cd ../day1

#step 2 - compile and run part 1
gcc day1part1.c ../basics/basics.a -fsanitize=address && ./a.out input.txt

#step 3 - compile and run part 2
gcc day1part2.c ../basics/basics.a -fsanitize=address && ./a.out input.txt

#step 4 - clean up
rm a.out
cd ../basics
make fclean 
cd ../day1 
