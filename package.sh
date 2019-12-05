PACKAGE_NAME=gzui
SOURCE="../lib/libpmd.so gz ./update.sh ./resume.sh setting hb_cfg.xml hb_descp.xml ph_descp.xml cali_config.xml error_code.xml ../resources fonts"
INSTALL=
DST_PATH=${INSTALL}${PACKAGE_NAME}

mkdir -p ${DST_PATH}
cp -rf ${SOURCE} ${DST_PATH}
#tar -zcf - ${PACKAGE_NAME} | openssl des3 -salt -k 123456 -out ${PACKAGE_NAME}.tar.gz
tar -zcf  ${PACKAGE_NAME}.tar.gz ${PACKAGE_NAME}
rm -rf ${INSTALL}




