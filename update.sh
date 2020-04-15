
FILE=$1
BIN=tmp/
SRC_PATH=${BIN}
DST_PATH=${HOME}/gzui/

LIB="libpmd.so"
EXE="gz update.sh resume.sh "
SETTING="setting "
CONFIG=" hb_descp.xml ph_descp.xml hb_cfg.xml error_code.xml cali_config.xml pass.bin resources  fonts "

if [ ! -d $SRC_PATH ];then
	mkdir -p $SRC_PATH
fi

if [ ! -d $DST_PATH ];then
	mkdir -p $DST_PATH
fi

#openssl des3 -d -k 123456 -in ${FILE} | tar -C ${SRC_PATH} -zxf  -
tar -zxf ${FILE} -C ${SRC_PATH}

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

#for i in $SETTING;
#do
#	if [ ! -f ${DST_PATH}$i ];then
#		if [ -d ${SRC_PATH}$i ];then
#			rm -rf ${DST_PATH}${i}
#		fi
#		mv ${SRC_PATH}$i $DST_PATH
#	fi
#done

rm -rf ${BIN}
