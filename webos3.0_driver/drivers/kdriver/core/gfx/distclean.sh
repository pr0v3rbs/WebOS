case $1 in 
l9)   rm -f l9/gfx_drv_l9.c l9/gfx_hw_l9.c l9/*.h ;;
h13)  echo "not ready" ;;
*)    echo "invlaid chip $1" ;;
esac
