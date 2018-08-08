#!/bin/bash
#exe_path="/Users/oda/LegendsOfCodeAndMagic/NWTraining/a.out"
simulator_dir=simulator
exe_path1=$simulator_dir/noNWAgent
exe_path2=$simulator_dir/noNWAgent
jar_path=$simulator_dir/locam.jar

log_dir=$1
model_dir=$2
model_replay_path=$model_dir/replays

usage() {
    echo "Usage:$0 model_dir log_dir"
    exit 1
}

if [ $# -lt 1 ];then
    usage;
fi
if [ ! -e $log_dir  ]; then
   mkdir -p $log_dir
fi


while true 
do
    echo "java -jar $jar_path $exe_path1 $exe_path2 $model_dir $log_dir"
    java -jar $jar_path $exe_path1 $exe_path2 $model_dir $log_dir &
    java -jar $jar_path $exe_path1 $exe_path2 $model_dir $log_dir &
    wait
done
