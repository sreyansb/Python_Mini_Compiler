lex lex_file.l
yacc -d parser_file_with_value.y
gcc lex.yy.c y.tab.c -ll
cat Code_Optimization/tests/invariance.py | ./a.out