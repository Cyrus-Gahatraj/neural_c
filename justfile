cc := "gcc"
cflag := "-Iinclude"
libs := "-lm"
main := "main"
src_dir := "./src"

default: build 

build:
	{{cc}} {{cflag}} {{src_dir}}/*.c -o {{main}} {{libs}}

clean:
	rm -f {{main}} 
