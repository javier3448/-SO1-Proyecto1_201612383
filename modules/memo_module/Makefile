# object module?
obj-m += memo_201612383.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

# we even clean using that mysterious makefile
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
