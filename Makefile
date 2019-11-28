all:
	make -C driver
	make -C string-format
	make -C test

clean:
	make -C driver clean
	make -C string-format clean
	make -C test clean