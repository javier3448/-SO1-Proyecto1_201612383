# needs sudo
if [ "$EUID" -ne 0 ]
  then echo "Please run as root"
  exit
fi

rmmod cpu_201612383.ko
insmod cpu_201612383.ko

