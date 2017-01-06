SRC_FILES=domotocell_inf.c \
          domotocell.c \
          recvdispatch.c \
          main.c 

app: $(SRC_FILES)
	gcc -o motocell.o $(SRC_FILES) -lpthread -lrt

all: app

clean:
	rm -rf *.o	
