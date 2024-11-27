#!/bin/zsh

make

bin/lexer < test_names_02.txt > output.txt
bin/lexer < test_parsing_error01.txt > output2.txt
# echo "HEre"
# gdb bin/lexer
bin/lexer < test_comments_01.txt> output3.txt
make clean 2>/dev/null
