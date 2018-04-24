// Brian Bell
// bdb160130@utdallas.edu
// CS 3377.002

#include <iostream>
#include <fstream>
#include <cstdint>
#include "cdk.h"

#define MATRIX_WIDTH 3
#define MATRIX_HEIGHT 5
#define BOX_WIDTH 20
#define MATRIX_NAME_STRING "Test Matrix"

using namespace std;


class BinaryFileHeader
{
public:
  uint32_t magicNumber;
  uint32_t versionNumber;
  uint64_t numRecords;
};

class BinaryFileRecord
{
public:
  uint8_t strLength;
  char stringBuffer[25];
};

int main()
{

  WINDOW	*window;
  CDKSCREEN	*cdkscreen;
  CDKMATRIX     *myMatrix;           // CDK Screen Matrix

  // Remember that matrix starts out at 1,1.
  // Since arrays start out at 0, the first entries
  // below ("R0", and "C0") are just placeholders
  // 
  // Finally... make sure your arrays have enough entries given the
  // values you choose to set for MATRIX_WIDTH and MATRIX_HEIGHT
  // above.

  const char 		*rowTitles[] = {"R0", "R1", "R2", "R3", "R4", "R5"};
  const char 		*columnTitles[] = {"C0", "C1", "C2", "C3", "C4", "C5"};
  int		boxWidths[] = {BOX_WIDTH, BOX_WIDTH, BOX_WIDTH, BOX_WIDTH, BOX_WIDTH, BOX_WIDTH};
  int		boxTypes[] = {vMIXED, vMIXED, vMIXED, vMIXED,  vMIXED,  vMIXED};

  /*
   * Initialize the Cdk screen.
   *
   * Make sure the putty terminal is large enough
   */
  window = initscr();
  cdkscreen = initCDKScreen(window);

  /* Start CDK Colors */
  initCDKColor();

  /*
   * Create the matrix.  Need to manually cast (const char**) to (char **)
  */
  myMatrix = newCDKMatrix(cdkscreen, CENTER, CENTER, MATRIX_HEIGHT, MATRIX_WIDTH, MATRIX_HEIGHT, MATRIX_WIDTH,
			  MATRIX_NAME_STRING, (char **) rowTitles, (char **) columnTitles, boxWidths,
				     boxTypes, 1, 1, ' ', ROW, true, true, false);

  if (myMatrix ==NULL)
    {
      printf("Error creating Matrix\n");
      _exit(1);
    }

  // Create ifstream
  ifstream input("cs3377.bin", ios::in | ios::binary);
  if (!input.is_open()) {
    return 1;
  }
  
  //Read a BinaryFileHeader from the binary file
  BinaryFileHeader* header = new BinaryFileHeader();
  input.read((char*)header,sizeof(BinaryFileHeader));

  //Build a properly formatted magicNumber string and add it to the matrix
  char* magicNumber = new char[41];
  sprintf(magicNumber,"Magic: 0x%X",header->magicNumber);
  setCDKMatrixCell(myMatrix, 1, 1, magicNumber);

  //Build a properly formated versionNumber string and add it to the matrix
  char* versionNumber = new char[41];
  sprintf(versionNumber, "Version: %d", header->versionNumber);
  setCDKMatrixCell(myMatrix,1,2,versionNumber); 

  //Build a properly formatted NumRecords string and add it to the matrix
  char* numRecords = new char[76];
  sprintf(numRecords,"NumRecords: %u", header->numRecords);
  setCDKMatrixCell(myMatrix,1,3,numRecords);

  /* Display the Matrix */
  drawCDKMatrix(myMatrix, true);

  /* So we can see results, pause until a key is pressed. */
  unsigned char x;
  cin >> x;

  // Cleanup screen
  input.close();
  endCDK();
};
