#include <dirent.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include <cstring>
#include <sys/stat.h>

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

// base64 エンコード
bool encode_base64(const std::vector<unsigned char>& src, std::string& dst)
{
    const std::string table("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/");
    std::string       cdst;

    for (std::size_t i = 0; i < src.size(); ++i) {
        switch (i % 3) {
        case 0:
            cdst.push_back(table[(src[i] & 0xFC) >> 2]);
            if (i + 1 == src.size()) {
                cdst.push_back(table[(src[i] & 0x03) << 4]);
                cdst.push_back('=');
                cdst.push_back('=');
            }

            break;
        case 1:
            cdst.push_back(table[((src[i - 1] & 0x03) << 4) | ((src[i + 0] & 0xF0) >> 4)]);
            if (i + 1 == src.size()) {
                cdst.push_back(table[(src[i] & 0x0F) << 2]);
                cdst.push_back('=');
            }

            break;
        case 2:
            cdst.push_back(table[((src[i - 1] & 0x0F) << 2) | ((src[i + 0] & 0xC0) >> 6)]);
            cdst.push_back(table[src[i] & 0x3F]);

            break;
        }
    }

    dst.swap(cdst);

    return true;
}

int main(int argc,char *argv[]){
    
    vector<string> fileNames;
    int i, n, dirElements, parentDirElements;
    string search_path;

    struct stat stat_buf;
    struct dirent **namelist=NULL;
    struct dirent **parentNameList=NULL;

    double** cardValue = new double*[2];
    double** cardDataCount = new double*[2];
    for(int n = 0; n < 2; n ++){
        cardValue[n] = new double[160];
        cardDataCount[n] = new double[160];
        for(int i = 0; i < 160; i ++){
            cardValue[n][i] = 0;
            cardDataCount[n][i] = 0;
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


    int* cardCosts = new int[160];
    int* cardTypes = new int[160];
    int totalMatches = 0;

    long*** cardSoukanWin = new long**[2];
    long*** cardSoukanLose = new long**[2];
    for(int n = 0; n < 2; n ++){
        cardSoukanWin[n] = new long*[160];
        cardSoukanLose[n] = new long*[160];
        for(int i = 0; i < 160; i ++){
            cardSoukanWin[n][i] = new long[160];
            cardSoukanLose[n][i] = new long[160];
            for(int j = 0; j < 160; j ++){
                cardSoukanWin[n][i][j] = 0;
                cardSoukanLose[n][i][j] = 0;
            }
        }
    }


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
            cout << "parent " << string(parentNameList[n] -> d_name) <<  endl;
            stat((parentDir + string(parentNameList[n] -> d_name)).c_str(), &stat_buf);
            if ((stat_buf.st_mode & S_IFMT) == S_IFDIR){
                // 再帰によりディレクトリ内を探索
                string searchDir = parentDir + string(parentNameList[n] -> d_name) + "/00/", fileNames;

                cout << "target " << searchDir <<  endl;

                // dirElements にはディレクトリ内の要素数が入る
                dirElements = scandir(searchDir.c_str(), &namelist, NULL, NULL);

                if(dirElements == -1) {
                    cout << "ERROR" <<  endl;
                }

                else{

                    if(dirElements < 300){
                        cout << string(parentNameList[n] -> d_name) << " has not enough log" <<  endl;
                        continue;
                    }
                    cout << "scan " << string(parentNameList[n] -> d_name) <<  endl;

                    long** cardWin = new long*[2];
                    cardWin[0] = new long[160];
                    cardWin[1] = new long[160];
                    long** cardLose = new long*[2];
                    cardLose[0] = new long[160];
                    cardLose[1] = new long[160];
                    long** noCardWin = new long*[2];
                    noCardWin[0] = new long[160];
                    noCardWin[1] = new long[160];
                    long** noCardLose = new long*[2];
                    noCardLose[0] = new long[160];
                    noCardLose[1] = new long[160];
                    int* playerWin = new int[2];
                    playerWin[0] = 0;
                    playerWin[1] = 0;

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

                    for(int i = 0; i < 160; i ++){
                        cardWin[0][i] = 0;
                        cardLose[0][i] = 0;
                        cardWin[1][i] = 0;
                        cardLose[1][i] = 0;
                    }

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
                                if(result[0] == "-1" || result[1] == "-1"){
                                    errorFlag = true;
                                    cout << "-1error" << matchID <<  endl;
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

                            totalMatches ++;

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
                                if (lineNo % 3 == 1){
                                    deckString = split(str, ' ');
                                }

                                if (lineNo % 3 == 0){
                                    int pick = std::atoi(str.c_str());
                                    for(int i = 0; i < 160; i ++){
                                        int cnt = std::atoi(deckString[i].c_str());
                                        if(winPlayer == 1){
                                            cardSoukanWin[0][i][pick] += cnt;
                                            cardSoukanWin[0][pick][i] += cnt;
                                        }
                                        else{
                                            cardSoukanLose[0][i][pick] += cnt;
                                            cardSoukanLose[0][pick][i] += cnt;
                                        }
                                    }
                                }

                                if(lineNo == 90){
                                    int* manaCurve = new int[13];
                                    int* typeTotal = new int[4];
                                    int* currentTotal = new int[160];
                                    for(int i = 0; i < 13; i ++){
                                        manaCurve[i] = 0;
                                    }
                                    for(int i = 0; i < 4; i ++){
                                        typeTotal[i] = 0;
                                    }
                                    for(int i = 0; i < 160; i ++){
                                        int cnt = std::atoi(deckString[i].c_str());
                                        if(i == std::atoi(str.c_str())){
                                            cnt ++;
                                        }
                                        currentTotal[i] = cnt;
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
                                    /*for(int i = 0; i < 160; i ++){
                                        for(int n = 0; n < 160; n ++){
                                            if(winPlayer == 1){
                                                cardSoukanWin[0][i][n] += min(currentTotal[i], currentTotal[n]);
                                            }
                                            else{
                                                cardSoukanLose[0][i][n] += min(currentTotal[i], currentTotal[n]);
                                            }
                                        }
                                    }*/
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
                                if (lineNo % 3 == 1){
                                    deckString = split(str, ' ');
                                }

                                if (lineNo % 3 == 0){
                                    int pick = std::atoi(str.c_str());
                                    for(int i = 0; i < 160; i ++){
                                        int cnt = std::atoi(deckString[i].c_str());
                                        if(winPlayer == 2){
                                            cardSoukanWin[1][i][pick] += cnt;
                                            cardSoukanWin[1][pick][i] += cnt;
                                        }
                                        else{
                                            cardSoukanLose[1][i][pick] += cnt;
                                            cardSoukanLose[1][pick][i] += cnt;
                                        }
                                    }
                                }

                                if(lineNo == 90){
                                    int* manaCurve = new int[13];
                                    int* typeTotal = new int[4];
                                    int* currentTotal = new int[160];
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
                                        currentTotal[i] = cnt;
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
                    for(int n = 0; n < 2; n ++){
                        for(int i = 0; i < 160; i ++){
                            double winRate = (double)cardTotalWin[n][i][0] / (double)(cardTotalWin[n][i][0] + cardTotalLose[n][i][0]); //カードを取らない場合の勝率
                            double currentCardValue = (double)cardWin[n][i] / (double)(cardWin[n][i] + cardLose[n][i]) - winRate;
                            double currentCount = (double)(cardWin[n][i] + cardLose[n][i]);
                            if(currentCount == 0 || cardTotalWin[n][i][0] + cardTotalLose[n][i][0] == 0){
                                continue;
                            }
                            cardValue[n][i] = (cardValue[n][i] * cardDataCount[n][i] + currentCardValue * currentCount) / (currentCount + cardDataCount[n][i]);
                            cardDataCount[n][i] += currentCount;
                        }
                    }
                }
            }
        }
    }

    cout << fixed;
    // setprecisionで小数点以下8桁を出すように
    cout << setprecision(3);

    for(int n = 0; n < 2; n ++){
        cout << "player:" << n << endl;
        for(int i = 0; i < 160; i ++){
            cout << i + 1 << ":" << cardValue[n][i] << endl;
        }
    }

    for(int n = 0; n < 2; n ++){
        cout << "{";
        for(int i = 0; i < 160; i ++){
            cout << cardValue[n][i] << ",";
        }
        cout << "}:" << endl;
    }

    for(int n = 0; n < 2; n ++){
        cout << "----------" << endl;
        cout << "{";
        for(int i = 0; i < 160; i ++){
            cout << "{";
            for(int j = 0; j < 160; j ++){
                cout << ((double)cardSoukanWin[n][i][j] / (double)(cardSoukanWin[n][i][j] + cardSoukanLose[n][i][j]));
                if(j != 159){
                    cout << ",";
                }
            }
            cout << "},";
        }
        cout << "};" << endl;
    }
    cout << "----------" << endl;
    cout << "{";
    for(int i = 0; i < 160; i ++){
        cout << "{";
        for(int j = 0; j < 160; j ++){
            cout << ((double)(cardSoukanWin[0][i][j] + cardSoukanWin[1][i][j]) / (double)(cardSoukanWin[0][i][j] + cardSoukanLose[0][i][j] + cardSoukanWin[1][i][j] + cardSoukanLose[1][i][j]));
            if(j != 159){
                cout << ",";
            }
        }
        cout << "},";
    }
    cout << "};" << endl;  

    cout << "------assyuku----" << endl;
    cout << "\"";
    for(int i = 0; i < 160; i ++){
        for(int j = i; j < 160; j ++){
            double target = ((double)(cardSoukanWin[0][i][j] + cardSoukanWin[1][i][j]) / (double)(cardSoukanWin[0][i][j] + cardSoukanLose[0][i][j] + cardSoukanWin[1][i][j] + cardSoukanLose[1][i][j]));
            int intTarget = target * 10000;
            cout << intTarget;
        }
    }
    cout << "\"" << endl;

    cout << "totalMatch:" << totalMatches << endl;

    free(namelist);
}