#include <bits/stdc++.h>

#define F first
#define S second
#define PB push_back
#define MP make_pair

using namespace std;

const int input_size = 100; // maximum number of lines in the input file
string input_line[input_size]; // array to store the input lines
int real_size_of_input = 0; // actual number of lines in the input file

// map to store transitions of the DFA
// each key-value pair represents a transition from one state to another on a specific input symbol
map<string, vector<pair<string, string>>>adj;

// read input from a file called "DFA_Input_1.txt" into the `input_line` array
void give_input() {
    ifstream inputfile ("DFA_Input_1.txt"); // open the input file
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

// vectors to store the DFA alphabet, states, start state, and final states
vector<string>alphabets, states, final_state;
string start_state;

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

// simulate the DFA on a given input word and return true if it is accepted, false otherwise
bool dfa_iterator(string word) {
    string current_state = start_state; // initialize the current state to the start state
    for(int i = 0; i < word.size(); i++) { // iterate over the characters in the input word
        bool next_char = 0; // flag to check if the next character is valid
        for(auto u: adj[current_state]) { // iterate over the transitions from the current state
            if(u.F[0] == word[i]) { // check if the transition is valid for the current character
                current_state = u.S; // set the current state to the next state
                next_char = 1; // set the flag to indicate a valid character has been found
                break; // break out of the loop since a valid transition has been found
            }
        }
        if(next_char == 0) { // if no valid transition was found for the current character
            return false; // return false since the word cannot be accepted by the DFA
        }
    }
    for(auto u: final_state) { // iterate over the final states of the DFA
        if(u == current_state) { // check if the current state is a final state
            return true; // if it is, return true since the word is accepted by the DFA
        }
    }
    return false;
}

// read user input strings from the console and print whether they are accepted or rejected by the DFA
void give_user_input() {
    string word; // temporary string variable to store each input word
    while(getline(cin, word)) { // read a line of input from the console until end of file (EOF) is reached
        bool accept = dfa_iterator(word); // check if the input word is accepted by the DFA
        if(accept) { // if the word is accepted
            cout << "machine accept " << word << '\n';
        }
        else { // if the word is rejected
            cout << "machine reject " << word << '\n';
        }
    }
}

int main () {
    give_input(); // read the input from the input file
    decode_input(); // decode the input and construct the DFA
    give_user_input(); // give user input strings and check if they are accepted by the DFA
}
