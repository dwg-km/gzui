#/usr/bin/bash
pid=$(ps | grep gzSettingUI | cut -d ' ' -f1)
if [ ! -n $pid ]; then
	kill -9 ${pid}
fi

cp -rf  ./setting_bak ./setting

./gzSettingUI

