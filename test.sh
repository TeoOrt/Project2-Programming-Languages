#!/bin/zsh

make
bin/lexer < test_names_02.txt > output.txt

make clean 2>/dev/null
