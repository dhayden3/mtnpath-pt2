//
//  main.cpp
//  p3
//
//  Created by Darrell L. Hayden Jr. on 9/20/17.
//  Copyright © 2017 ITCS3122. All rights reserved.
//
#include<iomanip>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int** getImg(string,int,int);
int findMax(int** pData,int,int);
void createPPMFile(int** pData, int, int, int);
int** findLowPath(int** pData,int,int);
void createPPMFileOutput(int** lowPath,int** pData,int,int,int);

int main() {
    string fileName;
    int **pData;
    int row,col;
    int maxPixel;
    int **lowPath;
    cout << "Filename: " << endl;
    cin >> fileName;
    
    cout << "Enter image height: " << endl;
    cin >> row;
    
    cout << "Enter image width: " << endl;
    cin >> col;
    
    pData = getImg(fileName,row,col);
    
    maxPixel = findMax(pData,row,col);
    
    createPPMFile(pData,row,col,maxPixel);
    
    for(int i = 0; i < 3; i++){
        lowPath = findLowPath(pData,row,col);
        createPPMFileOutput(lowPath,pData,maxPixel,row,col);
    }
    
    delete [] lowPath;
    
    delete [] pData;
    
    return 0;
}

int** getImg(string inFileName,int numRow, int numCol){
    //  Create input file
    ifstream file(inFileName);
    
    const int SIZE = numRow * numCol;
    int **pData;    // Declare a pointer to a pointer
    int temp[SIZE+2];
    int data[SIZE];
    
    pData = new int*[numRow];  // Allocate memory for the array of pointers that will store the pointers to arrays
    
    // Allocate memory for each row of two-dimensional array
    for (int i = 0; i != numRow; ++i){
        pData[i] = new int[numRow];
    }
    
    // Reads file contents int an 1D array
    if(file.is_open())
    {
        for(int i = 0; i < SIZE + 2; i++){
            file >> temp[i];
        }
    }
    // Store array image pixels w/o image dimension
    for(int i = 0; i < SIZE; i++){
        data[i] = temp [i+2];
    }
    
    // Pointer to 1D array
    int counter = 0;
    for(int i = 0; i < numRow; i++){
        pData[i] = &data[counter];
        counter += numCol;
    }
    
    file.close();
    
    return pData;
}


// Find the the max pixel value in array
int findMax(int** pData,int numRow, int numCol){
    int temp = 0;
    
    for(int i = 0; i < numRow; i++){
        for(int j = 0; j < numCol; j++){
            if(pData[i][j] > temp){
                temp = pData[i][j];
            }
        }
    }
    
    return temp;
}

// Creates a file in PPM image format
void createPPMFile(int** pData, int numRow, int numCol, int maxPixel){
    ofstream outputFile;
    
    string fileName = "ppmData.ppm";
    outputFile.open(fileName);
    
    // File header
    outputFile << "P3 " << endl;
    outputFile << numRow << " " << numCol << " # Image height and width" << endl;
    outputFile << maxPixel << " # Image Max Pixel Value " << endl;
    
    for (int i = 0; i < numRow; i++) {
        for(int j = 0; j < numCol; j++){
            outputFile << pData[i][j] << " " << pData[i][j] << " " << pData[i][j] << " ";

        }
    }
    
    outputFile.close();
}

int** findLowPath(int ** pData,int numRow, int numCol){
    int y = 0;
    int x = 0;
    int count = 0;
    
    cout << "Enter desired start row number (between " << 0 << "-" << numCol-1 << "): " << endl;
    cin >> x;
    
    int** lowPath = new int* [numRow];
    
    for (int i = 0; i != numRow; ++i){
        lowPath[i] = new int[numRow];
    }
    
    cout << lowPath[0][0] << endl;
    for(int i = 0; i < numRow; i++){
        for(int j = 0; j < numCol; j++){
            lowPath[i][j] = pData[i][j];
        }
    }
    
    int topRight = pData[x][y+1];
    int centerRight = pData[x][y+1];
    int bottomRight = pData[x+1][y+1];
    
    // Greedy alg.
    do{
        if(topRight <= centerRight && topRight <= bottomRight && x != 0){
            x -= 1;
            y += 1;
            
            
            lowPath[x][y] = -1;
            
            topRight = pData[x][y+1];
            centerRight = pData[x][y+1];
            bottomRight = pData[x+1][y+1];
        }
        else if(centerRight <= topRight && topRight <= bottomRight){
            x = x;
            y += 1;
            
            
            lowPath[x][y] = -1;
            
            topRight = pData[x][y+1];
            centerRight = pData[x][y+1];
            bottomRight = pData[x+1][y+1];
        }
        
        else if(bottomRight < topRight &&  bottomRight < centerRight && x <= numRow-1){
            x += 1;
            y += 1;
            
            lowPath[x][y] = -1;
            
            topRight = pData[x][y+1];
            centerRight = pData[x][y+1];
            bottomRight = pData[x+1][y+1];
        }
        else{
            x = x;
            y += 1;
            
            lowPath[x][y] = -1;
            
            topRight = pData[x][y+1];
            centerRight = pData[x][y+1];
            bottomRight = pData[x+1][y+1];
        }
        count++;
    }while(count < numCol);
    
    return lowPath;
}


void createPPMFileOutput(int** lowPath,int** pData,int maxPixel,int numRow, int numCol){
    ofstream outputFile;
    string fileName;
    
    cout << "Name the output file: "<< endl;
    cin >> fileName;
    
    outputFile.open(fileName);
    
    // Output file header
    outputFile << "P3 " << endl;
    outputFile << numRow << " " << numCol << " # Image height and width" << endl;
    outputFile << maxPixel << " # Image Max Pixel Value " << endl;
    
    // image data: loops over all the pixels and sets them all to red if on path
    for (int i = 0; i < numRow; i++) {
        for (int j = 0; j < numCol; j++){
            if(lowPath[i][j] == -1){
                outputFile << maxPixel << " " << 0 << " " << 0 << " ";
            }
            else{
                outputFile << pData[i][j] << " " << pData[i][j] << " " << pData[i][j] << " ";
            }
        }
    }
    
    outputFile.close();
}
