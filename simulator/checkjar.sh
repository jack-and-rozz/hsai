#!/bin/bash
#exe_path="/Users/oda/LegendsOfCodeAndMagic/NWTraining/a.out"
simulator_dir=simulator
exe_path1=$simulator_dir/simulator
exe_path2=$simulator_dir/simulator
jar_path=$simulator_dir/locam.jar

model_dir=$1
model_replay_path=$model_dir/replays

usage() {
    echo "Usage:$0 model_dir"
    exit 1
}

if [ $# -lt 1 ];then
    usage;
fi
if [ ! -e $model_log_path  ]; then
   mkdir -p $model_log_path
fi


while true 
do
    echo "java -jar $jar_path $exe_path1 $exe_path2 $model_dir $model_replay_path"
    java -jar $jar_path $exe_path1 $exe_path2 $model_dir $model_replay_path
done
