c_token: c_token.lex
	flex c_token.lex
	gcc -o c_token lex.yy.c

clean:
	rm *.yy.c
	rm c_token