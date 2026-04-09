// ============================================================
//        ADVANCED TIC TAC TOE  -  C++ Console Game
// ============================================================

#include <iostream>
#include <limits>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;

// ANSI Colors
#define RESET   "\033[0m"
#define BOLD    "\033[1m"
#define RED     "\033[31m"
#define BLUE    "\033[34m"
#define CYAN    "\033[36m"
#define YELLOW  "\033[33m"
#define GREEN   "\033[32m"
#define MAGENTA "\033[35m"

// Global Variables
char board[3][3];
int  playerScore = 0, aiScore = 0, drawScore = 0;
int  totalMoves  = 0;
string player1Name = "Player 1";
string player2Name = "Player 2";
bool hardMode = true;

// Game States
enum GameState { MENU, SETUP, PLAYING, ROUND_OVER, GAME_OVER };
GameState currentState = MENU;

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

string stateLabel() {
    switch(currentState) {
        case MENU:       return string(MAGENTA) + "MENU"       + RESET;
        case SETUP:      return string(CYAN)    + "SETUP"      + RESET;
        case PLAYING:    return string(GREEN)   + "PLAYING"    + RESET;
        case ROUND_OVER: return string(YELLOW)  + "ROUND OVER" + RESET;
        case GAME_OVER:  return string(RED)     + "GAME OVER"  + RESET;
    }
    return "";
}

// Reset Board - nested loop + 2D array
void resetBoard() {
    char ch = '1';
    for(int i = 0; i < 3; i++)
        for(int j = 0; j < 3; j++)
            board[i][j] = ch++;
    totalMoves = 0;
}

void displayHeader() {
    cout << CYAN BOLD;
    cout << "  +==================================+\n";
    cout << "  |       TIC  TAC  TOE  v2.0        |\n";
    cout << "  +==================================+\n";
    cout << RESET;
    cout << "  State: " << stateLabel() << "\n\n";
}

void displayScore() {
    cout << "  +----------------------------------+\n";
    cout << "  |  " << BLUE << BOLD << player1Name << RESET << "(X):" << playerScore
         << "  " << RED << BOLD << player2Name << RESET << "(O):" << aiScore
         << "  Draw:" << drawScore << "\n";
    cout << "  +----------------------------------+\n\n";
}

// Display Board - nested loop + 2D array
void displayBoard() {
    cout << "\n";
    cout << "  +-------+-------+-------+\n";
    for(int i = 0; i < 3; i++) {
        cout << "  |       |       |       |\n";
        cout << "  |";
        for(int j = 0; j < 3; j++) {
            char c = board[i][j];
            cout << "   ";
            if      (c == 'X') cout << BOLD << BLUE  << "X" << RESET;
            else if (c == 'O') cout << BOLD << RED   << "O" << RESET;
            else               cout << YELLOW << c << RESET;
            cout << "   |";
        }
        cout << "\n";
        cout << "  |       |       |       |\n";
        cout << "  +-------+-------+-------+\n";
    }
    cout << "\n  Move: " << BOLD << totalMoves << "/9" << RESET << "\n\n";
}

// Check moves left - nested loop
bool isMovesLeft() {
    for(int i = 0; i < 3; i++)
        for(int j = 0; j < 3; j++)
            if(board[i][j] != 'X' && board[i][j] != 'O')
                return true;
    return false;
}

int evaluate() {
    for(int i = 0; i < 3; i++) {
        if(board[i][0]==board[i][1] && board[i][1]==board[i][2]) {
            if(board[i][0]=='O') return +10;
            if(board[i][0]=='X') return -10;
        }
        if(board[0][i]==board[1][i] && board[1][i]==board[2][i]) {
            if(board[0][i]=='O') return +10;
            if(board[0][i]=='X') return -10;
        }
    }
    if(board[0][0]==board[1][1] && board[1][1]==board[2][2]) {
        if(board[0][0]=='O') return +10;
        if(board[0][0]=='X') return -10;
    }
    if(board[0][2]==board[1][1] && board[1][1]==board[2][0]) {
        if(board[0][2]=='O') return +10;
        if(board[0][2]=='X') return -10;
    }
    return 0;
}

// Minimax with depth - nested loops
int minimax(bool isMax, int depth) {
    int score = evaluate();
    if(score ==  10) return score - depth;
    if(score == -10) return score + depth;
    if(!isMovesLeft()) return 0;

    int best = isMax ? -1000 : 1000;
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            if(board[i][j] != 'X' && board[i][j] != 'O') {
                char temp   = board[i][j];
                board[i][j] = isMax ? 'O' : 'X';
                int val     = minimax(!isMax, depth + 1);
                board[i][j] = temp;
                best = isMax ? max(best, val) : min(best, val);
            }
        }
    }
    return best;
}

void easyMove() {
    srand((unsigned)time(0));
    int empties[9], count = 0;
    for(int i = 0; i < 3; i++)
        for(int j = 0; j < 3; j++)
            if(board[i][j] != 'X' && board[i][j] != 'O')
                empties[count++] = i * 3 + j;
    if(count > 0) {
        int pick = empties[rand() % count];
        board[pick / 3][pick % 3] = 'O';
    }
}

void bestMove() {
    if(!hardMode) { easyMove(); return; }
    int bestVal = -1000, bestRow = -1, bestCol = -1;
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            if(board[i][j] != 'X' && board[i][j] != 'O') {
                char temp   = board[i][j];
                board[i][j] = 'O';
                int val     = minimax(false, 0);
                board[i][j] = temp;
                if(val > bestVal) { bestVal = val; bestRow = i; bestCol = j; }
            }
        }
    }
    board[bestRow][bestCol] = 'O';
}

bool checkWin(char p) {
    for(int i = 0; i < 3; i++) {
        if(board[i][0]==p && board[i][1]==p && board[i][2]==p) return true;
        if(board[0][i]==p && board[1][i]==p && board[2][i]==p) return true;
    }
    if(board[0][0]==p && board[1][1]==p && board[2][2]==p) return true;
    if(board[0][2]==p && board[1][1]==p && board[2][0]==p) return true;
    return false;
}

void playerMove(char symbol) {
    string name  = (symbol == 'X') ? player1Name : player2Name;
    string color = (symbol == 'X') ? BLUE : RED;
    int choice;
    while(true) {
        cout << "  " << color << BOLD << name << RESET
             << " (" << color << symbol << RESET << ") -- Enter box (1-9): ";
        cin >> choice;
        if(cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << RED "  [!] Enter a number 1-9.\n" RESET;
            continue;
        }
        int r = (choice - 1) / 3;
        int c = (choice - 1) % 3;
        if(choice >= 1 && choice <= 9 && board[r][c] != 'X' && board[r][c] != 'O') {
            board[r][c] = symbol;
            totalMoves++;
            break;
        } else {
            cout << RED "  [!] Box taken or out of range. Try again.\n" RESET;
        }
    }
}

void playGame(bool vsAI) {
    resetBoard();
    currentState = PLAYING;

    while(true) {
        clearScreen();
        displayHeader();
        displayScore();
        displayBoard();

        playerMove('X');

        if(checkWin('X')) {
            currentState = ROUND_OVER;
            clearScreen(); displayHeader(); displayScore(); displayBoard();
            cout << GREEN BOLD "  >> " << player1Name << " (X) WINS this round!\n" RESET;
            playerScore++;
            return;
        }
        if(!isMovesLeft()) {
            currentState = ROUND_OVER;
            clearScreen(); displayHeader(); displayScore(); displayBoard();
            cout << YELLOW BOLD "  >> It's a DRAW!\n" RESET;
            drawScore++;
            return;
        }

        if(vsAI) {
            cout << MAGENTA "  >> CPU is thinking...\n\n" RESET;
            bestMove();
            totalMoves++;
        } else {
            playerMove('O');
        }

        if(checkWin('O')) {
            currentState = ROUND_OVER;
            clearScreen(); displayHeader(); displayScore(); displayBoard();
            if(vsAI)
                cout << RED BOLD "  >> CPU wins! Better luck next time.\n" RESET;
            else
                cout << RED BOLD "  >> " << player2Name << " (O) WINS this round!\n" RESET;
            aiScore++;
            return;
        }
        if(!isMovesLeft()) {
            currentState = ROUND_OVER;
            clearScreen(); displayHeader(); displayScore(); displayBoard();
            cout << YELLOW BOLD "  >> It's a DRAW!\n" RESET;
            drawScore++;
            return;
        }
    }
}

void showRules() {
    clearScreen();
    displayHeader();
    cout << "  +----------------------------------+\n";
    cout << "  |              RULES               |\n";
    cout << "  +----------------------------------+\n";
    cout << "  |  - Board has 9 boxes (1 to 9).   |\n";
    cout << "  |  - Players take turns.           |\n";
    cout << "  |  - Get 3 in a row to win:        |\n";
    cout << "  |      Horizontal / Vertical       |\n";
    cout << "  |      Diagonal                    |\n";
    cout << "  |  - All boxes filled = Draw.      |\n";
    cout << "  |  - Scores tracked each round.    |\n";
    cout << "  +----------------------------------+\n";
    cout << "\n  Press ENTER to go back...";
    cin.ignore(); cin.get();
}

void showGameOver() {
    currentState = GAME_OVER;
    clearScreen();
    displayHeader();
    cout << "  +----------------------------------+\n";
    cout << "  |         FINAL SCOREBOARD         |\n";
    cout << "  +----------------------------------+\n";
    cout << "  |  " << BLUE << BOLD << player1Name << RESET << " (X) : " << playerScore << "\n";
    cout << "  |  " << RED  << BOLD << player2Name << RESET << " (O) : " << aiScore     << "\n";
    cout << "  |  Draws : " << drawScore << "\n";
    cout << "  +----------------------------------+\n\n";
    if     (playerScore > aiScore) cout << GREEN  << BOLD << "  >> Overall Winner: " << player1Name << "!\n" << RESET;
    else if(aiScore > playerScore) cout << RED    << BOLD << "  >> Overall Winner: " << player2Name << "!\n" << RESET;
    else                           cout << YELLOW << BOLD << "  >> Overall Result: It's a tie!\n"           << RESET;
    cout << "\n  Thanks for playing!\n\n";
}

int main() {
    int choice;

    while(true) {
        // ======= STATE: MENU =======
        currentState = MENU;
        clearScreen();
        displayHeader();

        cout << "  +----------------------------------+\n";
        cout << "  |  [1]  Player vs Player           |\n";
        cout << "  |  [2]  Player vs AI  (Easy)       |\n";
        cout << "  |  [3]  Player vs AI  (Hard)       |\n";
        cout << "  |  [4]  Rules                      |\n";
        cout << "  |  [0]  Exit                       |\n";
        cout << "  +----------------------------------+\n";
        cout << "\n  Choice: ";
        cin >> choice;

        if(choice == 0) { showGameOver(); break; }
        if(choice == 4) { showRules(); continue; }
        if(choice < 1 || choice > 3) {
            cout << RED "  [!] Invalid choice.\n" RESET;
            continue;
        }

        // ======= STATE: SETUP =======
        currentState = SETUP;
        clearScreen();
        displayHeader();

        bool vsAI = (choice == 2 || choice == 3);
        hardMode  = (choice == 3);

        cin.ignore();
        cout << "  Enter " << BLUE << BOLD << "Player 1" << RESET << " name (X) [Enter to skip]: ";
        getline(cin, player1Name);
        if(player1Name.empty()) player1Name = "Player 1";

        if(!vsAI) {
            cout << "  Enter " << RED << BOLD << "Player 2" << RESET << " name (O) [Enter to skip]: ";
            getline(cin, player2Name);
            if(player2Name.empty()) player2Name = "Player 2";
        } else {
            player2Name = hardMode ? "CPU (Hard)" : "CPU (Easy)";
            cout << "  Opponent: " << RED << BOLD << player2Name << RESET << "\n";
        }

        playerScore = aiScore = drawScore = 0;
        cout << "\n  " << GREEN << "Ready! Press ENTER to start..." << RESET;
        cin.get();

        // ======= ROUNDS LOOP =======
        while(true) {
            playGame(vsAI);

            cout << "\n  +------------------------------+\n";
            cout << "  |  [1]  Play another round     |\n";
            cout << "  |  [2]  Main Menu              |\n";
            cout << "  |  [0]  Exit                   |\n";
            cout << "  +------------------------------+\n";
            cout << "  Choice: ";
            int rc; cin >> rc;

            if(rc == 1) continue;
            if(rc == 0) { showGameOver(); return 0; }
            break;
        }
    }

    return 0;
}
