#include <bits/stdc++.h>

#define PB push_back
#define PPF pop_front
#define PPB pop_back

using namespace std;

int state = 0, pointer = 0;

void normalize_tape(deque<char>&tape) {
    while(tape.size() > 0) {
        // Remove leading whitespace and decrement pointer
        if(tape[0] == ' ') {
            tape.PPF();
            pointer--;
        }
        // Remove trailing whitespace
        else if(tape[tape.size() - 1] == ' ') {
            tape.PPB();
        }
        // Stop when no more leading/trailing whitespace
        else {
            break;
        }
    }
}

// Check if tape head is out of range
bool outofrange(int x, deque<char>tape) {
    return !(0 <= x && x < tape.size());
}

// Check if tape head is out of range to the right
bool outofrange_right(int x, deque<char>tape) {
    return outofrange(x, tape) & (x >= tape.size());
}


// Advance to next state and modify tape
void next(int new_state, deque<char>&tape, char replac, char LR) {
    state = new_state;
    // Check if tape head is out of range
    if(outofrange(pointer, tape)) {
        // Add new symbol to end of tape
        if(outofrange_right(pointer, tape)) {
            tape.PB(replac);
        }
        //This state is not possible in the Turing machine implementation so its occurrence is not allowed
        else {
            if(replac != ' ' || LR != 'R') {
                cout << "INVALID";
                exit(0);
            }
            else {
                pointer++;
            }
        }
    }
    // Modify tape at current position
    else {
        tape[pointer] = replac;
    }
    // Move tape head left or right
    pointer += (LR == 'R');
    pointer -= (LR == 'L');
}

// Helper function to print contents of tape
void print(deque<char>c) {
    for(auto u: c) {
        cout << u;
    }
    cout << '\n';
}

int main () {
    int x, y;
    bool show_step;
    cin >> x >> y >> show_step;
    deque<char>tape;
    tape.clear();
    // Initialize tape with x 1's, '&' symbol, and y 1's
    for(int i = 0 ; i < x ; i++) {
        tape.PB('1');
    }
    tape.PB('&');
    for(int i = 0 ; i < y ; i++) {
        tape.PB('1');
    }
    while(true) {
        normalize_tape(tape);
        if(show_step) {
            // Print current state information if show step is true
            print(tape);
        }
        // Execute commands based on current state and symbol under tape head
        if(state == 0) {
            if(tape[pointer] == '1') {
                next(1, tape, 'X', 'R');
            }
            else if(tape[pointer] == '&') {
                next(5, tape, '&', 'R');
            }
        }
        else if(state == 1) {
            if(tape[pointer] == '1') {
                next(1, tape, '1', 'R');
            }
            else if(tape[pointer] == '&') {
                next(2, tape, '&', 'R');
            }
        }
        else if(state == 2) {
            if(outofrange(pointer, tape) || tape[pointer] == ' ') {
                next(9, tape, '=', 'L');
            }
            else if(tape[pointer] == '1') {
                next(3, tape, 'X', 'L');
            }
            else if(tape[pointer] == 'X') {
                next(2, tape, 'X', 'R');
            }
        }
        else if(state == 3) {
            if(tape[pointer] == 'X') {
                next(3, tape, 'X', 'L');
            }
            else if(tape[pointer] == '&') {
                next(4, tape, '&', 'L');
            }
        }
        else if(state == 4) {
            if(tape[pointer] == '1') {
                next(4, tape, '1', 'L');
            }
            else if(tape[pointer] == 'X') {
                next(0, tape, 'X', 'R');
            }
        }
        else if(state == 5) {
            if(outofrange(pointer, tape) || tape[pointer] == ' ') {
                next(9, tape, '=', 'L');
            }
            else if(tape[pointer] == 'X') {
                next(5, tape, 'X', 'R');
            }
            else if(tape[pointer] == '1') {
                next(6, tape, '1', 'R');
            }
        }
        else if(state == 6) {
            if(outofrange(pointer, tape) || tape[pointer] == ' ') {//test
                next(7, tape, '0', 'L');
            }
            else if(tape[pointer] == '1') {
                next(6, tape, '1', 'R');
            }
        }
        else if(state == 7) {
            if(outofrange(pointer, tape) || tape[pointer] == ' ') {
                next(8, tape, ' ', 'L');
            }
            else if(tape[pointer] == '1') {
                next(7, tape, ' ', 'L');
            }
            else if(tape[pointer] == 'X') {
                next(7, tape, ' ', 'L');
            }
            else if(tape[pointer] == '&') {
                next(7, tape, ' ', 'L');
            }
        }
        else if(state == 9) {
            if(outofrange(pointer, tape) || tape[pointer] == ' ') {
                next(10, tape, ' ', 'R');
            }
            else if(tape[pointer] == '1') {
                next(9, tape, '1', 'L');
            }
            else if(tape[pointer] == 'X') {
                next(9, tape, '1', 'L');
            }
            else if(tape[pointer] == '&') {
                next(9, tape, '&', 'L');
            }
        }
        else if(state == 10) {
            if(tape[pointer] == '1') {
                next(11, tape, ' ', 'R');
            }
            else if(tape[pointer] == '&') {
                next(17, tape, ' ', 'R');
            }
        }
        else if(state == 11) {
            if(tape[pointer] == '1') {
                next(11, tape, '1', 'R');
            }
            else if(tape[pointer] == '&') {
                next(12, tape, '&', 'R');
            }
        }
        else if(state == 12) {
            if(tape[pointer] == '1') {
                next(13, tape, 'X', 'R');
            }
            else if(tape[pointer] == '=') {
                next(16, tape, '=', 'L');
            }
        }
        else if(state == 13) {
            if(tape[pointer] == '1') {
                next(13, tape, '1', 'R');
            }
            else if(tape[pointer] == '=') {
                next(14, tape, '=', 'R');
            }
        }
        else if(state == 14) {
            if(outofrange(pointer, tape) || tape[pointer] == ' ') {
                next(15, tape, '1', 'L');
            }
            else if(tape[pointer] == '1') {
                next(14, tape, '1', 'R');
            }
        }
        else if(state == 15) {
            if(tape[pointer] == '1') {
                next(15, tape, '1', 'L');
            }
            else if(tape[pointer] == '=') {
                next(15, tape, '=', 'L');
            }
            else if(tape[pointer] == 'X') {
                next(12, tape, 'X', 'R');
            }
        }
        else if(state == 16) {
            if(outofrange(pointer, tape) || tape[pointer] == ' ') {
                next(10, tape, ' ', 'R');
            }
            else if(tape[pointer] == 'X') {
                next(16, tape, '1', 'L');
            }
            else if(tape[pointer] == '1') {
                next(16, tape, '1', 'L');
            }
            else if(tape[pointer] == '&') {
                next(16, tape, '&', 'L');
            }
        }
        else if(state == 17) {
            if(tape[pointer] == '1') {
                next(17, tape, ' ', 'R');
            }
            else if(tape[pointer] == '=') {
                next(18, tape, ' ', 'R');
            }
        }
        else {
            break;
        }
    }
    cout << "RESULT:\n";
    print(tape);
}
