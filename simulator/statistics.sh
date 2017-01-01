#!/bin/bash
#exe_path="/Users/oda/LegendsOfCodeAndMagic/NWTraining/a.out"
simulator_dir=simulator
exe_path1=$simulator_dir/noNWAgent
exe_path2=$simulator_dir/noNWAgent
jar_path=$simulator_dir/locam.jar

model_dir=$1
log_dir=$2
model_replay_path=$model_dir/replays

usage() {
    echo "Usage:$0 model_dir log_dir"
    exit 1
}

if [ $# -lt 1 ];then
    usage;
fi
if [ ! -e ${log_dir}  ]; then
    mkdir -p $log_dir
fi


while true 
do
    seed=$RANDOM
    if [ ! -e ${log_dir}/${seed}  ]; then
        mkdir -p ${log_dir}/${seed}
    fi

    echo "java -jar $jar_path $exe_path1 $exe_path2 $model_dir ${log_dir}/${seed} $seed"
    for i in `seq 5`
    do
    java -jar $jar_path $exe_path1 $exe_path2 $model_dir ${log_dir}/${seed} $seed &
    java -jar $jar_path $exe_path1 $exe_path2 $model_dir ${log_dir}/${seed} $seed &
    java -jar $jar_path $exe_path1 $exe_path2 $model_dir ${log_dir}/${seed} $seed &
    java -jar $jar_path $exe_path1 $exe_path2 $model_dir ${log_dir}/${seed} $seed &
    java -jar $jar_path $exe_path1 $exe_path2 $model_dir ${log_dir}/${seed} $seed &
    java -jar $jar_path $exe_path1 $exe_path2 $model_dir ${log_dir}/${seed} $seed &
    java -jar $jar_path $exe_path1 $exe_path2 $model_dir ${log_dir}/${seed} $seed &
    java -jar $jar_path $exe_path1 $exe_path2 $model_dir ${log_dir}/${seed} $seed &
    java -jar $jar_path $exe_path1 $exe_path2 $model_dir ${log_dir}/${seed} $seed &
    java -jar $jar_path $exe_path1 $exe_path2 $model_dir ${log_dir}/${seed} $seed &
    wait
    done
done
