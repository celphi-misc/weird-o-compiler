# Weird-o-compiler

A simple C-like compiler that outputs the abstract syntax tree and intermediate representation in JSON.

`make` : make project, generate executable "woc"

`make clean`: clean all generated files

```
woc [file] [options]
Options:
        -a      Generating AST in JSON
        -i      Generating IR tree in JSON
        -s      Generating symbol table for scopes in JSON
        -c      Generating assemble code, in WOCASM 
        -d      Debug mode, programmer should define usage in parse.y
```

## Ref
* https://github.com/soshimozi/Compiler
* [ANSI C grammar, Lex specification](http://www.lysator.liu.se/c/ANSI-C-grammar-l.html)
* [C-- Language Specification](https://www2.cs.arizona.edu/~debray/Teaching/CSc453/DOCS/cminusminusspec.html)
* [The syntax of C in Backus-Naur Form](https://cs.wmich.edu/~gupta/teaching/cs4850/sumII06/The%20syntax%20of%20C%20in%20Backus-Naur%20form.htm)
* [Flex & Bison](http://web.iitd.ac.in/~sumeet/flex__bison.pdfs)
* [MiniC](http://tinf2.vub.ac.be/~dvermeir/courses/compilers/minic/)
