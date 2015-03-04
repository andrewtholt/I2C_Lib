libi2c.so:	libi2c.c
	gcc -c -g -fPIC -shared libi2c.c -o libi2c.so

install:	libi2c.so
	cp libi2c.so /usr/local/lib
	/sbin/ldconfig

clean: 
	rm -f libi2c.so 

