all:
	make -i -f MakefileBuildMain.mk

test:
	make -i -f MakefileCppUTest.mk

clean:
	make -i -f MakefileCppUTest.mk clean
	
