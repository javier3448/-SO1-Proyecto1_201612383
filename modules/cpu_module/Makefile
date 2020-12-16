# object module?
obj-m += cpu_201612383.o

# we use a super complicated make file that is in a fuck off file in or computer
# that I would imagine comes with the kernel to be able to compile a kernel 
# module

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

# we even clean using that mysterious makefile
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean