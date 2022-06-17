#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <SFML/Graphics.hpp>
using namespace std;

/*
 * Документация для класса Database
 *
 * @brief Получение слова и его подсказки из файла
 *
 * @param filePath - путь к файлу со словами и подсказками
 *
 * В конструкторе класса происходит получение случайного
 * номера строки файла, указанного в filePath. Затем
 * происходит разделение подсказки и слова по последнему
 * пробелу в строке, так как формат хранения:
 * "Какая-то подсказка СЛОВО"
 * Благодаря методам returnWord и returnHint можно получить
 * слово и подсказку, соответственно.
 *
 * */
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

/*
 * Описание функции MessageInTheMiddle
 *
 * @brief Равномерно добавить к строке message пробелы слева и справа,
 * пока не будет достигнута (lineLength - 2) длина. В конце добавить
 * слева и справа "|"
 *
 * @param message - строка, которая будет оцентрирована с помощью пробелов
 * @param lineLength - длина нового сообщения с message в центре
 * @return строку вида: "|  message  |",
 *
 */
string MessageInTheMiddle(string message, int lineLength)
{
    for (int i = message.length(); i < lineLength - 2; i++)
    {
        if (i % 2 == 0) message = " " + message;
        else message = message + " ";
    }
    return "|" + message + "|";
}

/*
 * Описание функции PrintMessage
 *
 * @brief Отрисовка какого-либо контента в игре
 *
 * @param content - ЧТО будет отрисовано
 * @param printTop - если true, то будет отрисована верхняя горизонтальная линия
 * @param printBottom - если true, то будет отрисована нижняя горизонтальная линия
 * @return оцентрированный контент с/без верхней и/или нижней линии,
 * в зависимости от значений printTop и PrintBottom
 *
 */
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

/*
 * Описание функции PrintTitle
 *
 * @brief Отрисовка названия и псевдонима автора игры
 *
 * @param title - название игры (Hangman)
 * @param author - псевдоним автора (annanasnas)
 * @return две строки с title и author по центру
 * и с верхней и нижней горизонтальной линией
 *
 */
string PrintTitle(string title = "Hangman", string author = "annanasnas")
{
    string result;
    result += PrintMessage(title, true, false);
    result += PrintMessage("by " + author, false, true);
    return result;
}

/*
 * Описание функции DrawHangman
 *
 * @brief В зависимости от числа неправильных попыток,
 * будет отрисовываться сам hang man
 *
 * @param guessCount - число неправильных попыток
 * @return строка с нарисованным hang man
 *
 */
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

/*
 * Описание функции AvailableLetters
 *
 * @brief Отображение неиспользованных игроком букв
 *
 * @param wordFromUser - использованные игроком буквы
 * @param from - с какой буквы отображать
 * @param to - на какой букве остановить отображение
 * @return строку с буквами от from до to,
 * кроме тех, что есть в wordFromUser
 *
 */
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

/*
 * Описание функции PrintLetters
 *
 * @brief Отображение букв в английском алфавите,
 * которые доступны игроку для ввода
 *
 * @param wordFromUser - использованные игроком буквы
 * @return английский алфавит в виде двух оцентрированных
 * строк без букв, которые есть в wordFromUser
 *
 */
string PrintLetters(string wordFromUser)
{
    string result;
    result += PrintMessage("Available letters");
    result += PrintAvailableLetters(wordFromUser, 'A', 'M');
    result += PrintAvailableLetters(wordFromUser,'N', 'Z');
    return result;
}

/*
 * Описание функции PrintWordAndCheckWin
 *
 * @brief
 * 1) Отображение текущих отгаданных игроком
 * букв и подсказки к загаданному слову.
 * 2) Проверка на то, выиграл ли игрок
 *
 * @param wordToGuess - загаданное слово
 * @param wordFromUser - введенные игроком буквы
 * @param hint - подсказка к загаданному слову
 * @param won - если true, то игрок выиграл
 * @param result - отображаемое сообщение с
 * отгаданными буквами и подсказкой
 *
 * Проходимся циклом по каждой букве загаднного слова.
 * Если в буквах, введенных игроком, не находится буквы
 * из загаданного слова, переменная won становится равной
 * false, а к результату добавляется "_",
 * В противном случае, к результату добавляется правильно
 * отгаданная буква
 *
 */
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

/*
 * Описание функции TriesLeft
 *
 * @brief Подсчет числа попыток, сделанных игроком
 *
 * @param wordToGuess - загаданное слово
 * @param wordFromUser - введенные игроком буквы
 * @return число попыток, сделанных игроком
 *
 * Проходимся циклом по буквам, введенным игроком.
 * Если найдется буква, отсутствующая в загаданном слове,
 * игроку засчитывается +1 попытка.
 *
 */
int TriesLeft(string wordToGuess, string wordFromUser)
{
    int error = 0;
    for (int i = 0; i < wordFromUser.length(); i++)
    {
        if (wordToGuess.find(wordFromUser[i]) == string::npos) error++;
    }
    return error;
}

/*
 * Описание функции StartNewGame
 *
 * @brief старт новой игры
 *
 * @param tries - число попыток, сделанных пользователем
 * @param won - выиграл (true) / не выиграл (false)
 * @param UsersWordAndHint - строка с введенными игроком буквами и подсказкой
 * @param wordFromUser - введенные игроком буквы
 * @param wordAndHint - загаданное слово и подсказка
 *
 * Изменяем значения всех переменных как в начале игры
 *
 */
void StartNewGame(int &tries, bool &won, string &UsersWordAndHint, string &wordFromUser, Database &wordAndHint)
{
    wordAndHint = Database( "../words.txt");
    tries = 0;
    won = false;
    UsersWordAndHint = "";
    wordFromUser = "";
    PrintWordAndCheckWin(wordAndHint.returnWord(), wordFromUser, wordAndHint.returnHint(), won, UsersWordAndHint);
}

/*
 * Описание функции main
 *
 * @brief основная логика игры
 *
 */
int main()
{
    srand(time(0));
    string title = "Hangman";
    string author = "annanasnas";

    // инициализация основных для игры переменных
    Database wordAndHint;
    int tries = 0;
    bool won = false;
    string UsersWordAndHint;
    string wordFromUser;

    // подготовка интерфейса игры
    sf::RenderWindow window(sf::VideoMode(600, 780), "Hangman", sf::Style::Close | sf::Style::Titlebar);
    sf::Font font;
        font.loadFromFile("../consolas.ttf");
    sf::Text text("", font, 30);

    // отображение подсказки и числа букв, что нужно ввести пользователю
    PrintWordAndCheckWin(wordAndHint.returnWord(), wordFromUser, wordAndHint.returnHint(), won, UsersWordAndHint);

    while (window.isOpen())
    {
        char letterFromUser;
        sf::Event event;
        // обработчик событий
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::TextEntered)
            {
                // если введена буква
                if (event.text.unicode < 128)
                {
                    // пока пользователь не выиграл, можно вводить буквы
                    if (!won)
                    {
                        letterFromUser = toupper(static_cast<char>(event.text.unicode));
                        if (wordFromUser.find(letterFromUser) == string::npos) wordFromUser += letterFromUser;
                        tries = TriesLeft(wordAndHint.returnWord(), wordFromUser);
                        PrintWordAndCheckWin(wordAndHint.returnWord(), wordFromUser, wordAndHint.returnHint(), won, UsersWordAndHint);
                    }
                }
                // если пользователь выиграл, он должен нажать "enter" и начать новую игру
                if (event.text.unicode == 10)
                {
                    if (won) StartNewGame(tries, won, UsersWordAndHint, wordFromUser, wordAndHint);
                }
            }
        }
        // отрисовка основных компонентов игры
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
        text.setString(UsersWordAndHint);
        text.setPosition(0, 520);
        window.draw(text);
        sf::Text textSmall("", font, 25);
        textSmall.setColor(sf::Color(255, 69, 0));

        // проверка на выигрыш/проигрыш и отображение дальнейших указаний
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
        if (tries == 10) StartNewGame(tries, won, UsersWordAndHint, wordFromUser, wordAndHint);

        window.display();
    }

    return 0;
}
