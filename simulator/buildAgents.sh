cp source/NNAgent_evaluation.hpp source/mainAI.hpp
g++ source/mainAI.cpp  -o NNAgent_evaluation -std=c++14

cp source/NNAgent_sampling.hpp source/mainAI.hpp
g++ source/mainAI.cpp  -o NNAgent_sampling -std=c++14

cp source/NoNNAgent_sampling.hpp source/mainAI.hpp
g++ source/mainAI.cpp  -o NoNNAgent_sampling -std=c++14

cp source/NoNNAgent_evaluation.hpp source/mainAI.hpp
g++ source/mainAI.cpp  -o NoNNAgent_evaluation -std=c++14

g++ ../logAnalyzer/logparser.cpp -o logparser -std=c++14
