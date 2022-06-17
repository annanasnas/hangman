#include <gtest/gtest.h>

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
int TriesLeft(string wordToGuess, string wordFromUser)
{
    int error = 0;
    for (int i = 0; i < wordFromUser.length(); i++)
    {
        if (wordToGuess.find(wordFromUser[i]) == string::npos) error++;
    }
    return error;
}

TEST(Tester, MessageInTheMiddleFunc) {
    EXPECT_EQ(MessageInTheMiddle("FIREWALL", 20), "|     FIREWALL     |");
    EXPECT_EQ(MessageInTheMiddle("FIREWALL", 3), "|FIREWALL|");
    EXPECT_EQ(MessageInTheMiddle("", 5), "|   |");
}

TEST(Tester, PrintMessageFunc) {
    EXPECT_EQ(PrintMessage("FIREWALL", false, false), "|             FIREWALL            |\n");
    EXPECT_EQ(PrintMessage("FIREWALL", true, false), "+---------------------------------+\n|             FIREWALL            |\n");
    EXPECT_EQ(PrintMessage("FIREWALL", false, true), "|             FIREWALL            |\n+---------------------------------+\n");
    EXPECT_EQ(PrintMessage("FIREWALL", true, true), "+---------------------------------+\n|             FIREWALL            |\n+---------------------------------+\n");
}

TEST(Tester, DrawHangmanFunc) {
    EXPECT_EQ(DrawHangman(0), "|                                 |\n|                                 |\n|                                 |\n|                                 |\n|                                 |\n|                                 |\n");
    EXPECT_EQ(DrawHangman(9), "|                |                |\n|                |                |\n|                O                |\n|               /|\\               |\n|                |                |\n|               / \\               |\n");
}

TEST(Tester, PrintAvailableLettersFunc) {
    EXPECT_EQ(PrintAvailableLetters("", 'A', 'C'), "|              A B C              |\n");
    EXPECT_EQ(PrintAvailableLetters("AB", 'A', 'C'), "|                  C              |\n");
    EXPECT_EQ(PrintAvailableLetters("ABC", 'A', 'C'), "|                                 |\n");
}

TEST(Tester, TriesLeftFunc) {
    EXPECT_EQ(TriesLeft("ABC", "C"), 0);
    EXPECT_EQ(TriesLeft("ABC", "CADGF"), 3);
    EXPECT_EQ(TriesLeft("ABC", "ABCABC"), 0);
}