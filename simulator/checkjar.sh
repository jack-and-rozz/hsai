#!/bin/bash
#exe_path="/Users/oda/LegendsOfCodeAndMagic/NWTraining/a.out"
simulator_dir=simulator

model_dir=$1
model_replay_path=$model_dir/replays
exe_path1=$simulator_dir/NNAgent_sampling
exe_path2=$simulator_dir/noNNAgent_sampling
jar_path=$simulator_dir/locam.jar
timeout=timeout

usage() {
    echo "Usage:$0 model_dir agent1_path agent2_path"
    exit 1
}

if [ $# -lt 1 ];then
    usage;
fi
if [ ! -e $model_replay_path  ]; then
   mkdir -p $model_replay_path
fi

while true 
do
    echo "java -jar $jar_path $exe_path1 $exe_path2 $model_dir $model_replay_path"
    $timeout -sKILL 20 java -jar $jar_path $exe_path1 $exe_path2 $model_dir $model_replay_path &
    $timeout -sKILL 20 java -jar $jar_path $exe_path2 $exe_path1 $model_dir $model_replay_path &
    wait
done
