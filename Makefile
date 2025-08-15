all : px13-fnlock.c
	gcc -o px13-fnlock px13-fnlock.c

clean:
	rm -f px13-fnlock