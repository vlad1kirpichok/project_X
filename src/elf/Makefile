all: clean codeCave

clean:
	@rm -rf *.o codeCave

codeCave: main.o codeCave32.o codeCave64.o common.o
	gcc -no-pie $^ -o $@

main.o: main.c codeCave.h common.h
	gcc -no-pie -c $< -o $@

codeCave32.o: codeCave.c codeCave.h common.h codeCavePayload.h
	gcc -no-pie -DBUILD32 -c $< -o $@

codeCave64.o: codeCave.c codeCave.h common.h codeCavePayload.h
	gcc -no-pie -DBUILD64 -c $< -o $@

common.o: common.c common.h
	gcc -no-pie -c $< -o $@

.PHONY: clean