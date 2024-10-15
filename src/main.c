#include <genesis.h>

static char board[3][3];
static int currentPlayer = 1; // 1 for player 'X', -1 for player 'O'
static int selectedX = 0, selectedY = 0, winner, movedBefore=0, moved;

/* Initialize the TicTacToe board */
void initialize_board(void) {
    currentPlayer=1;
    winner=0;
    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 3; x++) {
            board[y][x] = ' ';
        }
    }
    VDP_drawText("            ", 10, 16);
}

/* Draw the TicTacToe board */
void draw_board(void) {
    for (int y = 0; y < 3; y++) {
        VDP_drawText(" ", 9, 10 + y*2);
        for (int x = 0; x < 3; x++) {
            VDP_drawText(board[y][x] == ' ' ? " " : (board[y][x] == 'X' ? "X" : "O"), 10 + x * 2, 10 + y*2);
            if(x<2)
                VDP_drawText("|", 11 + x * 2, 10 + y*2);
        }
        VDP_drawText(" ", 15, 10 + y*2);
        if(y<2)
            VDP_drawText("-----", 10, 11 + y*2);
    }

    // Highlight the selected cell (you could draw some kind of cursor here)
    VDP_drawText("[", 10 + selectedX * 2 - 1, 10 + selectedY*2);
    VDP_drawText("]", 10 + selectedX * 2 + 1, 10 + selectedY*2);
}

/* Check for a win or draw */
int check_winner(void) {
    for (int i = 0; i < 3; i++) {
        // Check rows and columns
        if (board[i][0] != ' ' && board[i][0] == board[i][1] && board[i][1] == board[i][2])
            return board[i][0];
        if (board[0][i] != ' ' && board[0][i] == board[1][i] && board[1][i] == board[2][i])
            return board[0][i];
    }
    // Check diagonals
    if (board[0][0] != ' ' && board[0][0] == board[1][1] && board[1][1] == board[2][2])
        return board[0][0];
    if (board[0][2] != ' ' && board[0][2] == board[1][1] && board[1][1] == board[2][0])
        return board[0][2];

    // Check for draw
    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 3; x++) {
            if (board[y][x] == ' ') {
                return 0; // Still playing
            }
        }
    }

    return 'D'; // Draw
}

/* Handle player move */
void place_marker(void) {
    if (board[selectedY][selectedX] == ' ') {
        board[selectedY][selectedX] = currentPlayer == 1 ? 'X' : 'O';
        currentPlayer = -currentPlayer; // Switch player
    }
}

/* Handle player input for navigation and selecting cells */
void handle_input(void) {
    u16 value = JOY_readJoypad(JOY_1);
    moved=0;
    if (value & BUTTON_LEFT) {
        moved=1;
        if(!movedBefore)
            selectedX = selectedX == 0 ? 2 : selectedX - 1;
    }
    if (value & BUTTON_RIGHT) {
        moved=1;
        if(!movedBefore)
            selectedX = selectedX == 2 ? 0 : selectedX + 1;
    }
    if (value & BUTTON_UP) {
        moved=1;
        if(!movedBefore)
            selectedY = selectedY == 0 ? 2 : selectedY - 1;
    }
    if (value & BUTTON_DOWN) {
        moved=1;
        if(!movedBefore)
            selectedY = selectedY == 2 ? 0 : selectedY + 1;
    }
    movedBefore=moved;
    if ((value & BUTTON_A) && !winner) {
        place_marker();
    }
    if ((value & BUTTON_START) && winner) {
        initialize_board();
    }
}

/* Main game loop */
void my_game_loop(void) {
    handle_input();
    draw_board();

    winner = check_winner();
    if (winner == 'X') {
        VDP_drawText("X Wins!", 10, 16);
    } else if (winner == 'O') {
        VDP_drawText("O Wins!", 10, 16);
    } else if (winner == 'D') {
        VDP_drawText("It's a draw!", 10, 16);
    }
}

/* Main entry point */
int main() {
    VDP_setScreenWidth320();
    VDP_setScreenHeight240();

    VDP_drawText("TicTacToe", 10, 1);

    initialize_board();

    while(1) {
        my_game_loop();
        SYS_doVBlankProcess();
    }

    return 0;
}
