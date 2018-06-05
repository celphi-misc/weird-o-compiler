c_token: c_token.l
	flex c_token.l
	gcc -o c_token lex.yy.c

clean:
	rm *.yy.c
	rm c_token