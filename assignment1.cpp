/*
 * assignment1.cpp
 * Class: CS 2337.002
 * Student: Fawaz Ahmed
 * Professor: Don Vogel
 * This is a c++ application that can solve a crossword puzzle by giving it the puzzle and words to find in text files.
 * It can also output the position of the words and which direction the words are formed in the puzzle.
 */

#include <iostream>
#include <string>
#include <fstream>
#include <cstring>
#include <algorithm>
#include <cctype>
#include <vector>
using namespace std;

enum direction {LEFT_UP, RIGHT_LEFT, LEFT_DOWN, UP, DOWN, RIGHT_UP, LEFT_RIGHT, RIGHT_DOWN };	//directions

const int MAX = 50;	//max cols and rows
struct wordGame //struct wordGame
{
	int version = 2; //extracredit
	int numberRows;
	int numberColumns;
	char puzzle[MAX][MAX];
};

struct wordFind	//struct wordFind
{
	string word;
	bool found = false;
	int row;
	int column;
	direction where;
	int foundCount = 0;
	int numDirection;
};

bool readPuzzle(wordGame &game, string inputFileName) //readpuzzle bool
{
	ifstream openPuzzle;
	openPuzzle.open(inputFileName);	//opens file
	if (openPuzzle.is_open()) {
		openPuzzle >> game.numberRows >> game.numberColumns;	//gets number of rows and columns
		for (int x = 0; x < game.numberRows; x++)
		{
			for (int y = 0; y < game.numberColumns; y++)
			{
				openPuzzle >> game.puzzle[x][y];	//stores char in the 2d puzzle array
			}
		}
		openPuzzle.close();	//closes
		return true;
	}
	else 
	{
		return false;	//returns false if the file cannot open
	}
}
	
bool firstLetter(wordGame &game, wordFind &theFind, int currRow, int currCol)	//function that finds if the word exits by using direction
{
	int wordSize = theFind.word.length();	//size of word
	int xAxis[] = { -1, -1, -1, 0, 0, 1, 1, 1 };	//x axis directions
	int yAxis[] = { -1, 0, 1, -1, 1, -1, 0, 1 };	//y axis directions
	if (game.puzzle[currRow][currCol] != theFind.word[0])
	{
		return false;	//if the first letter is the not same as the current row and column char then stop searching
	}
	else //if true then search for rest of letters
	{
		for (int direction = 0; direction < 8; direction++)	//test in every direction
		{
			int rowBound = currRow + ((wordSize - 1) * yAxis[direction]);
			int colBound = currCol + ((wordSize - 1) * xAxis[direction]);	//sets bounds
			if (rowBound >= game.numberRows || rowBound < 0 || colBound >= game.numberColumns || colBound < 0)
			{
				continue;	//skip the direction if the word goes out of bounds
			}
			int rowMove = currRow + yAxis[direction];
			int colMove = currCol + xAxis[direction];
			int x;
			for (x = 1; x < wordSize; x++)
			{
				if (game.puzzle[rowMove][colMove] != theFind.word[x]) //checks in all directions around the first letter  if it is the same as the word
				{
					break;	//break for loop if the letter is not the same letter as in the word
				}
				rowMove += yAxis[direction], colMove += xAxis[direction];	//keeps checking if they are the same
			}
			if (x == wordSize)
			{
				theFind.numDirection = direction;	//store direction info
				return true;			
			}
		}
	}
	return false;
	theFind.found = false;	//return false if it is not found
}
void findWord(wordGame &game, wordFind &theFind)
{
	for (int currRow = 0; currRow < game.numberRows; currRow++)	
	{
		for (int currCol = 0; currCol < game.numberColumns; currCol++)
		{
			if (firstLetter(game, theFind, currRow, currCol)) //function with parameters with the two structs and current row and column
			{
				theFind.found = true;	//if true then store the following information
				theFind.row = currRow + 1;
				theFind.column = currCol + 1;
				theFind.foundCount++;
				switch (theFind.numDirection)	//enum direction based on the direction value
				{
				case 0:
					theFind.where = LEFT_UP;
						break;
				case 1:
					theFind.where = RIGHT_LEFT;
					break;
				case 2:
					theFind.where = LEFT_DOWN;
					break;
				case 3:
					theFind.where = UP;
					break;
				case 4:
					theFind.where = DOWN;
					break;
				case 5:
					theFind.where = RIGHT_UP;
					break;
				case 6:
					theFind.where = LEFT_RIGHT;
					break;
				case 7:
					theFind.where = RIGHT_DOWN;
					break;
				default:
					break;
				}
				theFind.numDirection = 0;	//resets direction num
			}
		}
	}
}
void displayPuzzle(wordGame &game)
{
	char upperCase;	//char to store uppercase
	for(int x = 0; x < game.numberRows; x++)
	{
		for(int y = 0; y < game.numberColumns; y++)
		{
			upperCase = toupper(game.puzzle[x][y]); //stores
			cout << upperCase;//outputs chars in uppercase
		}
		cout << endl;
	}

}

// function prototypes go here

int main()
{
	string puzzle, words;	//vars to store file names
	string tempword;	//var to temporarily store words from <words>.txt
	wordGame puzzleReader;
	ifstream testPuzzle;	//2 ifstreams that test if the file can open the other 2 open and are used to solve the assignment
	ifstream testWords;
	ifstream openWords;
	ifstream openPuzzle;
	cin >> puzzle >> words;	//input file names
	testPuzzle.open(puzzle);	//opens
	testWords.open(words);
	if (testPuzzle.is_open())
	{
		openPuzzle.open(puzzle);
		cout << "The puzzle from file \"" << puzzle << "\"" << endl;//if puzzle can open that read and display it
		readPuzzle(puzzleReader, puzzle);
	
		displayPuzzle(puzzleReader);
		if (testWords.is_open())
			{
				openWords.open(words);//if words file can open then start getting words
				while (openWords >> tempword)//while the txt file has words test the puzzle for each word
				{
					wordFind puzzleSolver;
					puzzleSolver.word = tempword;
					findWord(puzzleReader, puzzleSolver);//calls findword to test puzzle
					if (puzzleSolver.found)
					{
						if (puzzleSolver.foundCount > 1)//output if the word was found more than once
						{
							cout << "The word " << puzzleSolver.word << " was found " << puzzleSolver.foundCount << " times" << endl;
						}
						else
						{
							//enum direction { LEFT_UP, RIGHT_LEFT, LEFT_DOWN, UP, DOWN, RIGHT_UP, LEFT_RIGHT, RIGHT_DOWN };
							transform(puzzleSolver.word.begin(), puzzleSolver.word.end(), puzzleSolver.word.begin(), ::toupper); //uppercase the words
							cout << "The word " << puzzleSolver.word << " was found at (" << puzzleSolver.row << ", " << puzzleSolver.column << ") - "; //output the answer
							if (puzzleSolver.where == LEFT_UP)
							{
								cout << "left/up";
							}
							else if (puzzleSolver.where == LEFT_RIGHT)
							{
								cout << "right";
							}
							else if (puzzleSolver.where == LEFT_DOWN)
							{
								cout << "left/down";
							}
							else if (puzzleSolver.where == UP)
							{
								cout << "up";
							}
							else if (puzzleSolver.where == DOWN)
							{
								cout << "down";
							}
							else if (puzzleSolver.where == RIGHT_DOWN)
							{
								cout << "right/down";
							}
							else if (puzzleSolver.where == RIGHT_LEFT)
							{
								cout << "left";
							}
							else if (puzzleSolver.where == RIGHT_UP)
							{
								cout << "right/up"; //outputs the right words according to direction enum
							}
							cout << endl;
						}
					}
					else
					{
						transform(puzzleSolver.word.begin(), puzzleSolver.word.end(), puzzleSolver.word.begin(), ::toupper); //uppercase the words
						cout << "The word " << puzzleSolver.word << " was not found" << endl;// if word is not found.
					}
				}
			}
		else
		{
			cout << "The puzzle file \"" << words << "\" could not be opened or is invalid" << endl;//if word file cannot be opened
		}
	

	}
	else
	{
		cout << "The puzzle file \"" << puzzle << "\" could not be opened or is invalid" << endl;//if puzzle file cannot be open
	}
	testPuzzle.close();
	openPuzzle.close();//closese opened files
	testWords.close();
	openWords.close();
}
