#! /bin/bash

#step 1 - make the basics.a library 
cd ../basics
make 
cd ../day4

#step 2 - compile and run part 1
gcc day4part1.c ../basics/basics.a -lm -fsanitize=address && ./a.out input.txt

#step 3 - compile and run part 2
gcc day4part2.c ../basics/basics.a -lm -fsanitize=address && ./a.out input.txt

#step 4 - clean up
rm a.out
cd ../basics
make fclean 
cd ../day4 


# -lm (link math library)