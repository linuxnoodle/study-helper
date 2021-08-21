#include <vector>
#include <filesystem>
#include <cards.h>

#define FMT_HEADER_ONLY
#include <fmt/core.h>

#ifdef WIN32
#include <Windows.h>
#endif

namespace fs = std::filesystem;

struct StudyFile {
    std::string fileName;
    std::string fullPath;
    int id;
};

std::vector<StudyFile> studyFiles;

// https://stackoverflow.com/a/16826908
int fast_atoi(const char *str)
{
    int val = 0;
    while(*str) {
        val = val*10 + (*str++ - '0');
    }
    return val;
}

StudyFile getDesiredStudy() {
    fmt::print("Choose a study:\n");

    // Print the studies.
    for (StudyFile &file : studyFiles)
        fmt::print("\t{}. {}\n", file.id, file.fileName);

    fmt::print("Enter q to exit.\n");

    fmt::print("> ");

    // Get the desired study id.
    std::string desiredIdString;
    std::getline(std::cin, desiredIdString);

    // If the line was to exit, exit.
    if (desiredIdString == "q")
    {
        fmt::print("Exiting.");
        exit(1);
    }

    // Otherwise, atoi the id.
    int desiredId = fast_atoi(desiredIdString.c_str());

    // Find the id relating to the study.
    for (StudyFile study : studyFiles) {
        // If the study id is the desired one, return it.
        if (study.id == desiredId)
        {
            fmt::print("Starting {} study.\n", study.fileName);
            return study;
        }
    }

    // If we did not return a study, request a different study from the user.
    fmt::print("Could not find study {}.\n", desiredId);
    return getDesiredStudy();
}

int main() {
// Enable ANSI escape codes on Windows.
#if WIN32
    // Get the std output handle.
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    // Get the current output mode.
    DWORD dwOriginalOutMode = 0;
    GetConsoleMode(hOut, &dwOriginalOutMode);

    // Set the console mode to include ENABLE_VIRTUAL_TERMINAL_PROCESSING.
    SetConsoleMode(hOut, dwOriginalOutMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
#endif

    // Get the current fullPath and replace \ with /.
    std::string currentPath = fs::current_path().string();
    std::replace(currentPath.begin(), currentPath.end(), '\\', '/');

    // The cards directory.
    std::string path = fmt::format("{}/{}", currentPath, "cards");

    // If the cards directory does not exist, we exit.
    if (!fs::exists(path))
    {
        fmt::print("Failed to find a directory at {}, Make directory and re-run!\n", path);
        exit(-1);
    }

    int id = 1;
    for (const auto & file : fs::directory_iterator(path))
    {
        // Skip if the file is actually a directory.
        if (file.is_directory())
            continue;

        std::string extension = file.path().extension().string();

        // Skip if the file extension is not ".sdy" or there is no extension.
        if (extension.empty() || extension != ".sdy")
            continue;

        // Get the full fullPath and raw filename without an extension from the file.
        std::string fileName = file.path().stem().string();
        std::string fullPath = file.path().string();

        // Add the file to our vector.
        studyFiles.push_back({fileName, fullPath, id});

        // Increase the id by one.
        ++id;
    }

    // Get the desired study.
    StudyFile desired = getDesiredStudy();

    // Start the study.
    startStudy(desired.fullPath);

    // End
    return 0;
}
