SRC_FILES=domotocell_inf.c \
          domotocell.c \
          recvdispatch.c \
          main.c 

app: $(SRC_FILES)
	gcc $(SRC_FILES) -lpthread -lrt -o motocell.o

all: app

clean:
	rm -rf *.o	
