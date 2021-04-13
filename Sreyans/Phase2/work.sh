lex copy_lexer.l
yacc -d copy_parser_file2.y
gcc lex.yy.c y.tab.c -ll
cat ../test4.py | ./a.out