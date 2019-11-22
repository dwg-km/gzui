
FILE=$1
SRC_PATH=tmp/
DST_PATH=${HOME}/gzui/

echo ${FILE}

LIB="libpmd.so libexec.so "
EXE="gz update.sh resume.sh poweroff.sh"
SETTING="setting hb_cfg.xml"
CONFIG="hb_descp.xml ph_descp.xml resource"

if [ ! -d $SRC_PATH ];then
	mkdir -p $SRC_PATH
fi

if [ ! -d $DST_PATH ];then
	mkdir -p $DST_PATH
fi

openssl des3 -d -k 123456 -in ${FILE} | tar -C ${SRC_PATH} -zxf  -

SRC_PATH=${SRC_PATH}$(ls ${SRC_PATH})"/"
echo ${SRC_PATH}

#tar_log=$(tar -zxf ${FILE} -C $SRC_PATH)
#if [ $tar_log ];then
#
#fi

for i in $LIB;
do
if [ -f  ${SRC_PATH}$i ];then
	mv ${SRC_PATH}$i /usr/lib/
fi
done

for i in $EXE;
do
if [ -f  ${SRC_PATH}$i ];then
	mv ${SRC_PATH}$i $DST_PATH
fi
done

for i in $CONFIG;
do
if [ -f  ${SRC_PATH}$i ];then
	mv ${SRC_PATH}$i $DST_PATH
fi
if [ -d ${SRC_PATH}$i ];then
	rm -rf ${DST_PATH}${i}
	mv ${SRC_PATH}$i $DST_PATH
fi
done

for i in $SETTING;
do
	if [ ! -f ${DST_PATH}$i ];then
		if [ -d ${SRC_PATH}$i ];then
			rm -rf ${DST_PATH}${i}
		fi
		mv ${SRC_PATH}$i $DST_PATH
	fi
done

#rm -rf ${SRC_PATH}${FILE}
