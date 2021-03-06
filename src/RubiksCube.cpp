#include <vector>
#include <cstdlib>

#include "RubiksCube.h"
#include "Shared.h"

// Faces as they appear at http://www.rubiksplace.com/move-notations/
RubiksCube::RubiksCube(): n_(3),
    xLeft_(Orange),
    xRight_(Red),
    yTop_(White),
    yBottom_(Yellow),
    zFront_(Green),
    zBack_(Blue)
{}

RubiksCube::RubiksCube(int*** state): n_(3),
    xLeft_(state[0]),
    xRight_(state[1]),
    yTop_(state[2]),
    yBottom_(state[3]),
    zFront_(state[4]),
    zBack_(state[5])
{}


int*** RubiksCube::getState() {
    int*** state = new int**[6];
    state[0] = xLeft_.getFace();
    state[1] = xRight_.getFace();
    state[2] = yTop_.getFace();
    state[3] = yBottom_.getFace();
    state[4] = zFront_.getFace();
    state[5] = zBack_.getFace();

    return state;
}

int* RubiksCube::reverse(int *a) {
    int* newStrip = new int[n_];
    int i;
    for (i=0; i<n_; i++) {
        newStrip[i] = a[n_-i-1];
    }

    return newStrip;
}

void RubiksCube::rotateXLeft() {
    xLeft_.rotateFaceClockwise(Ninety);

    int *tempCol = yTop_.getCol(0);
    yTop_.setCol(0, reverse(zBack_.getCol(2)));
    zBack_.setCol(2, reverse(yBottom_.getCol(0)));
    yBottom_.setCol(0, zFront_.getCol(0));
    zFront_.setCol(0, tempCol);
}

void RubiksCube::rotateXRight() {
    xRight_.rotateFaceClockwise(Ninety);

    int *tempCol = yTop_.getCol(2);
    yTop_.setCol(2, zFront_.getCol(2));
    zFront_.setCol(2, yBottom_.getCol(2));
    yBottom_.setCol(2, reverse(zBack_.getCol(0)));
    zBack_.setCol(0, reverse(tempCol));
}

void RubiksCube::rotateYTop() {
    yTop_.rotateFaceClockwise(Ninety);

    int *tempRow = xLeft_.getRow(0);
    xLeft_.setRow(0, zFront_.getRow(0));
    zFront_.setRow(0, xRight_.getRow(0));
    xRight_.setRow(0, zBack_.getRow(0));
    zBack_.setRow(0, tempRow);
}

void RubiksCube::rotateYBottom() {
    yBottom_.rotateFaceClockwise(Ninety);

    int *tempRow = xLeft_.getRow(2);
    xLeft_.setRow(2, zBack_.getRow(2));
    zBack_.setRow(2, xRight_.getRow(2));
    xRight_.setRow(2, zFront_.getRow(2));
    zFront_.setRow(2, tempRow);
}

void RubiksCube::rotateZFront() {
    zFront_.rotateFaceClockwise(Ninety);

    int *yTopLastRow = yTop_.getRow(2);
    yTop_.setRow(2, reverse(xLeft_.getCol(2)));
    xLeft_.setCol(2, yBottom_.getRow(0));
    yBottom_.setRow(0, reverse(xRight_.getCol(0)));
    xRight_.setCol(0, yTopLastRow);
}

void RubiksCube::rotateZBack() {
    zBack_.rotateFaceClockwise(Ninety);

    int *yTopFirstRow = yTop_.getRow(0);
    yTop_.setRow(0, xRight_.getCol(2));
    xRight_.setCol(2, reverse(yBottom_.getRow(2)));
    yBottom_.setRow(2, xLeft_.getCol(0));
    xLeft_.setCol(0, reverse(yTopFirstRow));
}

void RubiksCube::rotateXSlice() {
    int* tempCol = yTop_.getCol(1);
    yTop_.setCol(1, reverse(zBack_.getCol(1)));
    zBack_.setCol(1, reverse(yBottom_.getCol(1)));
    yBottom_.setCol(1, zFront_.getCol(1));
    zFront_.setCol(1, tempCol);
}

void RubiksCube::rotateYSlice() {
    int* tempRow = zFront_.getRow(1);
    zFront_.setRow(1, xLeft_.getRow(1));
    xLeft_.setRow(1, zBack_.getRow(1));
    zBack_.setRow(1, xRight_.getRow(1));
    xRight_.setRow(1, tempRow);
}

void RubiksCube::rotateZSlice() {
    int* temp = yTop_.getRow(1);
    yTop_.setRow(1, reverse(xLeft_.getCol(1)));
    xLeft_.setCol(1, yBottom_.getRow(1));
    yBottom_.setRow(1, reverse(xRight_.getCol(1)));
    xRight_.setCol(1, temp);
}

Move RubiksCube::oppositeMove(Move m) {
    Move opposite;
    opposite.slice = m.slice;

    if (m.degrees == TwoSeventy) {
        opposite.degrees = Ninety;
    } else if (m.degrees == OneEighty) {
        opposite.degrees = OneEighty;
    } else if (m.degrees == Ninety) {
        opposite.degrees = TwoSeventy;
    }

    return opposite;
}

std::vector<Move> RubiksCube::scramble(int n) {
    std::vector<Move> moves;

    for (int i=0; i < n; i++) {
        Move m;
        m.slice = static_cast<LetterNotation>(rand() % LastRotation);
        m.degrees = TwoSeventy;
        rotate(m.slice, m.degrees);

        // Push the reverse of the move onto the list of moves, for unscrambling
        Move opposite = RubiksCube::oppositeMove(m);
        moves.push_back(opposite);
    }

    return moves;
}

void RubiksCube::rotate(Move move) {
    rotate(move.slice, move.degrees);
}

void RubiksCube::rotate(LetterNotation slice, Degrees degrees) {
    for (int i=0; i < degrees+1; i++) {
        switch (slice) {
            case U:
                rotateYTop();
                break;
            case L:
                rotateXLeft();
                break;
            case F:
                rotateZFront();
                break;
            case R:
                rotateXRight();
                break;
            case B:
                rotateZBack();
                break;
            case D:
                rotateYBottom();
                break;
            case M:
                rotateXSlice();
                break;
            case E:
                rotateYSlice();
                break;
            case S:
                rotateZSlice();
                break;
            case LastRotation:
            default:
                return;
        }
    }
}
