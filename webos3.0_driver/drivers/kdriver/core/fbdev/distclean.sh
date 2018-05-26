case $1 in 
l9)   rm -f l9/fbdev_drv_l9.c l9/fbdev_util_l9.c  l9/fbdev_hw_l9*.c l9/*.h ;;
h13)  echo "not ready" ;;
*)    echo "invlaid chip $1" ;;
esac
