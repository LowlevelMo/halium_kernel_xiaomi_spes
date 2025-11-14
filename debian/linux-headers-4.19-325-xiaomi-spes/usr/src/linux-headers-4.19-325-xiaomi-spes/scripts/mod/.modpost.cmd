cmd_scripts/mod/modpost := clang  -fuse-ld=lld --rtlib=compiler-rt -o scripts/mod/modpost scripts/mod/modpost.o scripts/mod/file2alias.o scripts/mod/sumversion.o   
