#include <bits/stdc++.h>

#define F first
#define S second
#define PB push_back
#define MP make_pair

using namespace std;

const int input_size = 100; // maximum number of lines in the input file
string input_line[input_size]; // array to store the input lines
int real_size_of_input = 0; // actual number of lines in the input file
map<string, vector<pair<string, string>>>adj;// adjacency list for the NFA transitions
map<string, vector<pair<string, string>>>dfa_adj;// adjacency list for the DFA transitions
map<string, bool> isis_final;// map to check if a state is final or reach to final with landa

void give_input() {
    ifstream inputfile ("NFA_Input_2.txt"); // open the input file
    if(inputfile.is_open()) { // check if the file was successfully opened
        while(getline(inputfile, input_line[real_size_of_input++])); // read each line and increment real_size_of_input
    }
    else {
        cout << "Input file not found"; // print an error message if the input file could not be opened
        exit(0); // exit the program with an error code
    }
    inputfile.close();
}

// split a string into tokens separated by spaces and return them as a vector of strings
vector<string> decode_line(string input) {
    istringstream tmp(input); // create a stringstream object tmp from the input string
    vector<string> answer; // create a vector of strings to store the tokens
    string element; // temporary string variable to store each token
    while (getline(tmp, element, ' ')) { // iterate over the stringstream object and extract tokens separated by spaces
        answer.push_back(element); // add the token to the vector of strings
    }
    return answer;
}

// vectors to store the alphabet, states, and final states
vector<string>alphabets, states, final_state;
string start_state; // string to store the start state

// decode the input lines and extract the alphabets, states, start_state, final_state, and adj structures
void decode_input() {
    alphabets = decode_line(input_line[0]); // decode the alphabet from the first line and Obtaining alphabets
    states = decode_line(input_line[1]); // decode the states from the second line and Obtaining states
    start_state = input_line[2]; // set the start state from the third line
    final_state = decode_line(input_line[3]); // decode the final states from the fourth line and Obtaining final states

    // iterate over the remaining input lines and decode the transitions
    for(int i = 4; i < real_size_of_input; i++) {
        vector<string>line = decode_line(input_line[i]); // decode the line into a vector of tokens
        if(line.size() != 3) break; // ignore any invalid input lines
        adj[line[0]].PB(make_pair(line[1], line[2])); // add the transition to the adj map
    }
}

map<string, bool>viz; // map to mark visited states during DFS traversal
vector<string>dfslanda(string x) { // function to perform DFS traversal starting from a given state
    vector<string>answer; // create a vector to store the visited states
    answer.clear(); // clear the vector
    viz[x] = 1; // mark the starting state as visited
    for(auto u: adj[x]) {// iterate over each transition from the current state
        if(viz[u.S] == 0 && u.F == "λ") { // if the next state has not been visited and the input symbol is landa
            answer = dfslanda(u.S); // perform DFS traversal starting from the next state
        }
    }
    answer.push_back(x); // add the current state to the visited states vector
    return answer; // return the vector of visited states
}

// function to perform DFS traversal on a group of states, only with landa transitions
vector<string>dfslandagroup(vector<string> group) {
    vector<string>answer; // create a vector to store the visited states
    answer.clear();
    for(auto u: group) { // iterate over each state in the given group
        viz.clear();
        vector<string>reachable = dfslanda(u); // perform DFS traversal starting from the current state
        for(int i = 0 ; i < reachable.size() ; i++ ) { // iterate over each visited state
            answer.push_back(reachable[i]); // add the state to the visited states vector
        }
    }
    sort(answer.begin(), answer.end()); // sort the vector of visited states in ascending order
    auto last = unique(answer.begin(), answer.end()); // remove any duplicate states from the vector
    answer.erase(last, answer.end()); // and erase them from the vector
    return answer;
}

// function to find the set of states that can be reached from a given set of states on a given input symbol
vector<string>use_char(vector<string> x, string c) {
    vector<string>answer; // create a vector to store the reachable states
    answer.clear();
    for(auto u: x) { // iterate over each state in the given set
        for(auto u2: adj[u]) { // iterate over each transition from the current state
            if(u2.F == c) { // if the input symbol is the one we're looking for
                answer.push_back(u2.S); // add the next state to the reachable states vector
            }
        }
    }
    return answer;
}

// function to concatenate a vector of strings into a single string
string concate(vector<string> x) {
    string answer; // create a string to store the concatenated string
    answer.clear(); // clear the string
    sort(x.begin(), x.end()); // sort the vector of strings in ascending order
    for(auto u: x) { // iterate over each string in the vector
        answer = answer + u; // concatenate the string to the answer string
    }
    return answer;
}

vector<string>final_state_dfa; // create a vector to store the final states of the constructed DFA

// function to construct the DFA from the NFA using the method mentioned in the report and lecture note
void make_dfa() {
    for(int k = 0 ; k < states.size() ; k++ ) { // iterate over each state in the NFA
        vector<string>reachable = dfslanda(states[k]); // find all states reachable from the current state with landa transitions
        for(auto u: final_state) { // iterate over each final state in the NFA
            for(auto u2: reachable) { // iterate over each reachable state from the current state
                if(u == u2) { // if the reachable state is also a final state
                    isis_final[states[k]] = 1; // mark the current state as final in the DFA
                }
            }
        }
    }
    for(int i = 0; i < (1 << (int)states.size()); i++) { // iterate over each subset of the states of the NFA
        bool make_it_final = 0, is_new = 1; // initialize variables to keep track of whether the current subset is a final state and whether it is a new state
        for(int j = 0 ; j < alphabets.size() ; j++) { // iterate over each input symbol in the alphabet
            vector<string>start_points, end_points; // create vectors to hold the starting and ending points of each transition
            start_points.clear();
            end_points.clear();
            for(int k = 0 ; k < states.size() ; k++ ) { // iterate over each state in the NFA
                if((1 << k) & i) { // if this state is part of the current subset
                    if(isis_final[states[k]]) { // and it is a final state in the DFA
                        make_it_final = 1; // mark the current subset as a final state in the DFA
                    }
                    start_points.push_back(states[k]); // add the current state to the starting points vector
                    viz.clear();
                    vector<string>reachable_from_start_with_landa = dfslanda(states[k]); // find all states reachable from the current state with landa transitions
                    viz.clear();
                    vector<string>reachable_from_start_with_landa_then_char = use_char(reachable_from_start_with_landa, alphabets[j]); // find all states reachable from the current state on the current input symbol
                    vector<string>reachable_from_start_with_landa_then_char_then_landa = dfslandagroup(reachable_from_start_with_landa_then_char); // find all states reachable from these states with landa transitions
                    for(auto u: reachable_from_start_with_landa_then_char_then_landa) { // iterate over each state in the reachable states vector
                        end_points.push_back(u); // add the state to the ending points vector
                    }
                    sort(end_points.begin(), end_points.end());// duplicate elements are removed
                    auto last = unique(end_points.begin(), end_points.end());
                    end_points.erase(last, end_points.end());
                }
            }
            string name_start_point = "tohi" + concate(start_points); // concatenate the starting points into a single string
            string name_end_point = "tohi" + concate(end_points); // concatenate the ending points into a single string
            if(make_it_final && is_new == 1) { // If it was the final state and has not been previously added.
                final_state_dfa.push_back(name_start_point); // add the transition to the adjacency list for the DFA
                is_new = 0;
            }
            dfa_adj[name_start_point].push_back(make_pair(alphabets[j], name_end_point));//adding a transition to a DFA.
        }
    }
}

//Displaying output in standard format on console and in the requested file
void show_dfa() {
    ofstream myfile ("DFA_Output_2.txt");
    if (!myfile.is_open()) {
        cout << "Unable to open file";
    }
    for(auto u: alphabets) {
        myfile << u << ' ';
    }
    myfile << '\n';
    for(auto u: dfa_adj) {
        myfile << u.F << ' ';
    }
    myfile << '\n';
    myfile << "tohi" + start_state << '\n';
    for(auto u: final_state_dfa) {
        myfile << u << ' ';
    }
    myfile << '\n';
    for(auto u: dfa_adj) {
        for(auto u2: dfa_adj[u.F]) {
            myfile << u.F << ' ' << u2.F << ' ' << u2.S << '\n';
        }
    }

    for(auto u: alphabets) {
        cout << u << ' ';
    }
    cout << '\n';
    for(auto u: dfa_adj) {
        cout << u.F << ' ';
    }
    cout << '\n';
    cout << "tohi" + start_state << '\n';
    for(auto u: final_state_dfa) {
        cout << u << ' ';
    }
    cout << '\n';
    for(auto u: dfa_adj) {
        for(auto u2: dfa_adj[u.F]) {
            cout << u.F << ' ' << u2.F << ' ' << u2.S << '\n';
        }
    }
}

int main () {
    give_input(); // read the input from the input file
    decode_input(); // decode the input
    make_dfa(); // Converting an NFA to a DFA
    show_dfa(); // Displaying and Saving Output
}
