#!/bin/bash
#exe_path="/Users/oda/LegendsOfCodeAndMagic/NWTraining/a.out"
simulator_dir=simulator
logparser_dir=logAnalyzer
exe_path1=$simulator_dir/NNAgent_evaluation
exe_path2=$simulator_dir/noNNAgent_evaluation
jar_path=$simulator_dir/locam.jar

model_dir=$1
model_replay_path=$model_dir/evaluation
model_replay_path1=$model_dir/evaluation/sente
model_replay_path2=$model_dir/evaluation/gote

usage() {
    echo "Usage:$0 model_dir exe_path1"
    exit 1
}

if [ $# -lt 2 ];then
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

for i in `seq 0 9`
do
  echo "java -jar $jar_path $exe_path1 $exe_path2 $model_dir $model_replay_path1"
  gtimeout -sKILL 60 java -jar $jar_path $exe_path1 $exe_path2 $model_dir $model_replay_path1 None random
done
for i in `seq 0 9`
do
  echo "java -jar $jar_path $exe_path2 $exe_path1 $model_dir $model_replay_path2"
  gtimeout -sKILL 60 java -jar $jar_path $exe_path2 $exe_path1 $model_dir $model_replay_path2 None random
done

for filepath in $model_replay_path1/*; do
  $logparser_dir/logparser $filepath wins > $filepath/sente_summary
done

for filepath in $model_replay_path2/*; do
  $logparser_dir/logparser $filepath wins > $filepath/gote_summary
done

echo "done!"
