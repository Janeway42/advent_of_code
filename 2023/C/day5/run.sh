#! /bin/bash

#step 1 - make the basics.a library 
cd ../basics
make 
cd ../day5

#step 2 - compile and run part 1
gcc day5part1.c ../basics/basics.a -fsanitize=address && ./a.out input.txt

#step 3 - compile and run part 2 
#verify for memmory allocation/deallocation with -fsanitize=address 
#verify for data races with -fasintize=thread
gcc day5part2.c ../basics/basics.a -fsanitize=address && ./a.out input.txt
# gcc day5part2.c ../basics/basics.a -fsanitize=thread && ./a.out input.txt

#step 4 - clean up
rm a.out
cd ../basics
make fclean 
cd ../day5 
