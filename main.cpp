#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
using namespace std;

class Word
{
    string wordToGuess;
public:
    explicit Word(string filePath)
    {
        string word;
        vector<string> v;
        ifstream reader(filePath);
        if (reader.is_open())
        {
            while (std::getline(reader, word)) v.push_back(word);
            int randomLine = rand() % v.size();
            wordToGuess = v.at(randomLine);
            reader.close();
        }
    }
    string& returnWord()
    {
        return wordToGuess;
    }
};

string MessageInTheMiddle(string message, int lineLength)
{
    for (int i = message.length(); i < lineLength - 2; i++)
    {
        if (i % 2 == 0) message = " " + message;
        else message = message + " ";
    }
    return "|" + message + "|";
}

void PrintMessage(string content, bool printTop = true, bool printBottom = true)
{
    string line = "+---------------------------------+";
    if (printTop) cout << line << endl;
    cout << MessageInTheMiddle(content, line.length()) << endl;
    if (printBottom) cout << line << endl;
}

void PrintTitle(string title, string author)
{
    PrintMessage(title, true, false);
    PrintMessage("by " + author, false, true);
}

void DrawHangman(int guessCount)
{
    if (guessCount >= 1)
        PrintMessage("|", false, false);
    else
        PrintMessage("", false, false);

    if (guessCount >= 2)
        PrintMessage("|", false, false);
    else
        PrintMessage("", false, false);

    if (guessCount >= 3)
        PrintMessage("O", false, false);
    else
        PrintMessage("", false, false);

    if (guessCount == 4)
        PrintMessage("/  ", false, false);

    if (guessCount == 5)
        PrintMessage("/| ", false, false);

    if (guessCount >= 6)
        PrintMessage("/|\\", false, false);
    else
        PrintMessage("", false, false);

    if (guessCount >= 7)
        PrintMessage("|", false, false);
    else
        PrintMessage("", false, false);

    if (guessCount == 8)
        PrintMessage("/", false, false);

    if (guessCount >= 9)
        PrintMessage("/ \\", false, false);
    else
        PrintMessage("", false, false);
}

void PrintAvailableLetters(string wordFromUser, char from, char to)
{
    string availableLetters;
    for (char i = from; i <= to; i++)
    {
        if (wordFromUser.find(i) == string::npos)
        {
            availableLetters += i;
            availableLetters += " ";
        }
        else availableLetters += "  ";
    }
    PrintMessage(availableLetters, false, false);
}

void PrintLetters(string wordFromUser)
{
    PrintMessage("Available letters");
    PrintAvailableLetters(wordFromUser, 'A', 'M');
    PrintAvailableLetters(wordFromUser,'N', 'Z');
}

bool PrintWordAndCheckWin(string wordToGuess, string wordFromUser)
{
    bool won = true;
    string wordDisplayed;
    for (int i = 0; i < wordToGuess.length(); i++)
    {
        if (wordFromUser.find(wordToGuess[i]) == string::npos)
        {
            won = false;
            wordDisplayed += "_ ";
        }
        else
        {
            wordDisplayed += wordToGuess[i];
            wordDisplayed += " ";
        }
    }
    PrintMessage("Guess the word");
    PrintMessage(wordDisplayed, false);
    return won;
}

int TriesLeft(string wordToGuess, string wordFromUser)
{
    int error = 0;
    for (int i = 0; i < wordFromUser.length(); i++)
    {
        if (wordToGuess.find(wordFromUser[i]) == string::npos) error++;
    }
    return error;
}

int main() {
    srand(time(0));
    string title = "Hangman";
    string author = "annanasnas";
    Word wordToGuess = Word( "/Users/anastasia/CLionProjects/hangman/words.txt");
    int tries = 0;
    bool won = false;
    string wordFromUser;
    do {
        system("clear");
        PrintTitle(title, author);
        DrawHangman(tries);
        PrintLetters(wordFromUser);
        if (PrintWordAndCheckWin(wordToGuess.returnWord(), wordFromUser)) won = true;
        if (won) break;
        char letterFromUser;
        cout << ">";
        cin >> letterFromUser;
        if (wordFromUser.find(letterFromUser) == string::npos) wordFromUser += letterFromUser;
        tries = TriesLeft(wordToGuess.returnWord(), wordFromUser);
    } while(tries < 10);
    if (won) PrintMessage("You won!", false);
    else PrintMessage("Game over");
    getchar();
    return 0;
}
