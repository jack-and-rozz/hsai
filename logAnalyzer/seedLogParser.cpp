#include <sys/stat.h>
#include <dirent.h>
#include <iostream>
#include <vector>
#include <fstream>

using namespace std;


std::vector<std::string> split(std::string str, char del) {
    int first = 0;
    int last = str.find_first_of(del);
 
    std::vector<std::string> result;
 
    while (first < str.size()) {
        std::string subStr(str, first, last - first);
 
        result.push_back(subStr);
 
        first = last + 1;
        last = str.find_first_of(del, first);
 
        if (last == std::string::npos) {
            last = str.size();
        }
    }
 
    return result;
}

int main(int argc,char *argv[]){
    
    vector<string> fileNames;
    int i, n, dirElements, parentDirElements;
    string search_path;

    struct stat stat_buf;
    struct dirent **namelist=NULL;
    struct dirent **parentNameList=NULL;

    long** cardWin = new long*[2];
    cardWin[0] = new long[160];
    cardWin[1] = new long[160];
    long** cardLose = new long*[2];
    cardLose[0] = new long[160];
    cardLose[1] = new long[160];
    
    int* cardCosts = new int[160];
    int* cardTypes = new int[160];

    for(int i = 0; i < 160; i ++){
        cardWin[0][i] = 0;
        cardLose[0][i] = 0;
        cardWin[1][i] = 0;
        cardLose[1][i] = 0;
    }

    long*** cardTotalWin = new long**[2];
    cardTotalWin[0] = new long*[160];
    cardTotalWin[1] = new long*[160];
    long*** cardTotalLose = new long**[2];
    cardTotalLose[0] = new long*[160];
    cardTotalLose[1] = new long*[160];
    for(int i = 0; i < 160; i ++){
        for(int n = 0; n < 2; n ++){
            cardTotalWin[n][i] = new long[31];
            cardTotalLose[n][i] = new long[31];
            for(int j = 0; j < 31; j ++){
                cardTotalWin[n][i][j] = 0;
                cardTotalLose[n][i][j] = 0;
            }
        }
    }

    long*** manaCurveWin = new long** [2];
    manaCurveWin[0] = new long*[13];
    manaCurveWin[1] = new long*[13];
    long*** manaCurveLose = new long** [2];
    manaCurveLose[0] = new long*[13];
    manaCurveLose[1] = new long*[13];
    for(int i = 0; i < 13; i ++){
        manaCurveWin[0][i] = new long[31];
        manaCurveLose[0][i] = new long[31];
        manaCurveWin[1][i] = new long[31];
        manaCurveLose[1][i] = new long[31];
    }

    long*** cardTypeWin = new long**[2];
    long*** cardTypeLose = new long**[2];
    for(int n = 0; n < 2; n ++){
        cardTypeWin[n] = new long*[4];
        cardTypeLose[n] = new long*[4];
        for(int i = 0; i < 4; i ++){
            cardTypeWin[n][i] = new long[31];
            cardTypeLose[n][i] = new long[31];
            for(int j = 0; j < 31; j ++){
                cardTypeWin[n][i][j] = 0;
                cardTypeLose[n][i][j] = 0;
            }
        }
    }

    int* playerWin = new int[2];
    playerWin[0] = 0;
    playerWin[1] = 0;

    // parse cardlist
    std::ifstream ifs("cardlist.txt");
    std::string str;
    if (ifs.fail())
    {
        std::cerr << "失敗" << std::endl;
        return -1;
    }
    int id = 0;
    while (getline(ifs, str))
    {
        vector<string> result = split(str, ';');
        cardCosts[id] = std::atoi(result[3].c_str());
        if(result[2] == " creature "){
            cardTypes[id] = 0;
        }
        if(result[2] == " itemGreen "){
            cardTypes[id] = 1;
        }
        if(result[2] == " itemRed "){
            cardTypes[id] = 2;
        }
        if(result[2] == " itemBlue "){
            cardTypes[id] = 3;
        }
        id ++;
    }

    string parentDir = argv[1];
    string onlyWins = argv[2];

    // dirElements にはディレクトリ内の要素数が入る
    parentDirElements = scandir(parentDir.c_str(), &parentNameList, NULL, NULL);

    if(parentDirElements == -1) {
        cout << "ERROR" <<  endl;
    }

    else{

        //ディレクトリかファイルかを順番に識別
        for (n=0; n<parentDirElements; n+=1) {

            if ((stat_buf.st_mode & S_IFMT) == S_IFDIR){
                // 再帰によりディレクトリ内を探索
                string searchDir = parentDir + string(parentNameList[i] -> d_name) + "/00/", fileNames;

                cout << "target " << searchDir <<  endl;

                // dirElements にはディレクトリ内の要素数が入る
                dirElements = scandir(searchDir.c_str(), &namelist, NULL, NULL);

                if(dirElements == -1) {
                    cout << "ERROR" <<  endl;
                }

                else{

                    if(dirElements < 600){
                        cout << string(parentNameList[n] -> d_name) << " has not enough log" <<  endl;
                        continue;
                    }
                    cout << "scan " << string(parentNameList[n] -> d_name) <<  endl;

                    //ディレクトリかファイルかを順番に識別
                    for (i=0; i<dirElements; i+=1) {
                        int index = string(namelist[i]->d_name).find("result-");
                        if(index != std::string::npos){
                            string matchID = string(namelist[i]->d_name).substr(index + 7, 12);
                            int winPlayer;

                            // load result
                            std::ifstream ifs(searchDir + "/result-" + matchID + ".txt");
                            if (ifs.fail())
                            {
                                std::cerr << "失敗" << std::endl;
                                continue;
                            }
                            bool errorFlag = false;
                            while (getline(ifs, str))
                            {
                                vector<string> result = split(str, ' ');
                                if(result[0] == "-1" || result[0] == "-1"){
                                    errorFlag = true;
                                    break;
                                }
                                if(result[0] == "1"){
                                    winPlayer = 1;
                                    playerWin[0] ++;
                                }
                                else{
                                    winPlayer = 2;
                                    playerWin[1] ++;
                                }
                            }
                            if(errorFlag){
                                continue;
                            }

                            if(onlyWins == "wins"){
                                continue;
                            }

                            // load final deck player 1
                            std::ifstream ifs1(searchDir + "/player1-" + matchID + ".txt");
                            if (ifs1.fail())
                            {
                                std::cerr << "失敗" << std::endl;
                                continue;
                            }
                            int lineNo = 0;
                            
                            vector<string> deckString;
                            while (getline(ifs1, str))
                            {
                                lineNo ++;
                                if(lineNo == 88){
                                    deckString = split(str, ' ');
                                }
                                if(lineNo == 90){
                                    int* manaCurve = new int[13];
                                    int* typeTotal = new int[4];
                                    for(int i = 0; i < 13; i ++){
                                        manaCurve[i] = 0;
                                    }
                                    for(int i = 0; i < 4; i ++){
                                        typeTotal[i] = 0;
                                    }
                                    for(int i = 0; i < 160; i ++){
                                        int cnt = std::atoi(deckString[i].c_str());
                                        if(i == std::atoi(str.c_str()) - 1){
                                            cnt ++;
                                        }
                                        manaCurve[cardCosts[i]] += cnt;
                                        typeTotal[cardTypes[i]] += cnt;
                                        if(winPlayer == 1){
                                            cardWin[0][i] += cnt;
                                            cardTotalWin[0][i][cnt] += 1;
                                        }
                                        else{
                                            cardLose[0][i] += cnt;
                                            cardTotalLose[0][i][cnt] += 1;
                                        }
                                    }
                                    for(int i = 0; i < 13; i ++){
                                        if(winPlayer == 1){
                                            manaCurveWin[0][i][manaCurve[i]] += 1;
                                        }
                                        else{
                                            manaCurveLose[0][i][manaCurve[i]] += 1;
                                        }
                                    }
                                    for(int i = 0; i < 4; i ++){
                                        if(winPlayer == 1){
                                            cardTypeWin[0][i][typeTotal[i]] += 1;
                                        }
                                        else{
                                            cardTypeLose[0][i][typeTotal[i]] += 1;
                                        }
                                    }
                                    break;
                                }
                            }

                            // load final deck player 1
                            std::ifstream ifs2(searchDir + "/player2-" + matchID + ".txt");
                            if (ifs2.fail())
                            {
                                std::cerr << "失敗" << std::endl;
                                continue;
                            }
                            lineNo = 0;
                            while (getline(ifs2, str))
                            {
                                lineNo ++;
                                if(lineNo == 88){
                                    deckString = split(str, ' ');
                                }
                                if(lineNo == 90){
                                    int* manaCurve = new int[13];
                                    int* typeTotal = new int[4];
                                    for(int i = 0; i < 13; i ++){
                                        manaCurve[i] = 0;
                                    }
                                    for(int i = 0; i < 4; i ++){
                                        typeTotal[i] = 0;
                                    }
                                    for(int i = 0; i < 160; i ++){
                                        int cnt = std::atoi(deckString[i].c_str());
                                        if(i == std::atoi(str.c_str()) - 1){
                                            cnt ++;
                                        }
                                        manaCurve[cardCosts[i]] += cnt;
                                        typeTotal[cardTypes[i]] += cnt;
                                        if(winPlayer == 2){
                                            cardWin[1][i] += cnt;
                                            cardTotalWin[1][i][cnt] += 1;
                                        }
                                        else{
                                            cardLose[1][i] += cnt;
                                            cardTotalLose[1][i][cnt] += 1;
                                        }
                                    }
                                    for(int i = 0; i < 13; i ++){
                                        if(winPlayer == 2){
                                            manaCurveWin[1][i][manaCurve[i]] += 1;
                                        }
                                        else{
                                            manaCurveLose[1][i][manaCurve[i]] += 1;
                                        }
                                    }
                                    for(int i = 0; i < 4; i ++){
                                        if(winPlayer == 2){
                                            cardTypeWin[1][i][typeTotal[i]] += 1;
                                        }
                                        else{
                                            cardTypeLose[1][i][typeTotal[i]] += 1;
                                        }
                                    }
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    double avarageWinRate[2];
    cout << "totalScan:" << (playerWin[0] + playerWin[1]) / 2 << endl;
    for(int i = 0; i < 2; i ++){
        avarageWinRate[i] = (double)playerWin[i] / (double)(playerWin[0] + playerWin[1]);
    }
    cout << avarageWinRate[0] << "/" << avarageWinRate[1] << endl;
    if(onlyWins == "wins"){
        return 0;
    }
    for(int i = 0; i < 160; i ++){
        //double winRate1 = (double)cardWin[0][i] / (double)(cardWin[0][i] + cardLose[0][i]);
        //double winRate2 = (double)cardWin[1][i] / (double)(cardWin[1][i] + cardLose[1][i]);
        //cout << i + 1 << ":" << winRate1 - avarageWinRate[0] << "/" << winRate2 - avarageWinRate[1] << endl;
        double winRate1 = (double)(cardWin[0][i] + cardWin[1][i]) / (double)(cardWin[0][i] + cardLose[0][i] + cardWin[1][i] + cardLose[1][i]);
        cout << i + 1 << ":" << winRate1 - 0.5 << endl;
    }

    cout << "sente:"  << endl;
    for(int i = 0; i < 160; i ++){
        double winRate1 = (double)(cardWin[0][i] ) / (double)(cardWin[0][i] + cardLose[0][i]);
        cout << winRate1 - avarageWinRate[0] << ",";
    }

    cout << "gote:"  << endl;
    for(int i = 0; i < 160; i ++){
        double winRate1 = (double)(cardWin[1][i]) / (double)(cardWin[1][i] + cardLose[1][i]);
        cout << winRate1 - avarageWinRate[1] << ",";
    }

    for(int i = 0; i < 13; i ++){
        cout << "ManaSente:" << i  << endl;
        for(int n = 0; n < 31; n ++){
            double winRate = (double)(manaCurveWin[0][i][n]) / (double)(manaCurveWin[0][i][n] + manaCurveLose[0][i][n]);
            cout << n << ":" << winRate << ",";
        }
        cout << endl;
    }

    for(int i = 0; i < 13; i ++){
        cout << "ManaGote:" << i  << endl;
        for(int n = 0; n < 31; n ++){
            double winRate = (double)(manaCurveWin[1][i][n]) / (double)(manaCurveWin[1][i][n] + manaCurveLose[1][i][n]);
            cout << n << ":" << winRate << ",";
        }
        cout << endl;
    }


    for(int j = 0; j < 2; j ++){
        cout << "manaCurve[" << i << "]"; 
        for(int i = 0; i < 13; i ++){
            cout << "={";
            for(int n = 0; n < 31; n ++){
                double winRate = (double)(manaCurveWin[j][i][n]) / (double)(manaCurveWin[j][i][n] + manaCurveLose[j][i][n]);
                cout << winRate << ",";
            }
            cout << "}," << endl;
        }
    }

    int* totalManaCurve = new int[13];
    for(int i = 0; i < 13; i ++){
        totalManaCurve[i] = 0;
    }
    for(int i = 0; i < 160; i ++){
        totalManaCurve[cardCosts[i]] ++;
    }
    for(int i = 0; i < 13; i ++){
        cout << totalManaCurve[i] << ",";
    }

    cout << endl << "****card: totalcount****" << endl;
    for(int i = 0; i < 160; i ++){
        cout << "card:" << i + 1 << endl;
        for(int n = 0; n < 31; n ++){
            double winRate1 = (double)(cardTotalWin[0][i][n] + cardTotalWin[1][i][n]) / (double)(cardTotalWin[0][i][n] + cardTotalLose[0][i][n] + cardTotalWin[1][i][n] + cardTotalLose[1][i][n]);
            cout << n << ":" << winRate1 << " ";
        }
        cout << endl;
    }

    cout << "cardWinrate={"; 
    for(int i = 0; i < 160; i ++){
        cout << "{"; 
        for(int n = 0; n < 31; n ++){
            double winRate1 = (double)(cardTotalWin[0][i][n]) / (double)(cardTotalWin[0][i][n] + cardTotalLose[0][i][n]);
            cout << winRate1;
            if(n != 30){
                cout << ",";
            }
        }
        cout << "}," << endl;
    }
    cout << "};" << endl;

    cout << "cardWinrate={"; 
    for(int i = 0; i < 160; i ++){
        cout << "{"; 
        for(int n = 0; n < 31; n ++){
            double winRate1 = (double)(cardTotalWin[1][i][n]) / (double)(cardTotalWin[1][i][n] + cardTotalLose[1][i][n]);
            cout << winRate1;
            if(n != 30){
                cout << ",";
            }
        }
        cout << "}," << endl;
    }
    cout << "};" << endl;

    cout << endl << "****card: typecount****" << endl;
    for(int i = 0; i < 4; i ++){
        cout << "Type:" << i  << endl;
        for(int n = 0; n < 31; n ++){
            double winRate = (double)(cardTypeWin[0][i][n] + cardTypeWin[1][i][n]) / (double)(cardTypeWin[0][i][n] + cardTypeLose[0][i][n] + cardTypeWin[1][i][n] + cardTypeLose[1][i][n]);
            cout << n << ":" << winRate << ",";
        }
        cout << endl;
    }

    free(namelist);
}