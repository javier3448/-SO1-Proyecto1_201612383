# needs sudo
if [ "$EUID" -ne 0 ]
  then echo "Please run as root"
  exit
fi

rmmod memo_201612383.ko
insmod memo_201612383.ko
