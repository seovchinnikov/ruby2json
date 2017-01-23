Ruby's lexer and parser, ast builder originally written by Miquel Sabate( https://cgit.kde.org/kdev-ruby.git/)
and extended here for some new functionality (e.g. json visitor and something more).

 
#How to run:  

```
bison -d parser.y -o parser.c  
gcc -c   node.c parser.c  
g++ -c rubyastvisitor.cpp main.cpp  
g++ -o  myapp main.o node.o parser.o rubyastvisitor.o  
./myapp ruby.rb where.out  
```

where ruby.rb is input file that contains correct ruby program's text and where.out is output json file.  
JSON is not formated here, so smth like   http://json.parser.online.fr can be used.  
Lexer is located at  parser_yylex function, that's called inside yylex.

Note. Console is clogged with debug informaion.