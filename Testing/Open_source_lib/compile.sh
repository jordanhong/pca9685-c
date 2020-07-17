#rm -f ./a.out && gcc -Wall -pedantic $1 && valgrind ./a.out>log
rm -f ./a.out && gcc -Wall -pedantic $1 && valgrind ./a.out>log
