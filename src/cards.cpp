#include <cards.h>

#define FMT_HEADER_ONLY
#include <fmt/core.h>
#include <fmt/color.h>
#include <fmt/chrono.h>

#include <filesystem>

namespace fs = std::filesystem;

struct Card {
    std::string q;
    std::string a;
};

// https://stackoverflow.com/a/1798170
std::string trim(const std::string& str, const std::string& whitespace = " \t") {
    const auto strBegin = str.find_first_not_of(whitespace);
    if (strBegin == std::string::npos)
        return ""; // Return nothing.

    const auto strEnd = str.find_last_not_of(whitespace);
    const auto strRange = strEnd - strBegin + 1;

    return str.substr(strBegin, strRange);
}

bool commentOpen = false;
bool isComment(const std::string& line) {
    //    if (line.rfind("/*", 0) == 0) commentOpen = true;
    //    if (line.rfind("*/", 0) == 0) commentOpen = false;

    // Handle multi-line comments.
    if (line.rfind("/*", 0) == 0) commentOpen = true;
    if (line.rfind("*/", 0) == 0)
    {
        commentOpen = false;
        return true;
    }

    if (commentOpen)
        return true;

    // Handle single line comments.
    if (line.rfind("//", 0) == 0)
        return true;

    // It is not a comment, return false.
    return false;
}

void parseCards(const std::string& filepath, std::vector<Card> &cards){
    // Open the file.
    std::ifstream file(filepath);

    // Failsafe.
    if (!file) {
        std::cerr << "File not found error: Study file was not found.\n";
        std::exit(1);
    }

    std::string currentLine;
    unsigned int lineNumber = 1;
    while (std::getline(file, currentLine)){
        // Trim leading and trailing spaces.
        currentLine = trim(currentLine);

        // Ignore the line if it is empty or a comment.
        if (currentLine.empty() || isComment(currentLine))
            continue;

        // Find the " : " delimiter.
        size_t position = currentLine.find(" : ");

        // If the line does not have a " : " delimiter, we print an error and exit.
        if (position == std::string::npos) {
            std::cerr << fmt::format("Parse error: line {} lacks a ' : ' delimiter. Add one and re-run.\n", lineNumber);
            std::exit(1);
        }

        // Get the question and answer from the line.
        std::string question, answer;
        question = currentLine.substr(0, position);
        answer = currentLine.substr(position + 3, currentLine.length() - 2);

        // Add the card to our cards.
        cards.push_back({question, answer});

        ++lineNumber;
    }
}

void startStudy(const std::string& filename) {
    // Parse the cards.
    std::vector<Card> cards;
    parseCards(filename, cards);

    // Shuffle the cards.
    std::random_device rd;
    std::default_random_engine engine = std::default_random_engine{rd()};
    std::shuffle(std::begin(cards), std::end(cards), engine);

    fmt::print("Answer the questions, and you will receive feedback.\n");

    // Count how many correct answers.
    int correctCount = 0;

    // Start measuring elapsed time.
    const auto timeBegin = std::chrono::system_clock::now();

    // Displays shuffled cards
    for (const auto& card : cards){
        // Print the question and a "> " for the query.
        fmt::print("{}\n> ", card.q);

        // Get the user's input.
        std::string userInput;
        std::getline(std::cin, userInput);

        // Lower the correct answer and user input.
        std::string answer = card.a;
        transform(answer.begin(), answer.end(), answer.begin(), ::tolower);
        transform(userInput.begin(), userInput.end(), userInput.begin(), ::tolower);

        // If the user input is the same as the answer.
        if (userInput == answer)
        {
            // The answer was correct.
            ++correctCount;
            fmt::print(fmt::emphasis::bold | fg(fmt::color::green), "Correct!\n\n");
        } else {
            // The answer was incorrect.
            fmt::print(fmt::emphasis::bold | fg(fmt::color::red), "Incorrect! ");
            fmt::print("The correct answer was {}.\n\n", card.a);
        }
    }

    // Stop measuring time elapsed.
    const auto timeEnd = std::chrono::system_clock::now();

    // Get the duration of study time.
    const int seconds = (int)std::chrono::duration_cast<std::chrono::seconds>(timeEnd - timeBegin).count();
    const int minutes = seconds / 60;
    const int hours = minutes / 60;

    // Don't add the label if it is less than 0.
    std::string durationText;

    if (hours > 0)
        durationText.append(fmt::format("{}h", hours));

    if (minutes > 0)
        durationText.append(fmt::format("{}m", minutes));

    if (seconds > 0)
        durationText.append(fmt::format("{}s", seconds));

    // Print statistics.
    fmt::print("Complete! A total of {} cards. Got {}/{} correct. Spent {} studying.\n", cards.size(), correctCount, cards.size(), durationText);
}
