utils := ./src/utils/

run: 
	gcc -c ./toml/tomlc17.c -o ./build/tomlc17.o -I . .. ../include/
	gcc -c ./macro.c -o ./build/macro.o -I . .. ../include/
	gcc ./build/macro.o -o ./build/macro 
	./build/macro.exe