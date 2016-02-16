//**********************************************************//
// Program 3 - Boggle										//
// Written By: Cardi DeMonaco Jr.							//
// Date: October 25, 2007 - November 14, 2007				//
// Updated: 2016											//
// Description:  This program mimmicks the game				//
//    Boggle.  A 4x4 board will be made for User			//
//    to find words in.  Words can be made by				//
//    connecting letters that surround the letter			//
//    that is being focused on.  When the user's time		//
//    is up, the computer will then find the words that		//
//    the user did not find.  If the user finds more		//
//    than half of the words, they will probably win		//
//    (depending on the points for the words).  Words		//
//    found must be more than two letters long.				//
//**********************************************************//

#include<string>
#include<vector>
#include<iostream>
#include<algorithm>
#include<fstream>
#include<time.h>
#include<stdlib.h> // srand, rand 
using namespace std;

//Global variables...
const int SQUARES = 4; // Number of lines of text needed for the game
const int MAXTIME = 60000; // Amount of time (in milliseconds?) that is allowed to input words
const int SIDES = 6; // Number of sides on a Die
vector<string> dict; // Dictionary
vector<string> user; // Holds User words
vector<string> puter; // Holds Computer words

//Functions...
void dictionary(); // Makes the Dictionary to search words for in
void makeBoard(char board[SQUARES][SQUARES]); // Makes the Board
void printBoard(const char board[SQUARES][SQUARES]); // Prints the Board
char copyBoard(const char[SQUARES][SQUARES], char[SQUARES][SQUARES]); // Copies Board
int checkWord(int, int, const string); // Finds a word in the dictionary
void puterWords(char board[SQUARES][SQUARES], int x, int y, string word); // Finds computer words
void checkPuterWord(string word); // Checks if a word found by computer has not already been used
void score(); // Computes score for User and Computer

int main()
{
	cout << "//////// BOGGLE ////////" << endl << endl;

	//Put Boggle words into a vector...
	dictionary();

	//Make the Boggle board...
	char board[SQUARES][SQUARES]; // Board
	makeBoard(board); // Create the Boggle Board using the cubes.txt file

	//Print the Boggle board so the User can see it...
	printBoard(board);

	//User can enter words until time is up...
	string word; //Word input by user
	long now = clock(); // Clock for timing user

	while (now + MAXTIME  > clock())
	{
		cout << "Enter word: ";
		cin >> word;
		for (int x = 0; x<word.size(); x++)
		{
			//Fix case of the word that the user put in...
			if ((word[x] >= 65) && (word[x] <= 96))
			{
				word[x] = word[x] + 32;
			}

		}
		user.push_back(word);
	}



	cout << "TIME IS UP." << endl << endl;

	//Computer finds ALL words...
	for (int x = 0; x<4; x++)
	{
		for (int y = 0; y<4; y++)
		{
			char c(board[x][y]);
			string s(1, c); // s now has board[x][y]
			puterWords(board, x, y, s);
		}
	}

	cout << "////////////////////////" << endl << endl;

	score();

	cout << endl << "////////////////////////" << endl << endl;

	return 0;
}

//******************************************************************************************************//

//Put all Boggle words into a Vector...
void dictionary()
{
	//Pre-conditions: Dictionary is empty
	//Post-conditions: Dictionary is full, using bogwords.txt

	//Populate dictionary vector...
	string filename;
	ifstream input;
	string wd;

	//Open file...
	filename = "bogwords.txt";
	input.open(filename.c_str());

	//Put all Boggle words into a vector...
	for (int i = 0; i<19911; i++)
	{
		input >> wd;
		dict.push_back(wd);
	}
}

//******************************************************************************************************//

//Make the board...
void makeBoard(char board[SQUARES][SQUARES])
{
	//Pre-condition: The Array that is passed is of dimensions 4x4 and type char with only letters of the alphabet
	//Post-condition: A 4x4 array that is filled with letters from cubes.txt

	//Open input file...
	string filename = "cubes.txt"; //Filename that contains the Boggle cubes
	ifstream input; //Input File stream
	input.open(filename.c_str());

	//Insert items onto Board (one for each row in the input file)...
	string currentCube; //Current Cube of the cubes.txt file
	srand(time(NULL)); 
	for (int i = 0; i<SQUARES; i++)
	{
		for (int j = 0; j<SQUARES; j++)
		{
			//currentCube will be the whole row of the sides of the die (6 characters)...
			input >> currentCube;

			//Pick the letter that will be used for this location on the Board...
			char selectedSide = currentCube[rand() % SIDES]; 
			//cout << "rand(): " << rand() << " | ";
			//cout << "rand() % SIDES:" << rand() % SIDES << " | ";
			board[i][j] = selectedSide;
		}
	}

	//Shuffle the Individual Cubes themselves around
	char tempItem1;
	char tempItem2;
	int a;
	int b;

	srand(clock());
	for (int i = 0; i<SQUARES; i++)
	{
		for (int j = 0; j<SQUARES; j++)
		{
			a = rand() % SQUARES;
			b = rand() % SQUARES;

			tempItem1 = board[a][b];
			tempItem2 = board[i][j];
			board[i][j] = tempItem1;
			board[a][b] = tempItem2;
		}
	}

	// Or use the following for a "default" Board...
	/*
	board[0][0] = 'i';
	board[0][1] = 'p';
	board[0][2] = 'o';
	board[0][3] = 's';
	board[1][0] = 'l';
	board[1][1] = 'o';
	board[1][2] = 'h';
	board[1][3] = 'v';
	board[2][0] = 'o';
	board[2][1] = 'r';
	board[2][2] = 'm';
	board[2][3] = 'a';
	board[3][0] = 't';
	board[3][1] = 'o';
	board[3][2] = 'i';
	board[3][3] = 'n';
	*/
}

//******************************************************************************************************//

void printBoard(const char board[SQUARES][SQUARES]) // prints the board
{
	//Pre-conditions:
	//Post-conditions:

	for (int i = 0; i<SQUARES; i++)
	{
		cout << "        ";
		for (int j = 0; j<SQUARES; j++)
		{
			cout << board[i][j] << " ";
		}
		cout << endl << endl;
	}
	cout << "////////////////////////" << endl << endl;
}

//******************************************************************************************************//

//Copy the board...
char copyBoard(const char board[SQUARES][SQUARES], char tempBoard[SQUARES][SQUARES])
{
	//Pre-conditions:
	//Post-conditions:

	for (int r = 0; r<SQUARES; r++)
	{
		for (int c = 0; c<SQUARES; c++)
		{
			tempBoard[r][c] = board[r][c];
		}
	}
	return tempBoard[SQUARES][SQUARES];
}

//******************************************************************************************************//

//Searches in dictionary for word, returns position that it is found at...
int checkWord(int low, int high, const string word)
{
	//Pre-conditions:
	//Post-conditions:

	int mid;

	if (low > high)
	{
		return -1;
	}
	else
	{
		mid = (low + high) / 2;
		if (word == dict[mid])
		{
			//Returns position at which the value is found...
			return mid;
		}
		//If word is not at mid, and is below mid...
		else if (dict[mid] > word)
		{
			return checkWord(low, mid - 1, word);
		}
		//If word is not at mid, and is above mid...
		else
		{
			return checkWord(mid + 1, high, word);
		}
	}
}

//******************************************************************************************************//

void puterWords(char board[SQUARES][SQUARES], int x, int y, string word) //Computer finds the rest of the words
{
	//Pre-conditions:
	//Post-conditions:

	string tempWord = word;
	int max = 7; // Maximum length of a word that can be found

	char tempBoard[SQUARES][SQUARES];
	copyBoard(board, tempBoard);

	//Make the current item not accessible again...
	board[x][y] = 0;

	//Top left...
	if (x - 1 >= 0 && y - 1 >= 0 && word.length() <= max && board[x - 1][y - 1] != 0)
	{
		word = word + board[x - 1][y - 1];
		//cout << word << endl;
		if (word.length() >= 3)
		{
			if (checkWord(0, 19911, word) != -1)
			{
				checkPuterWord(word);
			}
		}
		puterWords(board, x - 1, y - 1, word);
		word = tempWord;
	}
	//Top middle...
	if (x - 1 >= 0 && word.length() <= max && board[x - 1][y] != 0)
	{
		word = word + board[x - 1][y];
		//cout << word << endl;
		if (word.length() >= 3)
		{
			if (checkWord(0, 19911, word) != -1)
			{
				checkPuterWord(word);
			}
		}
		puterWords(board, x - 1, y, word);
		word = tempWord;
	}
	//Top right...
	if (x - 1 >= 0 && y + 1 < 4 && word.length() <= max && board[x - 1][y + 1] != 0)
	{
		word = word + board[x - 1][y + 1];
		//cout << word << endl;
		if (word.length() >= 3)
		{
			if (checkWord(0, 19911, word) != -1)
			{
				checkPuterWord(word);
			}
		}
		puterWords(board, x - 1, y + 1, word);
		word = tempWord;
	}
	//Middle left...
	if (y - 1 >= 0 && word.length() <= max && board[x][y - 1] != 0)
	{
		word = word + board[x][y - 1];
		//cout << word << endl;
		if (word.length() >= 3)
		{
			if (checkWord(0, 19911, word) != -1)
			{
				checkPuterWord(word);
			}
		}
		puterWords(board, x, y - 1, word);
		word = tempWord;
	}
	//Middle right...
	if (y + 1 < 4 && word.length() <= max && board[x][y + 1] != 0)
	{
		word = word + board[x][y + 1];
		//cout << word << endl;
		if (word.length() >= 3)
		{
			if (checkWord(0, 19911, word) != -1)
			{
				checkPuterWord(word);
			}
		}
		puterWords(board, x, y + 1, word);
		word = tempWord;
	}
	//Bottom left...
	if (x + 1 < 4 && y - 1 >= 0 && word.length() <= max && board[x + 1][y - 1] != 0)
	{
		word = word + board[x + 1][y - 1];
		//cout << word << endl;
		if (word.length() >= 3)
		{
			if (checkWord(0, 19911, word) != -1)
			{
				checkPuterWord(word);
			}
		}
		puterWords(board, x + 1, y - 1, word);
		word = tempWord;
	}
	//Bottom middle...
	if (x + 1 < 4 && word.length() <= max && board[x + 1][y] != 0)
	{
		word = word + board[x + 1][y];
		//cout << word << endl;
		if (word.length() >= 3)
		{
			if (checkWord(0, 19911, word) != -1)
			{
				checkPuterWord(word);
			}
		}
		puterWords(board, x + 1, y, word);
		word = tempWord;
	}
	//Bottom right...
	if (x + 1 < 4 && y + 1 < 4 && word.length() <= max && board[x + 1][y + 1] != 0)
	{
		word = word + board[x + 1][y + 1];
		//cout << word << endl;
		if (word.length() >= 3)
		{
			if (checkWord(0, 19911, word) != -1)
			{
				checkPuterWord(word);
			}
		}
		puterWords(board, x + 1, y + 1, word);
		word = tempWord;
	}
	copyBoard(tempBoard, board);
}

//******************************************************************************************************//

//Check to see if computer word has already been used...
void checkPuterWord(string word)
{
	//Pre-conditions:
	//Post-conditions:

	bool found = false;

	//Go through computer's list...
	for (int i = 0; i<puter.size(); i++)
	{
		//If found in Computer's list already...
		if (word == puter[i])
		{
			found = true;
		}
	}

	//If found, do not add to list...
	if (found == false)
	{
		puter.push_back(word);
	}
}

//******************************************************************************************************//

void score() //Computes Player and Computer's score

{

	//Pre-conditions:

	//Post-conditions:



	cout << "YOUR WORDS: " << endl;

	int uTotal = 0;

	for (int i = 0; i<user.size(); i++)

	{

		//Check if User's word is in Computer's list...

		bool foundInPuterList = false;

		for (int j = 0; j<puter.size(); j++)

		{

			if (user[i] == puter[j])

			{

				foundInPuterList = true;

			}

		}



		//If found in the Computer's list...

		if (foundInPuterList == true)

		{

			//Check if word has already been used...

			bool foundInOwnList = false;

			for (int j = 0; j <= i; j++)

			{

				if (user[i] == user[j] && i != j)

				{

					foundInOwnList = true;

				}

			}



			//If already used...

			if (foundInOwnList == true)

			{

				cout << user[i] << ": " << "Already Used" << endl;

			}

			//If word is found in dictionary, give points...

			else

			{

				cout << user[i] << ": " << user[i].size() - 2 << endl;

				uTotal = uTotal + (user[i].size() - 2);

			}

		}

		else

		{

			cout << user[i] << ": " << "Illegal" << endl;

		}

	}

	cout << "TOTAL: " << uTotal << endl;



	cout << endl;



	//List all words that the computer found

	cout << "PUTER'S WORDS:" << endl;

	int pTotal = 0;

	for (int i = 0; i<puter.size(); i++)

	{

		bool found = false;



		//Go through user's list...

		for (int j = 0; j<user.size(); j++)

		{

			//If found in User's list...

			if (puter[i] == user[j])

			{

				found = true;

			}

		}

		if (found == false)

		{

			cout << puter[i] << ": " << puter[i].size() - 2 << endl;

			pTotal = pTotal + (puter[i].size() - 2);
		}
	}
	cout << "TOTAL: " << pTotal << endl;
}


//******************************************************************************************************//