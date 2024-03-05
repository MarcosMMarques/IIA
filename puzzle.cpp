#include <bits/stdc++.h>
#include <set>
#include <queue>
#include <vector>
#include <chrono>
using namespace std;

class Node{
    public:
        vector<vector<int>> matrix;
        char last_mov;
        pair<int, int> blank;

    bool operator<(const Node& other) const {
        for (size_t i = 0; i < matrix.size(); ++i) {
            for (size_t j = 0; j < matrix[i].size(); ++j) {
                if (matrix[i][j] != other.matrix[i][j])
                    return matrix[i][j] < other.matrix[i][j];
            }
        }
        return false;
    }
};


bool validateMatrix(Node node){
    vector<int> aux;
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            aux.push_back(node.matrix[i][j]);
        }
    }
    if(aux.back() != 0) return false;
    aux.pop_back();
    for(int i = 0; i < aux.size() - 1; i++){
        if(aux[i] > aux[i + 1]) return false;
    }
    return true;
}

vector<pair<pair<int,int>, char>> generate_actions(Node& node){
    vector<pair<pair<int,int>, char>> actions;
    pair<int, int> aux1;
    pair<pair<int, int>, char> aux2;
    if((node.blank.first - 1) >= 0 && node.last_mov != 'u'){
        aux1.first = node.blank.first - 1; aux1.second = node.blank.second;
        aux2.first = aux1; aux2.second = 'u';
        actions.push_back(aux2);
    }
    if((node.blank.first + 1) < 3 && node.last_mov != 'd'){
        aux1.first = node.blank.first + 1; aux1.second = node.blank.second;
        aux2.first = aux1; aux2.second = 'd';
        actions.push_back(aux2);
    }
    if((node.blank.second - 1) >= 0 && node.last_mov != 'l'){
        aux1.first = node.blank.first; aux1.second = node.blank.second - 1;
        aux2.first = aux1; aux2.second = 'l';
        actions.push_back(aux2);
    }
    if((node.blank.second + 1) < 3 && node.last_mov != 'r'){
        aux1.first = node.blank.first; aux1.second = node.blank.second + 1;
        aux2.first = aux1; aux2.second = 'r';
        actions.push_back(aux2);
    }
    return actions;
}

vector<Node> generate_nodes(vector<pair<pair<int,int>, char>>& possible_actions, Node& node){

    vector<Node> new_nodes;
    for(auto p : possible_actions){
        Node new_n = node;
        new_n.last_mov = p.second;
        new_n.matrix[node.blank.first][node.blank.second] = new_n.matrix[p.first.first][p.first.second];
        new_n.matrix[p.first.first][p.first.second] = 0; 
        new_n.blank.first = p.first.first; new_n.blank.second = p.first.second;
        new_nodes.push_back(new_n); 
    }
    return new_nodes;
}

string matrix2string(vector<vector<int>>& matrix){
    string aux = "";
    for(int i = 0; i < matrix.size(); i++){
        for(int j = 0; j < matrix[i].size(); j++){
            aux += to_string(matrix[i][j]);
        }
    }
    // cout << "String: " << aux << "\n";
    return aux;
}

Node search(Node node){
    queue<Node> frontier; frontier.push(node);
    set<Node> explored_set;
    
    while(!frontier.empty()){
        if(explored_set.count(frontier.front()) == 0){  
            if(validateMatrix(frontier.front())) return frontier.front();
            vector<pair<pair<int,int>, char>> possible_actions = generate_actions(frontier.front());  
            vector<Node> new_nodes = generate_nodes(possible_actions, frontier.front());
            explored_set.insert(frontier.front());
            for(auto no : new_nodes) frontier.push(no);
            possible_actions.clear();
            new_nodes.clear();
        }
        frontier.pop();
        // cout << explored_set.size() << "\n";
    }
    return node;
}

int countInversions(const vector<int>& puzzle) {
    int inversions = 0;
    for (int i = 0; i < puzzle.size() - 1; i++) {
        for (int j = i + 1; j < puzzle.size(); j++) {
            if (puzzle[i] > puzzle[j] && puzzle[i] != 0 && puzzle[j] != 0) {
                inversions++;
            }
        }
    }
    return inversions;
}

int main(){
    ios::sync_with_stdio(false); cin.tie(0);
    auto start = std::chrono::high_resolution_clock::now();
    vector<vector<int>> matrix;
    vector<int> puzzle;
    Node node; 
    int aux;

    for(int i = 0; i < 3; i++){
        vector<int> aux_v;
        for(int j = 0; j < 3; j++){
            cin >> aux; 
            aux_v.push_back(aux);
            if(aux != 0) puzzle.push_back(aux);
            else {
                node.blank.first = i;
                node.blank.second = j;
            }
        }    
        matrix.push_back(aux_v);
    }
    
    if (countInversions(puzzle) % 2 == 0) {
        cout << "O puzzle é solúvel.\n";
    } else {
        cout << "O puzzle não é solúvel.\n";
    }

    node.matrix = matrix; node.last_mov = 'a';
    node = search(node);

    for(int i = 0; i < node.matrix.size(); i++){
        for(int j = 0; j < node.matrix[i].size(); j++){
            cout << node.matrix[i][j] << " ";
        }
        cout << "\n";    
    }


    //Calculo de tempo de execução
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    double duration_ms = duration.count();
    std::cout << "Tempo de execução: " << duration_ms << " segundos" << std::endl;


    return 0;
}