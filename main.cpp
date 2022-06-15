#include <iostream>
using namespace std;

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

void PrintAvailableLetters(string lettersInWord, char from, char to)
{
    string availableLetters;
    for (char i = from; i <= to; i++)
    {
        if (lettersInWord.find(i) == string::npos)
        {
            availableLetters += i;
            availableLetters += " ";
        }
        else availableLetters += "  ";
    }
    PrintMessage(availableLetters, false, false);
}

void PrintLetters(string word)
{
    PrintMessage("Available letters");
    PrintAvailableLetters(word, 'A', 'M');
    PrintAvailableLetters(word,'N', 'Z');
}

bool PrintWordAndCheckWin(string word, string attempts)
{
    bool won = true;
    string wordDisplayed;
    for (int i = 0; i < word.length(); i++)
    {
        if (attempts.find(word[i]) == string::npos)
        {
            won = false;
            wordDisplayed += "_ ";
        }
        else
        {
            wordDisplayed += word[i];
            wordDisplayed += " ";
        }
    }
    PrintMessage("Guess the word");
    PrintMessage(wordDisplayed, false);
    return won;
}

int main() {
    string title = "Hangman";
    string author = "annanasnas";
    string attempts = "AIEN";
    PrintTitle(title, author);
    DrawHangman(9);
    PrintLetters(attempts);
    PrintWordAndCheckWin("MIEM", attempts);
    getchar();
    return 0;
}
