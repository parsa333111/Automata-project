#include <bits/stdc++.h>

#define F first
#define S second
#define PB push_back
#define MP make_pair

using namespace std;

int number_of_states; // Global variable to keep track of number of states

// Struct representing an NFA
struct NFA {
    vector<string>states; // Vector of strings representing the states of the NFA
    string final_state, start_state; // Strings representing the final and start state of the NFA
    map<string, vector<pair<string, string>>> adj;// Map store transitions from a state to a vector of pairs of strings alphabets and next states
};

// Struct representing an NFA or a character in the standard input
struct NFAorCHAR {
    NFA NFA;// An NFA
    char ch = ' '; // A character
    bool is_ch; // Boolean indicating whether this struct contains an NFA or a character
};

// Function to perform union operation on two NFAs
NFA ejtema(NFA A, NFA B) {
    NFA ans;
    for(int i = 0 ; i < A.states.size() ; i++ ) {// Copying over states from A
        ans.states.push_back(A.states[i]);
    }
    for(int i = 0 ; i < B.states.size() ; i++ ) {// Copying over states from B
        ans.states.push_back(B.states[i]);
    }
    for(auto u: A.adj) { // Copying over transitions from A
        ans.adj[u.F] = u.S;
    }
    for(auto u: B.adj) { // Copying over transitions from B
        ans.adj[u.F] = u.S;
    }
    // Adding a new start and final state to the NFA
    ans.start_state = "Q" + to_string(number_of_states);
    ans.final_state = "Q" + to_string(number_of_states+1);
    number_of_states += 2;// Updating the number of states
    vector<pair<string, string>>tmp;
    tmp.PB(MP("λ", A.start_state));
    tmp.PB(MP("λ", B.start_state));
    ans.adj[ans.start_state] = tmp;
    tmp.clear();
    ans.adj[A.final_state].PB(MP("λ", ans.final_state));
    ans.adj[B.final_state].PB(MP("λ", ans.final_state));
    ans.states.push_back(ans.start_state);
    ans.states.push_back(ans.final_state);
    return ans;
}

// Function to perform concatenation operation on two NFAs
NFA concate(NFA A, NFA B) {
    NFA ans;
    for(int i = 0 ; i < A.states.size() ; i++ ) {// Copying over states from A
        ans.states.push_back(A.states[i]);
    }
    for(int i = 0 ; i < B.states.size() ; i++ ) {// Copying over states from B
        ans.states.push_back(B.states[i]);
    }
    for(auto u: A.adj) {// Copying over transitions from A
        ans.adj[u.F] = u.S;
    }
    for(auto u: B.adj) {// Copying over transitions from B
        ans.adj[u.F] = u.S;
    }
    ans.adj[A.final_state].PB(MP("λ", B.start_state));
    ans.final_state = B.final_state;
    ans.start_state = A.start_state;
    return ans;
}

// Function to perform star operation on an NFA
NFA star(NFA A) {
    NFA ans;
    for(int i = 0 ; i < A.states.size() ; i++ ) {// Copying over states from A
        ans.states.push_back(A.states[i]);
    }
    for(auto u: A.adj) { // Copying over transitions from A
        ans.adj[u.F] = u.S;
    }
    // Adding a new start and final state to the NFA
    ans.start_state = "Q" + to_string(number_of_states);
    ans.final_state = "Q" + to_string(number_of_states+1);
    number_of_states += 2;// Updating the global number of states variable
    ans.states.push_back(ans.start_state);
    ans.states.push_back(ans.final_state);
    ans.adj[ans.start_state].PB(MP("λ", ans.final_state));
    ans.adj[A.final_state].PB(MP("λ", A.start_state));
    ans.adj[ans.start_state].PB(MP("λ", A.start_state));
    ans.adj[A.final_state].PB(MP("λ", ans.final_state));
    return ans;
}

vector<string>alphabets;

//function is responsible for building simple NFAs such as NFAs with single character acceptors or landa acceptors
NFA make_simple_NFA(string c) {
    NFA ans;
    ans.start_state = "Q" + to_string(number_of_states);
    ans.final_state = "Q" + to_string(number_of_states+1);
    number_of_states += 2;
    ans.states.push_back(ans.start_state);
    ans.states.push_back(ans.final_state);
    ans.adj[ans.start_state].push_back(MP(c, ans.final_state));
    return ans;
}

//function puts the output in the requested file
void print(NFA x) {
    ofstream myfile ("NFA_Output _2.txt");
    if (!myfile.is_open()) {
        cout << "Unable to open file";
    }
    else {
        for(auto u: alphabets)myfile << u << ' ';
        myfile << '\n';
        for(auto u: x.states) {
            myfile << u << ' ';
        }
        myfile << '\n';
        myfile << x.start_state << '\n';
        myfile << x.final_state << '\n';
        for(auto u: x.adj) {
            for(auto u2: u.S) {
                myfile << u.F << ' ' << u2.F << ' ' << u2.S << '\n';
            }
        }
    }
}

//function creates an NFA for regular expressions without () and *
NFA make_NFA_for_simple_RE(int l, int r, vector<NFAorCHAR>X) {
    vector<NFA>result;
    result.clear();
    NFA tmp;
    tmp.start_state = "NOTHING";
    for(int i = l+1 ; i <= r ; i++ ) {
        if(tmp.start_state == "NOTHING") {
            tmp = X[i].NFA;
        }
        else if(X[i].is_ch) {
            result.PB(tmp);
            tmp.start_state = "NOTHING";
        }
        else {
            tmp = concate(tmp, X[i].NFA);
        }
    }
    while(result.size() > 1) {
        NFA tmp1, tmp2;
        NFA tmp3;
        tmp1 = result[result.size() - 2];
        tmp2 = result[result.size() - 1];
        tmp3 = ejtema(tmp1, tmp2);
        result.pop_back();
        result.pop_back();
        result.PB(tmp3);
    }
    return result[0];
}

//function creates an NFA for regular expressions that only have a single * at the end
NFA make_NFA_for_simple_RE_with_star(int l, int r, vector<NFAorCHAR>X) {
    NFA result = make_NFA_for_simple_RE(l, r, X);
    return star(result);
}

//function returns the number of NFAs inside an vector
int number_of_NFA(vector<NFAorCHAR>X) {
    int ans = 0;
    for(auto u: X) {
        ans += (u.is_ch == 0);
    }
    return ans;
}

//function returns true if X have star
bool have_star(vector<NFAorCHAR>X) {
    for(auto u: X) {
        if(u.is_ch == 1) {
            if(u.ch == '*') {
                return 1;
            }
        }
    }
    return 0;
}

//convert NFA type to NFAorCHAR
NFAorCHAR usable(NFA nfa) {
    NFAorCHAR ans;
    ans.NFA = nfa;
    ans.is_ch = 0;
    return ans;
}

// Function to combine NFAs using concatenation ans union and star operators
NFA make_dfa_from_standard_input(vector<NFAorCHAR>standard) {
    while(number_of_NFA(standard)>1 || have_star(standard)) {
        int l = -1;
        for(int i = 0 ; i < standard.size() ; i++) {
            if(standard[i].ch == '(') {
                l = i;
            }
            if(standard[i].ch == ')') {
                NFA rep;
                if(i+1 != standard.size() && standard[i+1].ch == '*') {
                    rep = make_NFA_for_simple_RE_with_star(l, i, standard);
                    i++;
                }
                else {
                    rep = make_NFA_for_simple_RE(l, i, standard);
                }
                vector<NFAorCHAR>new_standard;
                new_standard.clear();
                for(int j = 0 ; j < l ; j++ ) {
                    new_standard.PB(standard[j]);
                }
                new_standard.PB(usable(rep));
                for(int j = i + 1 ; j < standard.size() ; j++ ) {
                    new_standard.PB(standard[j]);
                }
                standard = new_standard;
                break;
            }
        }
    }
    for(auto u: standard) {
        if(u.is_ch == 0) {
            return u.NFA;
        }
    }
}

// function checks whether the given character is a member of the alphabet or not
bool is_alphabets(char s) {
    for(auto u: alphabets) {
        if(u[0] == s) return true;
    }
    return false;
}

// Function to convert regular expression to standard input format
vector<NFAorCHAR> make_standard_input(string regular) {
    regular = '(' + regular + ')';
    vector<NFAorCHAR>result;
    result.clear();
    for(auto u: regular) {
        if(u == '^')continue;
        if(is_alphabets(u) || to_string(u) == "λ") {
            NFAorCHAR tmp;
            string tmp2(1, u);
            tmp.NFA = make_simple_NFA(tmp2);
            tmp.is_ch = 0;
            result.PB(tmp);
        }
        else {
            NFAorCHAR tmp;
            tmp.ch = u;
            tmp.is_ch = 1;
            result.PB(tmp);
        }
    }
    return result;
}

string s;

// Function to read input from file and store in global variables
void decode_input() {
    ifstream inputfile ("RE_Input_3.txt");
    if(inputfile.is_open()) {
        string line;
        getline(inputfile, line);
        getline(inputfile, s);
        istringstream tmp(line);
        string element;
        while (getline(tmp, element, ' ')) {
            alphabets.push_back(element);
        }
    }
    else {
        cout << "Input file not found";
        exit(0);
    }
    inputfile.close();
}

int main () {
    decode_input();
    vector<NFAorCHAR> standard = make_standard_input(s);
    NFA ans = make_dfa_from_standard_input(standard);
    print(ans);
}
