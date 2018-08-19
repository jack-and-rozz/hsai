#!/bin/bash
#exe_path="/Users/oda/LegendsOfCodeAndMagic/NWTraining/a.out"
simulator_dir=simulator
exe_path1=$3
exe_path2=$simulator_dir/noNWAgent
jar_path=$simulator_dir/locam.jar

model_dir=$1
model_replay_path=$2
model_replay_path2=$2/gote
model_replay_path1=$2/sente

usage() {
    echo "Usage:$0 model_dir log_dir exe_path1"
    exit 1
}

if [ $# -lt 3 ];then
    usage;
fi
if [ ! -e $model_replay_path  ]; then
   mkdir -p $model_replay_path
fi
if [ ! -e $model_replay_path1  ]; then
   mkdir -p $model_replay_path1
fi
if [ ! -e $model_replay_path2  ]; then
   mkdir -p $model_replay_path2
fi

for i in `seq 0 33`
do
  echo "java -jar $jar_path $exe_path1 $exe_path2 $model_dir $model_replay_path1"
  java -jar $jar_path $exe_path1 $exe_path2 $model_dir $model_replay_path1
done
for i in `seq 0 33`
do
  echo "java -jar $jar_path $exe_path2 $exe_path1 $model_dir $model_replay_path2"
  java -jar $jar_path $exe_path2 $exe_path1 $model_dir $model_replay_path2
done

