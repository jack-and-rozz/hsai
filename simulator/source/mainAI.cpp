#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <random>
#include <fstream>
#include <chrono>

//8/4 use card value
// 80 to 15! so strong

#define DEBUG
#define RANDOM_PICK
//#define READ_EPOCH_FILE
//#define TEKAGEN

#ifdef READ_EPOCH_FILE
#include <dirent.h>
#include <cctype>
#include <algorithm>
#include <sys/stat.h>
#include <sys/types.h>
#endif

#ifdef DEBUG
#include<iostream>
#include<fstream>
#endif

double cardValues[160] = {0.000433529,-0.0037734,0.0028331,-0.00276243,0.00866409,0.00926344,0.029362,0.00190389,0.00771829,-0.0085139,0.00581557,-0.00253273,0.00805238,-0.0130036,0.00763868,-0.00624154,0.0089192,0.0130663,0.00519659,-0.00748882,0.00147536,-0.00139611,0.00551419,-0.00618836,0.00153689,0.000873843,0.00477187,0.000716053,0.00987119,-0.000576243,-0.00940427,0.00990177,0.00820396,-0.00255504,-0.0107639,0.000583625,0.0121412,-0.00073514,-0.00222392,-0.00431018,-0.00395841,-0.0183421,-0.00577225,0.0173889,-0.00909391,-0.00408303,0,0.00701637,0.0153841,0.00628554,0.0268336,0.00478309,0.0175815,0.00212425,-0.0197821,-0.00298329,-0.0164201,-0.0026735,-0.000324628,-0.00508684,0.00347833,-0.00198799,-0.00943521,0.00879462,0.0269608,0.0134584,0.0148968,0.0268221,0.0128387,-0.00183205,-0.0080639,0.00564646,0.0012535,-0.00336064,0.00918993,-0.0122763,0.000566452,-0.00734073,0.00065331,0.014409,0.00213692,0.00267892,-0.00134645,0.00707656,0.00295596,-0.0021922,0,0.00973341,-0.00481232,-0.00470274,0.00171294,-0.00863851,-3.54887e-05,-0.00495691,0.0114015,0.00413277,0.00348691,-0.00129351,0.00659267,-0.000100164,-0.0088968,-0.0131982,0.0147266,0.00447027,0.00678091,0.00324469,-0.0110531,-0.00837989,0.00685987,-0.029233,0.00775385,-0.00119425,-0.0179728,0.00456286,0.00147749,0.0126015,-0.00583768,-0.000873908,0.00163193,-0.00624224,0.00362457,0.00108682,-0.00808074,-0.0122458,-0.00222169,0.00195875,-0.00252306,0.00807992,0.00380313,-0.00646744,-0.00850858,-0.00819513,0.0121858,-0.00390422,0.00307388,-0.00422738,-0.00375989,-0.0140479,0.013371,-0.033683,0.0047868,-0.00601638,-0.0131875,0.00265587,0.00170743,-0.00901371,0.00466675,-0.000131648,-0.00772928,0.00934158,0.0150233,0.00444484,-0.0306787,-0.0165606,0.000195894,-0.0167981,0.00228041,0.00319721,-0.0113772,-0.0294801};
//TODO think about card advantage
//TODO 

// 0805 honkide 13th

using namespace std;

double** mulMatrix(double** a,double** b, int r1, int c1, int r2, int c2)
{
    // If column of first matrix in not equal to row of second matrix,
    // ask the user to enter the size of matrix again.
    while (c1!=r2)
    {
        cerr << "Error! column of first matrix not equal to row of second.";
    }

    cerr << "1" << endl;

    double** result = new double*[r1];
    for(int i = 0; i < r1; i++){
        result[i] = new double[c2];
    }

    cerr << "2" << endl;

    // Initializing elements of matrix mult to 0.
    for(int i = 0; i < r1; i++){
        for(int j = 0; j < c2; j++)
        {
            result[i][j]=0;
        }
    }

    cerr << "3" << endl;

    // Multiplying matrix a and b and storing in array mult.
    for(int i = 0; i < r1; i++){
        for(int j = 0; j < c2; j++){
            for(int k = 0; k < c1; k++)
            {
                /*cerr << i << ":" << j << ":" << k << endl;
                cerr << result[i][j] << endl;
                cerr << a[i][k] << endl;
                cerr << b[k][j] << endl;*/
                result[i][j] += a[i][k] * b[k][j];
            }
        }
    }

    // Displaying the multiplication of two matrix.
    //cerr << "Output Matrix: " << endl;
    /*for(int i = 0; i < r1; i++)
    for(int j = 0; j < c2; j++)
    {
        //cerr << " " << result[i][j];
        if(j == c2-1)
            //cerr << endl;
    }*/
    return result;
}

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

class Card {
    int cardNumber;
    int instanceId;
    int location;
    int cardType;
    int cost;
    int attack;
    int defense;
    string abilities;
    int myHealthChange;
    int opponentHealthChange;
    int cardDraw;
    bool actionDone;
    int playScore;
    string command;
public:
    Card(int cardNumber, int instanceId, int location, int cardType, int cost, int attack, int defense, string abilities, int myHealthChange, int opponentHealthChange, int cardDraw, bool actionDone){
        this->cardNumber = cardNumber;
        this->instanceId = instanceId;
        this->location = location;
        this->cardType = cardType;
        this->cost = cost;
        this->attack = attack;
        this->defense = defense;
        this->abilities = abilities;
        this->myHealthChange = myHealthChange;
        this->opponentHealthChange = opponentHealthChange;
        this->cardDraw = cardDraw;
        this->actionDone = actionDone;
        this->playScore = 0;
    }

    Card* getCardCopy(){
        return new Card(this->cardNumber, this->instanceId, this->location, this->cardType, this->cost, this->attack, this->defense, this->abilities, this->myHealthChange, this->opponentHealthChange, this->cardDraw, this->actionDone);
    }

    int getDraw(){
        return cardDraw;
    }

    int getCost(){
        return cost;
    }

    int getID(){
        return instanceId;
    }

    int getAttack(){
        return attack;
    }

    int getDefense(){
        return defense;
    }

    string getAbilities(){
        return abilities;
    }

    bool isGuard(){
        return abilities.find("G") != std::string::npos;
    }

    bool isCharge(){
        return abilities.find("C") != std::string::npos;
    }

    bool isLethal(){
        return abilities.find("L") != std::string::npos;
    }

    bool isWard(){
        return abilities.find("W") != std::string::npos;
    }

    bool isDrain(){
        return abilities.find("D") != std::string::npos;
    }

    bool isBreakThrough(){
        return abilities.find("B") != std::string::npos;
    }

    bool getActionDone(){
        return actionDone;
    }

    void doAction(){
        this->actionDone = true;
    }

    int takeDamage(int damage){
        if(isWard()){
            std::replace(abilities.begin(), abilities.end(), 'W', '-');
        }
        else{
            defense -= damage;
        }
    }

    // hyokati calculation
    int calcBasicValue(){
        int base = attack + defense;
        if(isWard()){
            base += attack;
        }
        if(isGuard()){
            base += 1;
        }
        if(isDrain()){
            base += attack;
        }
        if(isBreakThrough()){
            if(attack >= 5){
                base += attack - 4;
            }
        }
        return base;
    }

    int getType(){
        return cardType;
    }

    bool operator<(const Card& another) const {
        return cost > another.cost;
    }

    int getPlayScore(){
        return playScore;
    }

    void setPlayScore(int score){
        this->playScore = score;
    }

    void buff(Card* card){
        if(card->isGuard() && !isGuard()){
            abilities += 'G';
        }
        if(card->isLethal() && !isLethal()){
            abilities += 'L';
        }
        if(card->isWard() && !isWard()){
            abilities += 'W';
        }
        if(card->isDrain() && !isDrain()){
            abilities += 'D';
        }
        if(card->isBreakThrough() && !isBreakThrough()){
            abilities += 'B';
        }
        attack += card->getAttack();
        defense += card->getDefense();
    }

    void deBuff(Card* card){
        if(card->isGuard()){
            std::replace(abilities.begin(), abilities.end(), 'G', '-');
        }
        if(card->isLethal()){
            std::replace(abilities.begin(), abilities.end(), 'L', '-');
        }
        if(card->isWard()){
            std::replace(abilities.begin(), abilities.end(), 'W', '-');
        }
        if(card->isDrain()){
            std::replace(abilities.begin(), abilities.end(), 'D', '-');
        }
        if(card->isBreakThrough()){
            std::replace(abilities.begin(), abilities.end(), 'B', '-');
        }
        attack += card->getAttack();
        if(attack < 0){
            attack = 0;
        }
        takeDamage(-card->getDefense());
    }

    void setCommand(string command){
        this->command = command;
    }

    string getCommand(){
        return command;
    }

    int getCardNumber(){
        return cardNumber;
    }

    int getMyHealthChange(){
        return myHealthChange;
    }

    int getOpponentHealthChange(){
        return opponentHealthChange;
    }
};
bool cmp(Card *a, Card *b)
    {
        return (a->getCost() > b->getCost()); 
    } 
class Player {
    int health;
    int mana;
    int deck;
    int rune;
public:
    Player(int health, int mana, int deck, int rune){
        this->health = health;
        this->mana = mana;
        this->deck = deck;
        this->rune = rune;
    }

    Player* getPlayerCopy(){
        return new Player (health, mana, deck, rune);
    }

    int getMana(){
        return mana;
    }

    int getHealth(){
        return health;
    }

    int takeDamage(int damage){
        health -= damage;
    }
};


int getTradeScore(Card* me, Card* target){
    int dealDamage = min(me->getAttack(), target->getDefense());
    if( target->isWard()){
        dealDamage = 1 + target->getAttack();
    }
    int takeDamage = min(target->getAttack(), me->getDefense());
    if( me->isWard()){
        takeDamage = 1;
    }
    int score = dealDamage - takeDamage;
    if(me->getAttack() >= target->getDefense() || (me->isLethal() && dealDamage > 0)){
        score += target->getDefense() + target->getAttack();
    }
    if(target->getAttack() >= me->getDefense() || (target->isLethal() && takeDamage > 0)){
        score -= me->getDefense() + me->getAttack();
    }
    return score;
}

int executeFight(Card* attacker, Card* target){
    attacker->doAction();
    if(attacker->isLethal()){
        target->takeDamage(999);
    }
    else{
        target->takeDamage(attacker->getAttack());
    }
    if(target->isLethal()){
        attacker->takeDamage(999);
    }
    else{
        attacker->takeDamage(target->getAttack());
    }
}

int LEATHAL_DANGER_VALUE = 7;

double getBoardScore(vector<Card*> myBoardCard, vector<Card*> enemyBoardCard, Player* me, Player* enemy){
    // enemy lethal
    int lethalScore = 0;
    bool hasTaunt = false;
    for(Card* enemyCard: enemyBoardCard){
        if(enemyCard->isGuard()){
            hasTaunt = true;
            break;
        }
    }
    int totalAttack = 0;
    int nextTotalAttack = 0;
    int lifeStealTotal = 0;
    for(Card* card: myBoardCard){
        nextTotalAttack += card->getAttack();
        if(!card->getActionDone()){
            totalAttack += card->getAttack();
        }
    }
    for(Card* card: enemyBoardCard){
        if(card -> isDrain()){
            lifeStealTotal += card->getAttack();
        }
    }
    if(!hasTaunt && totalAttack >= enemy->getHealth() || enemy->getHealth() <= 0){
        lethalScore = 10000;
    }
    else if(!hasTaunt && totalAttack + nextTotalAttack + LEATHAL_DANGER_VALUE >= enemy->getHealth() + lifeStealTotal){
        lethalScore = totalAttack + nextTotalAttack + LEATHAL_DANGER_VALUE - enemy->getHealth() - lifeStealTotal;
    }

    // next turn leathal score


    // min max algorizm
    // enemy card value is max(enemy card, max tradable my card)
    // my card value is max(my card, min tradable enemy card)
    int myCardValueTotal = 0;
    int myDefenceTotal = 0;
    for(Card* myCard: myBoardCard){
        int minTradeScore = -999;
        for(Card* enemyCard: enemyBoardCard){
            int score = getTradeScore(myCard, enemyCard);
            if(minTradeScore > score){
                minTradeScore = score;
            }
        }
        if(myCard->isGuard()){
            myDefenceTotal += myCard->getDefense();
        }
        int basicValue = myCard->calcBasicValue();
        /*if(myCard->isLethal()){
            basicValue += me->getMana();
        }*/
        if(minTradeScore != -999){
            myCardValueTotal += max(basicValue, minTradeScore);
        }
        else{
            myCardValueTotal += basicValue;
        }
    }

    int enemyCardValueTotal = 0;
    int enemyAttackTotal = 0;
    for(Card* enemyCard: enemyBoardCard){
        int maxTradeScore = 0;
        for(Card* myCard: myBoardCard){
            int score = getTradeScore(enemyCard, myCard);
            if(maxTradeScore < score){
                maxTradeScore = score;
            }
        }
        int basicValue = enemyCard->calcBasicValue();
        /*if(enemyCard->isLethal()){
            basicValue += me->getMana();
        }*/
        enemyAttackTotal += enemyCard->getAttack();
        enemyCardValueTotal += max(basicValue, maxTradeScore);
    }
    // avoid enemy leathal
    int leathalPenalty = 0;
    if(enemyAttackTotal >= me->getHealth() + myDefenceTotal - LEATHAL_DANGER_VALUE){
        leathalPenalty = (enemyAttackTotal - myDefenceTotal - me->getHealth() + LEATHAL_DANGER_VALUE + 1) * 100;
    }

    return myCardValueTotal - enemyCardValueTotal - leathalPenalty + lethalScore;
}

int simulateTradeScore(vector<Card*> myBoardCard, vector<Card*> enemyBoardCard, Card* attacker, Card* target, Player* me, Player* enemy){
    vector<Card*> myBoardCardCopy;
    vector<Card*> enemyBoardCardCopy;
    Player* meCopy = me->getPlayerCopy();
    Player* enemyCopy = enemy->getPlayerCopy();
    // make copy board
    for(Card* card: myBoardCard){
        Card* copy = card->getCardCopy();
        if(attacker->getID() == copy->getID()){
            attacker = copy;
        }
        myBoardCardCopy.push_back(copy);
    }
    for(Card* card: enemyBoardCard){
        Card* copy = card->getCardCopy();
        if(target != nullptr && target->getID() == copy->getID()){
            target = copy;
        }
        enemyBoardCardCopy.push_back(copy);
    }
    if(target != nullptr){
        executeFight(attacker, target);
        // trunple
        if(attacker->isBreakThrough()){
            if(target->getDefense() < 0){
                enemyCopy->takeDamage(-target->getDefense());
            }
        }
        if(attacker->isDrain()){
            meCopy->takeDamage(-attacker->getAttack());
        }
    }
    // face
    else {
        attacker->doAction();
        enemy = enemy->getPlayerCopy();
        enemy->takeDamage(attacker->getAttack());
    }
    // jyoukyou kiin syori
    auto itr = myBoardCardCopy.begin();
    while (itr != myBoardCardCopy.end())
    {
        if ((*itr)->getDefense() <= 0)
        {
            itr = myBoardCardCopy.erase(itr);
        }
        else
        {
            itr++;
        }
    }

    itr = enemyBoardCardCopy.begin();
    while (itr != enemyBoardCardCopy.end())
    {
        if ((*itr)->getDefense() <= 0)
        {
            itr = enemyBoardCardCopy.erase(itr);
        }
        else
        {
            itr++;
        }
    }
    return getBoardScore(myBoardCardCopy, enemyBoardCardCopy, me, enemy);
}

class SaikiResult{
public:
    vector<Card*> useCards;
    int score;

    SaikiResult(vector<Card*> useCards, int score){
        this->useCards = useCards;
        this->score = score;
    }
};

class Game {
    vector<Card*> myHandCard;
    vector<Card*> myBoardCard;
    vector<Card*> enemyBoardCard;
    Player* me;
    Player* opponent;
    std::stringstream ss;
    int manaLeft;
public:
    Game(vector<Card*> myHandCard, vector<Card*> myBoardCard, vector<Card*> enemyBoardCard, Player* me, Player* opponent, string ss, int manaLeft){
        this->myHandCard = myHandCard;
        this->myBoardCard = myBoardCard;
        this->enemyBoardCard = enemyBoardCard;
        this->me = me;
        this->opponent = opponent;
        this->ss << ss;
        this->manaLeft = manaLeft;
    }

    Game* simulation(bool goToFace){

        Card* actionCard = nullptr;
        Card* targetCard = nullptr;
        int target = -1;
        int maxScore = -99999999;
        int currentBoardScore = getBoardScore(myBoardCard, enemyBoardCard, me, opponent);

        for(Card* card: myBoardCard){
            // deal yuuri trade
            if(card->getAttack() == 0 || card->getActionDone()){
                continue;
            }
            Card* targetCard = nullptr;
            bool hasTaunt = false;
            // taunt
            for(Card* enemyCard: enemyBoardCard){
                if(enemyCard->isGuard()){
                    hasTaunt = true;
                    //int score = getTradeScore(card, enemyCard);
                    int score = simulateTradeScore(myBoardCard, enemyBoardCard , card, enemyCard, me, opponent);
                    if(score > maxScore){
                        maxScore = score;
                        actionCard = card;
                        target = enemyCard->getID();
                        targetCard = enemyCard;
                    }
                }
            }
            if (!hasTaunt) {
                // calculate face attack value
                int faceScore = simulateTradeScore(myBoardCard, enemyBoardCard , card, nullptr, me, opponent);
                if(faceScore > maxScore){
                    maxScore = faceScore;
                    actionCard = card;
                    target = -1;
                    targetCard = nullptr;
                }

                for(Card* enemyCard: enemyBoardCard){
                    int score = simulateTradeScore(myBoardCard, enemyBoardCard , card, enemyCard, me, opponent);
                    if(score > maxScore){
                        maxScore = score;
                        actionCard = card;
                        target = enemyCard->getID();
                        targetCard = enemyCard;
                    }
                }
            }
        }

        vector<Card*> myHandCardCopy;
        vector<Card*> myBoardCardCopy;
        vector<Card*> enemyBoardCardCopy;
        Player* meCopy;
        Player* opponentCopy;
        std::stringstream ssCopy;
        ssCopy << ss.str(); // copy everything inside ss1's buffer to ss2's buffer

        // make copy board
        for(Card* card: myHandCard){
            myHandCardCopy.push_back(card->getCardCopy());
        }
        for(Card* card: myBoardCard){
            myBoardCardCopy.push_back(card->getCardCopy());
        }
        for(Card* card: enemyBoardCard){
            enemyBoardCardCopy.push_back(card->getCardCopy());
        }
        meCopy = me->getPlayerCopy();
        opponentCopy = opponent->getPlayerCopy();
        
        if(actionCard == nullptr){
            ssCopy << "PASS;";
        }
        else{
            ssCopy << "ATTACK " << actionCard->getID() << " " << target << ";";
            // execute command
            for(Card* card: myBoardCardCopy){
                if(card->getID() == actionCard->getID()){
                    card->doAction();
                    actionCard = card;
                    break;
                }
            }
            if(target == -1){
                opponentCopy->takeDamage(actionCard->getAttack());
            }
            else{
                for(Card* card: enemyBoardCardCopy){
                    if(card->getID() == target){
                        executeFight(actionCard, card);
                        // trunple
                        if(actionCard->isBreakThrough()){
                            if(card->getDefense() < 0){
                                opponentCopy->takeDamage(-card->getDefense());
                            }
                        }
                        break;
                    }
                }
            }
        }
        // jyoukyou kiin syori
        auto itr = myBoardCardCopy.begin();
        while (itr != myBoardCardCopy.end())
        {
            if ((*itr)->getDefense() <= 0)
            {
                itr = myBoardCardCopy.erase(itr);
            }
            else
            {
                itr++;
            }
        }

        itr = enemyBoardCardCopy.begin();
        while (itr != enemyBoardCardCopy.end())
        {
            if ((*itr)->getDefense() <= 0)
            {
                itr = enemyBoardCardCopy.erase(itr);
            }
            else
            {
                itr++;
            }
        }

        return new Game(myHandCardCopy, myBoardCardCopy, enemyBoardCardCopy, meCopy, opponentCopy, ssCopy.str(), manaLeft);
    }

    Game* getCopy(){
        vector<Card*> myHandCardCopy;
        vector<Card*> myBoardCardCopy;
        vector<Card*> enemyBoardCardCopy;
        Player* meCopy;
        Player* opponentCopy;
        std::stringstream ssCopy;
        ssCopy << ss.str(); // copy everything inside ss1's buffer to ss2's buffer

        // make copy board
        for(Card* card: myHandCard){
            myHandCardCopy.push_back(card->getCardCopy());
        }
        for(Card* card: myBoardCard){
            myBoardCardCopy.push_back(card->getCardCopy());
        }
        for(Card* card: enemyBoardCard){
            enemyBoardCardCopy.push_back(card->getCardCopy());
        }
        meCopy = me->getPlayerCopy();
        opponentCopy = opponent->getPlayerCopy();
        return new Game(myHandCardCopy, myBoardCardCopy, enemyBoardCardCopy, meCopy, opponentCopy, ssCopy.str(), manaLeft);
    }


    SaikiResult* saikiPlayCard(int index, vector<Card*> currentPlay, int leftMana, int boardLeft, Game* current){
        SaikiResult* result = new SaikiResult(currentPlay, 0);
        vector<Card*> playCard;
        
        if(myHandCard.size() <= index){
            Game* copyGame = current->getCopy();
            int score = 0;
            int totalDraw = 0;
            for(Card* card: currentPlay){
                totalDraw += card->getDraw();
                // use health only cards
                if(card->getType() == 3 && card->getDefense() == 0){
                    totalDraw += 2;
                }
            }
            // simulate trade
            while(copyGame->getCommands().find("PASS") == std::string::npos){
                copyGame = copyGame->simulation(false);
            }
            result->score = copyGame->getGameBoardScore() - currentPlay.size() + totalDraw;
            return result;
        }
        result = saikiPlayCard(index + 1, currentPlay, leftMana, boardLeft, current);
        if(boardLeft == 0 && myHandCard[index]->getType() == 0){
            return result;
        }

        if(myHandCard[index]->getCost() <= leftMana){
            if(myHandCard[index]->getType() == 0){
                boardLeft --;   
            }
            vector<Card*> newPlay;
            SaikiResult* tmpResult;
            int tmpScore = 0;
            for(Card* card: currentPlay){
                newPlay.push_back(card);
            }
            Card* playCardCopy = myHandCard[index]->getCardCopy();
            newPlay.push_back(playCardCopy);
            if(playCardCopy->getType() == 0){
                boardLeft --;
            }

            Game* newBoard = current->getCopy();
            newBoard->setBestCardPlay(playCardCopy, newBoard->getGameBoardScore());
            newBoard->playCard(playCardCopy);

            tmpResult = saikiPlayCard(index + 1, newPlay, leftMana - myHandCard[index]->getCost(), boardLeft, newBoard);
            if(tmpResult->score > result->score){
                result = tmpResult;
            }
        }
        
        return result;
    }

    int getGameBoardScore(){
        return getBoardScore(myBoardCard, enemyBoardCard, me, opponent);
    }

    vector<Card*> calcBestPlayCards(bool goToFace){
        vector<Card*> firstCards;
        return saikiPlayCard(0, firstCards, manaLeft, 6 - myBoardCard.size(), this)->useCards;
    }

    void setBestCardPlay(Card* hand, int currentBoardScore){
        // health change
        Player* copyMe = me->getPlayerCopy();
        Player* copyOpponent = opponent->getPlayerCopy();
        copyMe->takeDamage(-hand->getMyHealthChange());
        copyOpponent->takeDamage(-hand->getOpponentHealthChange());
        
        if(hand->getType() == 1){
            if(myBoardCard.size() != 0){
                int maxScore = -99999;
                for(Card* targetCreature : myBoardCard){
                    vector<Card*> myBoardCardCopy;
                    for(Card* card: myBoardCard){
                        if(targetCreature->getID() == card->getID()){
                            Card* buffedCard = card->getCardCopy();
                            buffedCard->buff(hand);
                            myBoardCardCopy.push_back(buffedCard);
                        }
                        else{
                            myBoardCardCopy.push_back(card);
                        }
                    }
                    Game* current = new Game(myHandCard, myBoardCardCopy, enemyBoardCard, copyMe, copyOpponent, "", manaLeft);
                    while(current->getCommands().find("PASS") == std::string::npos){
                        current = current->simulation(false);
                    }
                    int score = current->getGameBoardScore() - currentBoardScore - 1 + hand->getDraw();
                    if(score > maxScore){
                        maxScore = score;
                        hand->setPlayScore(score);
                        std::stringstream ss;
                        ss << "USE " << hand->getID() << " " << targetCreature->getID() << ";";
                        hand->setCommand(ss.str());
                    }
                }
            }
            else{
                hand->setPlayScore(-999);
            }
        }
        else if(hand->getType() == 2){
            if(enemyBoardCard.size() != 0){
                int maxScore = -99999;
                for(Card* targetCreature : enemyBoardCard){
                    vector<Card*> enemyBoardCardCopy;
                    for(Card* card: enemyBoardCard){
                        if(targetCreature->getID() == card->getID()){
                            Card* buffedCard = card->getCardCopy();
                            buffedCard->deBuff(hand);
                            enemyBoardCardCopy.push_back(buffedCard);
                            // jyoukyou kiin syori
                            auto itr = enemyBoardCardCopy.begin();
                            itr = enemyBoardCardCopy.begin();
                            while (itr != enemyBoardCardCopy.end())
                            {
                                if ((*itr)->getDefense() <= 0)
                                {
                                    itr = enemyBoardCardCopy.erase(itr);
                                }
                                else
                                {
                                    itr++;
                                }
                            }
                        }
                        else{
                            enemyBoardCardCopy.push_back(card);
                        }
                    }
                    Game* current = new Game(myHandCard, myBoardCard, enemyBoardCardCopy, copyMe, copyOpponent, "", manaLeft);
                    while(current->getCommands().find("PASS") == std::string::npos){
                        current = current->simulation(false);
                    }
                    int score = current->getGameBoardScore() - currentBoardScore - 1 + hand->getDraw();
                    if(score > maxScore){
                        maxScore = score;
                        hand->setPlayScore(score);
                        std::stringstream ss;
                        ss << "USE " << hand->getID() << " " << targetCreature->getID() << ";";
                        hand->setCommand(ss.str());
                    }
                }
            }
            else{
                hand->setPlayScore(-999);
            }
        }
        else if(hand->getType() == 3){
            if(hand->getDefense() < 0){
                int maxScore = -99999;
                // target player score
                Player* damagedOpponent = copyOpponent->getPlayerCopy();
                damagedOpponent->takeDamage(-hand->getDefense());
                maxScore = getBoardScore(myBoardCard, enemyBoardCard, copyMe, damagedOpponent) - currentBoardScore - 1 + hand->getDraw();
                hand->setPlayScore(maxScore);
                std::stringstream ss;
                ss << "USE " << hand->getID() << " " << "-1" << ";";
                hand->setCommand(ss.str());

                for(Card* targetCreature : enemyBoardCard){
                    vector<Card*> enemyBoardCardCopy;
                    for(Card* card: enemyBoardCard){
                        if(targetCreature->getID() == card->getID()){
                            Card* buffedCard = card->getCardCopy();
                            buffedCard->deBuff(hand);
                            enemyBoardCardCopy.push_back(buffedCard);
                            // jyoukyou kiin syori
                            auto itr = enemyBoardCardCopy.begin();
                            itr = enemyBoardCardCopy.begin();
                            while (itr != enemyBoardCardCopy.end())
                            {
                                if ((*itr)->getDefense() <= 0)
                                {
                                    itr = enemyBoardCardCopy.erase(itr);
                                }
                                else
                                {
                                    itr++;
                                }
                            }
                        }
                        else{
                            enemyBoardCardCopy.push_back(card);
                        }
                    }
                    Game* current = new Game(myHandCard, myBoardCard, enemyBoardCardCopy, copyMe, copyOpponent, "", manaLeft);
                    while(current->getCommands().find("PASS") == std::string::npos){
                        current = current->simulation(false);
                    }
                    int score = current->getGameBoardScore() - currentBoardScore - 1 + hand->getDraw();
                    if(score > maxScore){
                        maxScore = score;
                        hand->setPlayScore(score);
                        std::stringstream ss;
                        ss << "USE " << hand->getID() << " " << targetCreature->getID() << ";";
                        hand->setCommand(ss.str());
                    }
                }
            }
            else{
                int score = getBoardScore(myBoardCard, enemyBoardCard, copyMe, copyOpponent) - currentBoardScore + 1 + hand->getDraw();
                hand->setPlayScore(score);
                std::stringstream ss;
                ss << "USE " << hand->getID() << " -1" << ";";
                hand->setCommand(ss.str());
            }
        }
        else if(hand->getType() == 0){
            if(hand->isCharge() && myBoardCard.size() < 6){
                vector<Card*> myBoardCardCopy;
                for(Card* card: myBoardCard){
                    myBoardCardCopy.push_back(card);
                }
                myBoardCardCopy.push_back(hand);

                Game* current = new Game(myHandCard, myBoardCardCopy, enemyBoardCard, copyMe, copyOpponent, "", manaLeft);
                while(current->getCommands().find("PASS") == std::string::npos){
                    current = current->simulation(false);
                }
                int score = current->getGameBoardScore() - currentBoardScore - 1 + hand->getDraw();
                hand->setPlayScore(score);
                std::stringstream ss;
                ss << "SUMMON " << hand->getID() << ";";
                hand->setCommand(ss.str());
            }
            else if(myBoardCard.size() < 6){
                vector<Card*> myBoardCardCopy;
                for(Card* card: myBoardCard){
                    myBoardCardCopy.push_back(card);
                }
                myBoardCardCopy.push_back(hand);
                hand->setPlayScore(getBoardScore(myBoardCardCopy, enemyBoardCard, copyMe, copyOpponent) - currentBoardScore);
                std::stringstream ss;
                ss << "SUMMON " << hand->getID() << ";";
                hand->setCommand(ss.str());
            }
            else{
                hand->setPlayScore(-999);
            }
        }
        cerr << hand->getID() << "playValueBefore=" << hand->getPlayScore() << endl;
    }

    void playCard(Card* card){
        // remove from hand
        auto itr = myHandCard.begin();
        while (itr != myHandCard.end())
        {
            if ((*itr)->getID() == card->getID())
            {
                itr = myHandCard.erase(itr);
                break;
            }
            else
            {
                itr++;
            }
        }

        // health change
        me->takeDamage(-card->getMyHealthChange());
        opponent->takeDamage(-card->getOpponentHealthChange());

        if(card->getType() == 0 && myBoardCard.size() < 6){
            ss << "SUMMON " << card->getID() << ";";
            addMyCardToBoard(card);
            // charge
            if(!card->isCharge()){
                card->doAction();
            }
            manaLeft -= card->getCost();
        }
        else if(card->getType() == 1 && card->getCommand() != ""){
            ss << card->getCommand();
            int targetID = std::atoi(split(card->getCommand(), ' ')[2].c_str());
            for(Card* target: myBoardCard){
                if(targetID == target->getID()){
                    target->buff(card);
                    break;
                }
            }
            manaLeft -= card->getCost();
        }
        else if(card->getType() == 2 && card->getCommand() != ""){
            ss << card->getCommand();
            int targetID = std::atoi(split(card->getCommand(), ' ')[2].c_str());
            for(Card* target: enemyBoardCard){
                if(targetID == target->getID()){
                    target->deBuff(card);
                    // jyoukyou kiin syori
                    auto itr = enemyBoardCard.begin();
                    itr = enemyBoardCard.begin();
                    while (itr != enemyBoardCard.end())
                    {
                        if ((*itr)->getDefense() <= 0)
                        {
                            itr = enemyBoardCard.erase(itr);
                        }
                        else
                        {
                            itr++;
                        }
                    }
                    break;
                }
            }
            manaLeft -= card->getCost();
        }
        else if(card->getType() == 3 && card->getCommand() != ""){
            ss << card->getCommand();
            int targetID = std::atoi(split(card->getCommand(), ' ')[2].c_str());
            if(targetID != -1){
                for(Card* target: enemyBoardCard){
                    if(targetID == target->getID()){
                        target->deBuff(card);
                        // jyoukyou kiin syori
                        auto itr = enemyBoardCard.begin();
                        itr = enemyBoardCard.begin();
                        while (itr != enemyBoardCard.end())
                        {
                            if ((*itr)->getDefense() <= 0)
                            {
                                itr = enemyBoardCard.erase(itr);
                            }
                            else
                            {
                                itr++;
                            }
                        }
                        break;
                    }
                }
            }
            manaLeft -= card->getCost();
        }
    }

    string getCommands(){
        return ss.str();
    }

    void addMyCardToBoard(Card* card){
        myBoardCard.push_back(card);
    }

    void addString(string str){
       ss << str;
    }
};

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/

// mana curve
int* manaCurve = new int[13];
//int idealManaCurve[13] = {2, 6, 4, 6, 3, 3, 2, 2, 1, 1, 1, 1, 1};

// by statistics
int idealManaCurve[13] = {1, 4, 3, 6, 4, 4, 3, 4, 1, 2, 0, 0, 2};

// card cost counts
int catdCostCounts[13] = {7,12,33,28,30,18,15,7,5,3,0,0,2};

int* currentDeck = new int[160];
int pickedCard = 0;
int HIDDEN_SIZE = 100;
double*** w;
double** b;

double ** relu(double** input, int r, int c){
    for(int i = 0; i < r; i ++){
        for(int n = 0; n < c; n ++){
            if(input[i][n] < 0){
                input[i][n] = 0;
            }
        }
    }
    return input;
}

double ** sigmoid(double** input, int r, int c){
    for(int i = 0; i < r; i ++){
        for(int n = 0; n < c; n ++){
            input[i][n] = 1/(1 + exp(-input[i][n]));
        }
    }
    return input;
}

double ** tanh(double** input, int r, int c){
    for(int i = 0; i < r; i ++){
        for(int n = 0; n < c; n ++){
            input[i][n] = 1 - exp(-2 * input[i][n]) / (1 + exp(-2 * input[i][n]));
        }
    }
    return input;
}

string activation;

double ** applyActivation(double** input, int r, int c){
    cerr << activation << endl;
    if(activation == "\"relu\""){
        return relu(input, r, c);
    }
    if(activation == "\"tanh\""){
        return tanh(input, r, c);
    }
    if(activation == "\"sigmoid\""){
        return sigmoid(input, r, c);
    }
    //exit(1);
}

double** getOneHot(){
    double ** result = new double* [640];
    for(int i = 0; i < 640; i++){
        result[i] = new double[1];
        result[i][0] = 0;
    }
    for(int i = 0; i < 160; i++){
        if(currentDeck[i] == 0){
            result[i * 4][0] = 1;
        }
        if(currentDeck[i] == 1){
            result[i * 4 + 1][0] = 1;
        }
        if(currentDeck[i] == 2){
            result[i * 4 + 2][0] = 1;
        }
        if(currentDeck[i] >= 3){
            result[i * 4 + 3][0] = 1;
        }
    }
    return result;
}

double** calcNN(){
    double** inputVector = getOneHot();

    // calc matrix 1
    cerr << "layer1" << endl;
    double** x1 = mulMatrix(w[0], inputVector, 100, 640, 640, 1);
    cerr << "endmul" << endl;
    for(int i = 0; i < HIDDEN_SIZE; i ++){
        x1[i][0] += b[0][i];
    }
    applyActivation(x1, HIDDEN_SIZE, 1);
    // calc matrix 2
    cerr << "layer2" << endl;
    double** x2 = mulMatrix(w[1], x1, 100, 100, 100, 1);
    for(int i = 0; i < HIDDEN_SIZE; i ++){
        x2[i][0] += b[1][i];
    }
    applyActivation(x2, HIDDEN_SIZE, 1);
    // calc matrix 3
    cerr << "layer3" << endl;
    double** x3 = mulMatrix(w[2], x2, 160, 100, 100, 1);
    for(int i = 0; i < 160; i ++){
        x3[i][0] += b[2][i];
    }
    applyActivation(x3, 160, 1);
    return x3;
}

string modelDir;
string logFileName;
string logDirName;
string confDir;

#ifdef READ_EPOCH_FILE
void readModel(){
    std::string str;

    // read activation
    std::ifstream ifs(confDir);
    if (ifs.fail()){
        std::cerr << "失敗" << std::endl;
        return;
    }
    while (getline(ifs, str)){
        if(str.substr(0, 10) == "activation"){
            activation = str.substr(13);
        }
    }

    // load bias
    b = new double*[3];
    for(int n = 0; n < 3; n ++){
        cerr << "read bias file" << n << endl;
        string fileName;
        int size;
        if(n == 0){
            fileName = modelDir + "/Inference-Forward1-biases.csv";
            size = 100;
        } else if(n == 1){
            fileName = modelDir + "/Inference-Forward2-biases.csv";
            size = 100;
        } else if(n == 2){
            fileName = modelDir + "/Inference-Output-biases.csv";
            size = 160;
        }
        std::ifstream ifs(fileName);
        if (ifs.fail()){
            std::cerr << "失敗" << std::endl;
            return;
        }
        b[n] = new double[size];
        int i = 0;
        while (getline(ifs, str)){
            b[n][i] = std::atof(str.c_str());
            i ++;
        }
    }

    // load weight
    w = new double**[3];
    for(int n = 0; n < 3; n ++){
        cerr << "read weight file" << n << endl;
        string fileName;
        int size1, size2;
        if(n == 0){
            fileName = modelDir  + "/Inference-Forward1-weights.csv";
            size1 = 100;
            size2 = 640;
        } else if(n == 1){
            fileName = modelDir  + "/Inference-Forward2-weights.csv";
            size1 = 100;
            size2 = 100;
        } else if(n == 2){
            fileName = modelDir  + "/Inference-Output-weights.csv";
            size1 = 160;
            size2 = 100;
        }
        std::ifstream ifs(fileName);
        if (ifs.fail()){
            std::cerr << "失敗" << std::endl;
            return;
        }
        w[n] = new double*[size1];
        for(int i = 0; i < size1; i ++){
            w[n][i] = new double[size2];
        }
        int i = 0;
        while (getline(ifs, str)){
            vector<string> result = split(str, ' ');
            for(int j = 0; j < size1; j ++){
                w[n][j][i] = std::atof(result[j].c_str());
            }
            i ++;
        }
    }
    cerr << "read weight done" << endl;
}
#endif



int main(int argc,char *argv[])
{

#ifdef DEBUG
    logDirName = argv[1];
#endif

#ifdef READ_EPOCH_FILE
    modelDir = argv[2];
    confDir = argv[3];
    readModel();
    for(int i = 0; i < 160; i ++){
        currentDeck[i] = 1;
    }
    double** nn = calcNN();
    for(int i = 0; i < 160; i ++){
        cerr << nn[i][0] << endl;
    }
#endif

    for(int i = 0; i < 160; i ++){
        currentDeck[i] = 0;
    }
    for(int i = 0; i < 13; i ++){
        manaCurve[i] = 0;
    }
    // game loop
    while (1) {
        vector<Card*> myHandCard;
        vector<Card*> myBoardCard;
        vector<Card*> enemyBoardCard;
        Player* me;
        Player* opponent;
        for (int i = 0; i < 2; i++) {
            int playerHealth;
            int playerMana;
            int playerDeck;
            int playerRune;
            cin >> playerHealth >> playerMana >> playerDeck >> playerRune; cin.ignore();
            if(i == 0){
                me = new Player(playerHealth, playerMana, playerDeck, playerRune);
            }
            else {
                opponent = new Player(playerHealth, playerMana, playerDeck, playerRune);
            }
        }
        int opponentHand;
        cin >> opponentHand; cin.ignore();
        int cardCount;
        cin >> cardCount; cin.ignore();
        for (int i = 0; i < cardCount; i++) {
            int cardNumber;
            int instanceId;
            int location;
            int cardType;
            int cost;
            int attack;
            int defense;
            string abilities;
            int myHealthChange;
            int opponentHealthChange;
            int cardDraw;
            cin >> cardNumber >> instanceId >> location >> cardType >> cost >> attack >> defense >> abilities >> myHealthChange >> opponentHealthChange >> cardDraw; cin.ignore();
            Card* card = new Card(cardNumber, instanceId, location, cardType, cost, attack, defense, abilities, myHealthChange, opponentHealthChange, cardDraw, false);
            if(location == 0){
                myHandCard.push_back(card);
            }
            else if (location == 1){
                myBoardCard.push_back(card);
            }
            else if(location == -1){
                enemyBoardCard.push_back(card);
            }
        }

        std::chrono::system_clock::time_point  start, end; // 型は auto で可
        start = std::chrono::system_clock::now(); // 計測開始時間

        // while draft phase
        Card* lowestCard = nullptr;
        int lowestIndex = -1;
        int maxScore = -9999999;
        int i = 0;
        double expectedManaCurve[13] = {0,0,0,0,0,0,0,0,0,0,0,0,0};
        for(int n = 0; n < 13; n ++){
            expectedManaCurve[n] = manaCurve[n] + (30 - pickedCard - 1) * ((double)catdCostCounts[n] / 160);
            cerr << "mana" << n << ":" << manaCurve[n] << endl;
            cerr << "expedtedmana" << n << ":" << expectedManaCurve[n] << endl;
        }


        // think about mana curve
        if(me->getMana() == 0){
            for(Card* card: myHandCard){
                double score = idealManaCurve[card->getCost()] - expectedManaCurve[card->getCost()];
#ifndef TEKAGEN
                score += cardValues[card->getCardNumber() - 1] * 200;
#endif
#ifdef READ_EPOCH_FILE
                score = calcNN()[card->getCardNumber() - 1][0];
#endif
                /*if(card->getType() != 0){
                    score = -9999;
                }*/
                if(lowestCard == nullptr || maxScore < score){
                    lowestCard = card;
                    lowestIndex = i;
                    maxScore = score;
                }
                i ++;
            }
#ifdef RANDOM_PICK
            std::random_device rd;
            std::mt19937 mt(rd());
            std::uniform_int_distribution<int> dice(0,10);
            int rnd = dice(mt);
            if(rnd <= 2){
                lowestIndex = rnd;
                lowestCard = myHandCard[lowestIndex];
            }
#endif

            cerr << lowestIndex << endl;
            cout << "PICK " << lowestIndex << endl;

#ifdef DEBUG
            ofstream outputfile(logDirName, ios::app);
            for(int i = 0; i < 160; i++){
                outputfile << currentDeck[i] << " ";
            }
            outputfile << endl;
            for(Card* card: myHandCard){
                outputfile << card->getCardNumber() - 1 << " ";
            }
            outputfile << endl;
            outputfile << lowestCard->getCardNumber() - 1 << endl;
            outputfile.close();
#endif DEBUG

            currentDeck[lowestCard->getCardNumber() - 1]++;
            manaCurve[lowestCard->getCost()]++;
            pickedCard ++;
        }
        else{
            // judge leathal
            bool goToFace = false;
            int totalAttack = 0;
            int totalDefence = 0;
            for(Card* card: myBoardCard){
                totalAttack += card->getAttack();
            }
            for(Card* card: enemyBoardCard){
                totalDefence += card->getDefense();
            }
            if(totalAttack * 2 > opponent->getHealth() + totalDefence){
                goToFace = true;
            }
            
            // battle phase
            cerr << goToFace << endl;
            Game* current = new Game(myHandCard, myBoardCard, enemyBoardCard, me, opponent, "", me->getMana());
            
            // make play card score 
            vector<Card*> beforePlayCards = current->calcBestPlayCards(goToFace);
            for(Card* card: beforePlayCards){
                cerr << "beforePlay:" << card->getCommand() << endl;
                current->playCard(card);
            }

            while(current->getCommands().find("PASS") == std::string::npos){
                current = current->simulation(goToFace);
                cerr << current->getCommands() << endl;
            }

            int passIndex = current->getCommands().find("PASS");
            
            vector<Card*> playCards;

            // play cards untill no more want to play
            playCards = current->calcBestPlayCards(false);
            while(playCards.size() > 0){
                for(Card* card: playCards){
                    current -> playCard(card);
                }
                playCards = current->calcBestPlayCards(false);
            }

            // use chage cards
            current = current->simulation(goToFace);
            while(current->getCommands().find("PASS", passIndex + 1) == std::string::npos){
                current = current->simulation(goToFace);
                cerr << current->getCommands() << endl;
            }

            end = std::chrono::system_clock::now();  // 計測終了時間
            double elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count(); //処理に要した時間をミリ秒に変換

            cout << current->getCommands() << "PASS " << elapsed << "ms;" << endl;
        }

        // Write an action using cout. DON'T FORGET THE "<< endl"
        // To debug: cerr << "Debug messages..." << endl;

       
    }
}