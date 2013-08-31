#include <stdio.h>

#define empty    1
#define computer 3
#define opponent 5

int rowMultiplication[3]={1,1,1},
    columnMultiplication[3]={1,1,1},
    diagonal=1,
    reverseDiagonal=1;

enum Check{Row, Column, Diagonal, ReverseDiagonal};

  void getInput(int* j) {
    char input[9];
    scanf("%c%c%c\n%c%c%c\n%c%c%c", input, input+1, 
					input+2, input+3, input+4, input+5, input+6, input+7, input+8);
    for(int i=0; i<9; i++) {
      if(input[i] == 'X')
        j[i] = empty;
      else if(input[i] == 'C')
        j[i] = computer;
      else if(input[i] == 'O')
        j[i] = opponent;
    }
  }

	void calcMultiplications(const int* input) {
    //Rows
		for(int i=0; i<3; i++) {
      for(int j=0; j<3; j++) {
        rowMultiplication[i] *= input[(i*3)+j];
      }
    }
    //Columns
		for(int i=0; i<3; i++) {
      for(int j=0; j<3 * 3; j=j+3) {
        columnMultiplication[i] *= input[i+j];
      }
    }

    //Diagonal
    for(int i=0; i< 3 * 3; i=i+4) {
      diagonal *= input[i];
    }

    //ReverseDiagonal
    for(int i=2; i<3 * 3; i=i+2) {
	    reverseDiagonal *= input[i];
    }
  }

  int checkWinFor(const int* input, int player) {
    //rowMultiplication
    for(int i=0; i<3; i++) {
			if(rowMultiplication[i]/player == player) {
				for(int j=i*3; j< (i*3)+3; j++) {
					if(input[j] == empty)
            return j;
				}
			}
    }

		//columnMultiplication
    for(int i=0; i<3; i++) {
			if(columnMultiplication[i]/player == player) {
				for(int j=i; j< 3 * 3; j=j+3) {
					if(input[j] == empty)
            return j;
				}
			}
    }
    
 		//Diagonal
    if(diagonal/player == player) {
			for(int i=0; i< 3 * 3; i=i+4) {
				if(input[i] == empty)
        	return i;
			}
		}

		//reverseDiagonal
    if(reverseDiagonal/player == player) {
			for(int i=2; i< 3 * 3; i=i+2) {
				if(input[i] == empty) 
        	return i;
			}
		}    
		return -1;
  }

  int checkOwnWin(const int* input) {
    return checkWinFor(input, computer);
  }

  int stopOpponentWin(const int* input) {
    return checkWinFor(input, opponent);
  }

  int applyStrategy(const int* input) {
    for(int i=0; i<3; i+=2) {
			if(rowMultiplication[0]%opponent != 0 && 
         columnMultiplication[i]%opponent != 0 && input[i] != computer) {
				if(input[2-i] != computer)
					return 2-i;
				else if(input[i+6] != computer)
					return i+6;
				else
					return i;
			}
		}
		for(int i=0; i<3; i+=2) {
			if(rowMultiplication[2]%opponent != 0 && 
         columnMultiplication[i]%opponent != 0 && input[i+6] != computer) {
        if(input[i] != computer)
          return i;
        else if(input[8-i] != computer)
          return i+6;
        else
          return i;
      }
		}
    if(input[4] == empty) {
      for(int i=0, j=i+6; i<18; i+=6, j=(i+3)%9) {
				if( input[i] == computer && input[i+2] == computer && 
            input[j] == empty && input[j+2] == empty)
					return 4;
      }
      for(int i=0, j=i+2; i<4; i+=2, j=(i+6+1)%9) {
				if( input[i] == computer && input[i+6] == computer &&
 						input[j] == empty && input[j+6] == empty)
					return 4;
			}
		}
    return -1;
  }

  int blockStrategy(const int* input) {
		if(input[4] == computer) {
      if(rowMultiplication[0] == opponent && rowMultiplication[2] == opponent &&
 				 columnMultiplication[0] == opponent && columnMultiplication[2] == opponent) {
				if((input[0] == opponent && input[8] == opponent) ||
					 (input[2] == opponent && input[6] == opponent) )
					return 1;
			}
		}  		
  	return -1;
  }

  int center(const int* input) {
    if(input[4] == empty)
      return 4;
    return -1;
  }

  int oppositeCorner(const int* input) {
    if(input[0] == opponent && input[8] == empty)
      return 8;
    else if(input[2] == opponent && input[6] == empty)
      return 6;
    else if(input[6] == opponent && input[2] == empty)
      return 2;
    else if(input[8] == opponent && input[0] == empty) 
      return 0;
    return -1;
  } 

	int emptyCorner(const int* input) {
    for(int j=0; j<9; j=j+2) {
      if(input[j] == empty)
        return j;
    }
    return -1;
  }

  int emptySide(const int* input) {
    for(int j=1; j<9; j=j+2) {
      if(input[j] == empty)
        return j;
    }
    return -1;
  }

  void printOutput(const int* out) {
    for(int i=0; i<9; i++) {
      if(out[i] == empty)
        printf("%c", 'X');
      else if(out[i] == computer)
        printf("%c", 'C');
      else if(out[i] == opponent)
        printf("%c", 'O');
      if((i+1)%3 == 0)
        printf("\n");
    }
  }

	int getNextMove(const int* input) {
    int nextMove;
    return (nextMove = checkOwnWin(input)) != -1 ? nextMove :
           (nextMove = stopOpponentWin(input)) != -1 ? nextMove :
           (nextMove = applyStrategy(input)) != -1 ? nextMove :
           (nextMove = blockStrategy(input)) != -1 ? nextMove :
           (nextMove = center(input)) != -1 ? nextMove :
           (nextMove = oppositeCorner(input)) != -1 ? nextMove :
           (nextMove = emptyCorner(input)) != -1 ? nextMove :
           (nextMove = emptyCorner(input)) != -1 ? nextMove : -1;
	}

  int main() {
    int input[9];
    getInput(input);
    calcMultiplications(input);
    int totalMultiplication = 1, nextMove;
    //Checking if first move
    for(int i=0; i<3 ; i++)
      totalMultiplication *= rowMultiplication[i];
    if(totalMultiplication == 1)
      nextMove = 0;
    else
      nextMove = getNextMove(input);
    if(nextMove != -1)
    	input[nextMove] = computer;
    
    printOutput(input);
    return 0;
  }
