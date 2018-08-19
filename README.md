# hsai

### How to use

* To run trainer

``` python main.py ${model_path} train ```

* To run simulator for sampling replays

``` ./simulator/checkjar.sh ${model_path} ```

* To make testing games against a rule-based baseline
```
./simulator/evaluateAgent.sh trained_models/tmp ./tmplog
```

* To evaluate model (against noNWAgent)
```
./simulator/evaluateAgent.sh ${model_dir} ${agent_path}

./simulator/evaluateAgent.sh trained_models/tmp log_test simulator/simulator
```