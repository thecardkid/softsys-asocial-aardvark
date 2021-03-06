#include "CubeFace.h"
#include <iostream>

CubeFace::CubeFace(Color color): n_(3) {
    face_ = new int*[n_];

    for (int i=0; i < n_; i++) {
        face_[i] = new int[n_];
        std::fill_n(face_[i], n_, color);
    }
}

CubeFace::CubeFace(int** state): n_(3) {
    face_ = state;
}

void CubeFace::transposeFace() {
    int temp;
    for (int r=0; r <= n_-2; r++) {
        for (int c=r+1; c <= n_-1; c++) {
            temp = face_[r][c];
            face_[r][c] = face_[c][r];
            face_[c][r] = temp;
        }
    }
}

void CubeFace::reverseRows() {
    int temp;
    for (int r=0; r < n_; r++) {
        for (int c=0; c < n_/2; c++) {
            temp = face_[r][c];
            face_[r][c] = face_[r][n_-1-c];
            face_[r][n_-1-c] = temp;
        }
    }
}

void CubeFace::reverseCols() {
    int temp;
    for (int c=0; c < n_; c++) {
        for (int r=0; r < n_/2; r++) {
            temp = face_[r][c];
            face_[r][c] = face_[n_-1-r][c];
            face_[n_-1-r][c] = temp;
        }
    }
}

void CubeFace::rotate90() {
    transposeFace();
    reverseRows();
}

void CubeFace::rotate180() {
    rotate90();
    rotate90();
}

void CubeFace::rotate270() {
    transposeFace();
    reverseCols();
}

int** CubeFace::getFace() {
    int** copyOfFace = new int*[n_];
    for (int r=0; r < n_; r++) {
        copyOfFace[r] = new int[n_];
        for (int c=0; c < n_; c++)
            copyOfFace[r][c] = face_[r][c];
    }

    return copyOfFace;
}

void CubeFace::rotateFaceClockwise(Degrees d) {
    switch (d) {
        case Ninety: return rotate90();
        case OneEighty: return rotate180();
        case TwoSeventy: return rotate270();
        case LastDegree:
        default: return;
    }
}

bool CubeFace::isValidIndex(int i) {
    return (i < n_ && i >= 0);
}

void CubeFace::setRow(int whichRow, int* row) {
    if (isValidIndex(whichRow))
        face_[whichRow] = row;
}

int* CubeFace::getRow(int whichRow) {
    if (isValidIndex(whichRow)) {
        int *copyOfRow = new int[n_];
        for (int i=0; i < n_; i++) copyOfRow[i] = face_[whichRow][i];
        return copyOfRow;
    }

    return NULL;
}

void CubeFace::setCol(int whichCol, int *col) {
    if (isValidIndex(whichCol)) {
        for (int i=0; i < n_; i++)
            face_[i][whichCol] = col[i];
    }
}

int* CubeFace::getCol(int whichCol) {
    if (isValidIndex(whichCol)) {
        int* copyOfCol = new int[n_];
        for (int i=0; i < n_; i++) copyOfCol[i] = face_[i][whichCol];
        return copyOfCol;
    }

    return NULL;
}
