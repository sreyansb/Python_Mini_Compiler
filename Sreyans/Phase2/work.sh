lex lex_file.l
yacc -d parser_file2.y
gcc -Wall lex.yy.c y.tab.c -ll
cat ../test.py | ./a.out