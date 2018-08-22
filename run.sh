#!/bin/bash
usage() {
    echo "Usage:$0 model_dir"
    exit 1
}
if [ $# -lt 1 ];then
    usage;
fi
model_dir=$1
nohup python main.py $model_dir train >logs/learning.log 2>logs/learning.err&
nohup ./simulator/checkjar.sh $model_dir >logs/sample.log 2>logs/sample.err&
