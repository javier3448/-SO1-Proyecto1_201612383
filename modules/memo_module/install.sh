# se the current working directory to where the script is
cd "$(dirname "$0")"

# needs sudo
if [ "$EUID" -ne 0 ]
  then echo "Please run as root"
  exit
fi

rmmod memo_201612383
insmod memo_201612383.ko
