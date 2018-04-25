/*
 * Janet Tang - jxt170630@utdallas.edu
 * CS 3377.502
 * homework 6 - main program
 */

#include <iostream>
#include <fstream>
#include <iomanip>
#include <stdint.h>
#include <stdlib.h>
#include <string>
#include <sstream>
#include "cdk.h"
#include <bitset>

#define MATRIX_WIDTH 3
#define MATRIX_HEIGHT 5
#define BOX_WIDTH 25
#define MATRIX_NAME_STRING "Binary File Contents"

using namespace std;

const int maxRecordStringLength = 25;


class BinaryFileRecord
{
public:

  uint8_t  strLength;
  char     stringBuffer[maxRecordStringLength];

};

class BinaryFileHeader
{
public:

  uint32_t  magicNumber;
  uint32_t  versionNumber;
  uint64_t  numRecords;

};

int main()
{
  /**** read from binary file and store data in array corresponding to display matrix ****/

  BinaryFileHeader *fileHeader = new BinaryFileHeader();
  BinaryFileRecord *myRecord = new BinaryFileRecord();
  ifstream binInfile("cs3377.bin", ios::in | ios::binary);
  string header[3];     // magicNumber, versionNumber, numRecords
  string output[4][2];  // strlen, record content

  if(binInfile.is_open() == false){
    printf("Error opening input file. \nExiting now.\n");
    _exit(1);
  }
  // read header
  binInfile.read((char *) fileHeader, sizeof(BinaryFileHeader));
  stringstream temp; 

  bitset<32> magic (fileHeader->magicNumber);
  temp << uppercase << hex << magic.to_ulong();
  header[0] = "0x" + temp.str();
  temp.str("");
  

  bitset<32> version (fileHeader->versionNumber);
  temp << version.to_ulong();
  header[1] = temp.str();
  temp.str("");

  bitset<32> records (fileHeader->numRecords);
  temp << records.to_ulong();
  header[2] = temp.str();

  int numRecs = records.to_ulong();
  temp.str("");

  // read records
  for(int i=0; i < numRecs; i++){
    binInfile.read((char *) myRecord, sizeof(BinaryFileRecord));

    bitset<32> len (myRecord->strLength);
    temp << dec << len.to_ulong();
    output[i][0] = "strlen: " + temp.str();
    temp.str("");

    temp << myRecord->stringBuffer;
    output[i][1] = temp.str();
    temp.str("");
  }

  binInfile.close();

  /**** create matrix using cdk library ****/

  WINDOW     *window;
  CDKSCREEN  *cdkscreen;
  CDKMATRIX  *myMatrix;          // CDK Screen Matrix

  const char *rowTitles[] = {"0", "a", "b", "c", "d", "e"};
  const char *columnTitles[] = {"0", "a", "b", "c", "d", "e"};
  int boxWidths[] = {BOX_WIDTH, BOX_WIDTH, BOX_WIDTH, BOX_WIDTH, BOX_WIDTH};
  int boxTypes[] = {vMIXED, vMIXED, vMIXED, vMIXED, vMIXED};

  // initialize CDK screen
  window = initscr();
  cdkscreen = initCDKScreen(window);

  //start CDK colors
  initCDKColor();

  //create matrix
  myMatrix = newCDKMatrix(cdkscreen, CENTER, CENTER, MATRIX_HEIGHT, MATRIX_WIDTH, MATRIX_HEIGHT, MATRIX_WIDTH,MATRIX_NAME_STRING, (char **) rowTitles, (char **) columnTitles, boxWidths, boxTypes, 1, 1, ' ', ROW, true, true, false);

  if(myMatrix == NULL){
    printf("Error creating matrix\n");
    _exit(1);
  }

  //display matrix
  drawCDKMatrix(myMatrix, true);
  string h1 = "Magic: " + header[0];
  string h2 = "Version: " + header[1];
  string h3 = "NumRecords: " + header[2];
  
  setCDKMatrixCell(myMatrix, 1, 1, h1.c_str());
  setCDKMatrixCell(myMatrix, 1, 2, h2.c_str());
  setCDKMatrixCell(myMatrix, 1, 3, h3.c_str());
  for(int i=0; i < numRecs; i++){
   setCDKMatrixCell(myMatrix, 2+i, 1, output[i][0].c_str());
  }
  for(int i=0; i < numRecs; i++){
    setCDKMatrixCell(myMatrix, 2+i, 2, output[i][1].c_str());
  }
  drawCDKMatrix(myMatrix, true);
  
  //pause until key is pressed
  unsigned char x;
  cin >> x;

  //cleanup screen
  endCDK();

  return 0;
}
