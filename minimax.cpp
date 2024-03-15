#include <iostream>
#include <vector>
#include <climits>

using namespace std;

enum tableSymbols {NO_WINNER, NO_MOVES, EMPTY, O, X};

int currentRound = 0;

int cost(vector<vector<tableSymbols>>& table) {
    int winnerPlayer = winner(table);
    
    if (winnerPlayer == X) return 1;
    else if (winnerPlayer == O) return -1;
    else return 0;
}


int minimax(vector<vector<tableSymbols>>& table, tableSymbols maximizingPlayer) {
    if (winner(table) != NO_WINNER) return cost(table);
    
    vector<vector<tableSymbols>> resultTable;

    if (maximizingPlayer == X) {
        int bestScore = INT_MIN;
        for (auto action : actions(table)) {
            resultTable = newTable(table, action);
            int currentScore = minimax(resultTable, O);
            bestScore = max(bestScore, currentScore);
        }
        return bestScore;
    } else {
        int bestScore = INT_MAX;
        for (auto action : actions(table)) {
            resultTable = newTable(table, action);
            int currentScore = minimax(resultTable, X);
            bestScore = min(bestScore, currentScore);
        }
        return bestScore;
    }
}

pair<int, int> betterMove(vector<vector<tableSymbols>>& table) {
    int bestScore = INT_MIN;

    pair<int, int> position;

    for(auto action : actions(table)){
        vector<vector<tableSymbols>> resultTable = newTable(table, action);
        int score = minimax(resultTable, X);
        
        if(score > bestScore){
            bestScore = score;
            position = action;
        }
    }

    return position;
}

void printGame(vector<vector<tableSymbols>>& table) {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (table[i][j] == X)
                cout << "X ";
            else if (table[i][j] == O)
                cout << "O ";
            else
                cout << "  ";

            if(j < 2) cout << "|";
        }
        cout << "\n---------\n";
    }
}

tableSymbols currentlyPlayer() {
    if(currentRound % 2 == 0) return X;
    return O;
}

vector<pair<int, int>> actions(vector<vector<tableSymbols>>& table) {
    
    vector<pair<int, int>> positions;

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (table[i][j] == EMPTY) {
                positions.push_back({i, j});
            }
        }
    }
    return positions;
}

vector<vector<tableSymbols>> newTable(vector<vector<tableSymbols>> table, pair<int, int> position) {
    table[position.first][position.second] = currentlyPlayer();
    return table;
}

tableSymbols winner(vector<vector<tableSymbols>>& table) {
    if (table[0][0] != EMPTY && table[0][0] == table[1][1] && table[1][1] == table[2][2])
        return table[0][0];
    if (table[0][2] != EMPTY && table[0][2] == table[1][1] && table[1][1] == table[2][0])
        return table[0][2];

    for (int i = 0; i < 3; ++i) {
        if (table[i][0] != EMPTY && table[i][0] == table[i][1] && table[i][1] == table[i][2])
            return table[i][0];
        if (table[0][i] != EMPTY && table[0][i] == table[1][i] && table[1][i] == table[2][i])
            return table[0][i];
    }
    

    if(actions(table).empty()) return NO_MOVES;

    return NO_WINNER;
}

int main() {

    cout << "--------------------\n";
    cout << "| TIC TAC TOE GAME |\n";
    cout << "-------------------- \n" << endl;

    vector<vector<tableSymbols>> table = {
        {EMPTY, EMPTY, EMPTY},
        {EMPTY, EMPTY, EMPTY},
        {EMPTY, EMPTY, EMPTY}
    };

    int winnerPlayer = EMPTY;

    while(winner(table) == NO_WINNER){
        tableSymbols currentPlayer = currentlyPlayer();
        
        if(currentPlayer == O){
            pair<int,int> movePosition;
            bool validation;

            do{
                validation = false;
                cout << "\nDigite a linha e coluna:\n";
                cin >> movePosition.first >> movePosition.second;
            
                if(table[movePosition.first][movePosition.second] != EMPTY || 
                   movePosition.first < 0 || movePosition.first > 2 || 
                   movePosition.second < 0 || movePosition.second > 2){
                
                    cout << "!!! Movimento Inválido !!!\n";
                    validation = true;
                }
            }while(validation);

            table[movePosition.first][movePosition.second] = O;
        }else{
            pair<int, int> movePosition = betterMove(table);
            table[movePosition.first][movePosition.second] = X;
        }

        currentRound++;
        printGame(table);
    }

    winnerPlayer = winner(table);
    if (winnerPlayer == X)
        cout << "\n Você perdeu!\n";
    else if (winnerPlayer == O)
        cout << "\n Você ganhou!!!\n";
    else
        cout << "\n Empate :/ \n";

    return 0;
}