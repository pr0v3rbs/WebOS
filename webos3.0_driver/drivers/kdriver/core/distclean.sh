
usage()
{
	echo ""
	echo "SOC dist cleaner..."
	echo "remove inactive SOC source/data file(s)..."
	echo "[usage] sh $0 [L9|H13|...]"
	echo ""
}

[ $# -ne 1 ] && usage && exit 1

DSTLIST=$(/bin/ls */distclean.sh)

# call each distclean.sh for all directories
for x in $DSTLIST
do
	echo "<!> $x $1.."
	cd `dirname $x` && sh distclean.sh $1 && cd ..
done
