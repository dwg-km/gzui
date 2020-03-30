
#array=$(ps -ef | grep gz | head -n 1)

#for var in ${array[@]}
#do
#   echo $var
#done 

while true
do
#pid=$(pgrep gz)
pid=$(pidof gz)
if [ -n "$pid" ];then
	echo $pid
else
version=$(~/gz_run.sh -d)
	if [ "$version" = "release" ];then
	~/gz_run.sh &
	fi
fi
sleep 3
done
