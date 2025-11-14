cmd_scripts/genksyms/genksyms := clang  -fuse-ld=lld --rtlib=compiler-rt -o scripts/genksyms/genksyms scripts/genksyms/genksyms.o scripts/genksyms/parse.tab.o scripts/genksyms/lex.lex.o   
