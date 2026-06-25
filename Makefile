CC=gcc
CFLAG=-Iinclude
LIBS=-lm
MAIN=main
SRC_DIR=./src
SRCS=$(wildcard $(SRC_DIR)/*.c)

all: $(MAIN)

$(MAIN): $(SRCS)
	$(CC) $(CFLAG) $(SRCS) -o $(MAIN) $(LIBS)

clean:
	rm -f $(MAIN)
