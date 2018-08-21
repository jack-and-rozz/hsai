cp source/NNAgent_evaluation.hpp source/mainAI.hpp
g++ source/mainAI.cpp  -o NNAgent_evaluation

cp source/NNAgent_sampling.hpp source/mainAI.hpp
g++ source/mainAI.cpp  -o NNAgent_sampling

cp source/NoNNAgent_sampling.hpp source/mainAI.hpp
g++ source/mainAI.cpp  -o NoNNAgent_sampling

cp source/NoNNAgent_evaluation.hpp source/mainAI.hpp
g++ source/mainAI.cpp  -o NoNNAgent_evaluation
