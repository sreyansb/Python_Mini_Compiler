lex lex_file.l
yacc -d parser_file2.y
gcc lex.yy.c y.tab.c -ll
cat Phase2test/evalCSEfor.py | ./a.out