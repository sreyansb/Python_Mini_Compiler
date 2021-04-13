lex lex_file.l
yacc -d parser_file.y
gcc lex.yy.c y.tab.c -ll
cat test4.py | ./a.out