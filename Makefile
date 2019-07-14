.phony: clean

all:
	cd Client && make
	cd SP && make

clean:
	cd SP && make clean
	cd Client && make clean