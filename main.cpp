#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <SFML/Graphics.hpp>
using namespace std;


class Database
{
    string wordToGuess;
    string hint;
public:
    Database(string filePath)
    {
        string word;
        vector<string> v;
        ifstream reader(filePath);
        if (reader.is_open())
        {
            while (std::getline(reader, word)) v.push_back(word);
            int randomLine = rand() % v.size();
            string wordAndHint = v.at(randomLine);
            int index = 0;
            for(int i = wordAndHint.length() - 1; i >= 0; --i) {
                if (wordAndHint[i] == ' ') {
                    index = i;
                    break;
                }
            }
            hint = wordAndHint.substr(0, index);
            wordToGuess = wordAndHint.erase(0,wordAndHint.find_last_of(' ')+1);
            reader.close();
        }
    }
    string& returnWord()
    {
        return wordToGuess;
    }
    string& returnHint()
    {
        return hint;
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

string PrintMessage(string content, bool printTop = true, bool printBottom = true)
{
    string result;
    string line = "+---------------------------------+";
    if (printTop)
    {
        result += line;
        result += "\n";
    }
    result += MessageInTheMiddle(content, line.length());
    result += "\n";
    if (printBottom)
    {
        result += line;
        result += "\n";
    }
    return result;
}

string PrintTitle(string title, string author)
{
    string result;
    result += PrintMessage(title, true, false);
    result += PrintMessage("by " + author, false, true);
    return result;
}

string DrawHangman(int guessCount)
{
    string result;
    if (guessCount >= 1)
        result += PrintMessage("|", false, false);
    else
        result += PrintMessage("", false, false);

    if (guessCount >= 2)
        result += PrintMessage("|", false, false);
    else
        result += PrintMessage("", false, false);

    if (guessCount >= 3)
        result += PrintMessage("O", false, false);
    else
        result += PrintMessage("", false, false);

    if (guessCount == 4)
        result += PrintMessage("/  ", false, false);

    if (guessCount == 5)
        result += PrintMessage("/| ", false, false);

    if (guessCount >= 6)
        result += PrintMessage("/|\\", false, false);
    else
        result += PrintMessage("", false, false);

    if (guessCount >= 7)
        result += PrintMessage("|", false, false);
    else
        result += PrintMessage("", false, false);

    if (guessCount == 8)
        result += PrintMessage("/  ", false, false);

    if (guessCount >= 9)
        result += PrintMessage("/ \\", false, false);
    else
        result += PrintMessage("", false, false);

    return result;
}

string PrintAvailableLetters(string wordFromUser, char from, char to)
{
    string result;
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
    result += PrintMessage(availableLetters, false, false);
    return result;
}

string PrintLetters(string wordFromUser)
{
    string result;
    result += PrintMessage("Available letters");
    result += PrintAvailableLetters(wordFromUser, 'A', 'M');
    result += PrintAvailableLetters(wordFromUser,'N', 'Z');
    return result;
}

void PrintWordAndCheckWin(string wordToGuess, string wordFromUser, string hint, bool &won, string &result)
{
    bool realWon = true;
    string wordDisplayed;
    result = "";
    for (int i = 0; i < wordToGuess.length(); i++)
    {
        if (wordFromUser.find(wordToGuess[i]) == string::npos)
        {
            wordDisplayed += "_ ";
            realWon = false;
        }
        else
        {
            wordDisplayed += wordToGuess[i];
            wordDisplayed += " ";
        }
    }
    result += PrintMessage("Hint", true, false);
    result += PrintMessage(hint, false, true);
    result += PrintMessage(wordDisplayed, false);
    won = realWon;
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

void StartNewGame(int &tries, bool &won, string &result, string &wordFromUser, Database &wordAndHint)
{
    wordAndHint = Database( "/Users/anastasia/CLionProjects/hangman/words.txt");
    tries = 0;
    won = false;
    result = "";
    wordFromUser = "";
    PrintWordAndCheckWin(wordAndHint.returnWord(), wordFromUser, wordAndHint.returnHint(), won, result);
}

int main()
{
    srand(time(0));
    string title = "Hangman";
    string author = "annanasnas";
    Database wordAndHint = Database( "../words.txt");
    int tries = 0;
    bool won = false;
    string result;
    string wordFromUser;

    sf::RenderWindow window(sf::VideoMode(600, 780), "Hangman", sf::Style::Close | sf::Style::Titlebar);
    sf::Font font;
        font.loadFromFile("../consolas.ttf");
    sf::Text text("", font, 30);

    PrintWordAndCheckWin(wordAndHint.returnWord(), wordFromUser, wordAndHint.returnHint(), won, result);

    while (window.isOpen())
    {
        char letterFromUser;
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::TextEntered)
            {
                if (event.text.unicode < 128)
                {
                    if (!won)
                    {
                        letterFromUser = toupper(static_cast<char>(event.text.unicode));
                        if (wordFromUser.find(letterFromUser) == string::npos) wordFromUser += letterFromUser;
                        tries = TriesLeft(wordAndHint.returnWord(), wordFromUser);
                        PrintWordAndCheckWin(wordAndHint.returnWord(), wordFromUser, wordAndHint.returnHint(), won, result);
                    }
                }
                if (event.text.unicode == 10)
                {
                    if (won) StartNewGame(tries, won, result, wordFromUser, wordAndHint);
                }
            }
        }
        window.clear();
        text.setString(PrintTitle(title, author));
        text.setPosition(0, 0);
        window.draw(text);
        text.setString(DrawHangman(tries));
        text.setPosition(0, 140);
        window.draw(text);
        text.setString(PrintLetters(wordFromUser));
        text.setPosition(0, 350);
        window.draw(text);
        text.setString(result);
        text.setPosition(0, 520);
        window.draw(text);

        sf::Text textSmall("", font, 25);
        textSmall.setColor(sf::Color(255, 69, 0));

        if (won)
        {
            textSmall.setString("Press enter to start new game...");
            textSmall.setPosition(0, 730);
            window.draw(textSmall);
        }
        if (tries == 9)
        {
            textSmall.setString("Your last attempt");
            textSmall.setPosition(0, 730);
            window.draw(textSmall);
        }
        if (tries == 10) StartNewGame(tries, won, result, wordFromUser, wordAndHint);

        window.display();
    }

    return 0;
}
