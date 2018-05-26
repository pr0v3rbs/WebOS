case $1 in 
l9)   rm -f l9/demod_common_l9.c l9/demod_drv_l9.c  l9/demod_dvb_l9.c  l9/demod_vqi_l9.c  l9/*.h ;;
h13)  echo "not ready" ;;
h14)   rm -f h14/demod_common_h14.c h14/demod_drv_h14.c  h14/demod_dvb_h14.c   h14/*.h ;;
*)    echo "invlaid chip $1" ;;
esac
