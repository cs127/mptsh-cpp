/*
    OpenMPT ANSI Syntax Highlighting (C++ version)
    original code by cs127 @ https://cs127.github.io
    C++ port by Sightem @ https://github.com/Sightem
    https://github.com/cs127/mptsh-cpp

    version 0.0.1, (port of Java version 0.2.4)
    2022-11-30

    Contributors:
    * cs127 (Original Java code)            https://cs127.github.io
    * Sightem (C++ port)                    https://github.com/Sightem
    * Nowilltolife (code improvements)      https://github.com/Nowilltolife
 */

#include <iostream>
#include <string>
#include <vector>
#include <exception>
#include <sstream>
#include <regex>
#include <cstring>
#include <numeric>
#include "clipboardxx.hpp"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#define _WIN_
#endif

#if !defined(_WIN_)
#include <chrono>
#include <thread>
#endif

struct CLIOptions {
    bool HELP = false;
    bool USE_STDIN = false;
    bool USE_STDOUT = false;
    bool AUTO_MARKDOWN = false;
    bool REVERSE_MODE = false;
};

struct Format {
    bool FORMAT_M : 1;
    bool FORMAT_S : 1;
};

const std::string HELP_MESSAGE =
"Usage: [EXEC] [OPTIONS] [COLORS]                                              \n"
"                                                                              \n"
"Options:                                                                      \n"
"-h | --help       Help (display this screen)                                  \n"
"-i | --stdin      Read input from STDIN instead of clipboard                  \n"
"-o | --stdout     Write output to STDOUT instead of clipboard                 \n"
"-d | --markdown   Wrap output in Markdown code block (for Discord)            \n"
"-r | --reverse    Reverse mode (removes syntax highlighting instead of adding)\n"
"                                                                              \n"
"Using markdown does nothing if reverse mode is enabled.                       \n"
"                                                                              \n"
"Colors:                                                                       \n"
"X,X,X,X,X,X,X,X  Each value from 0 to 15 (Discord only supports 0 to 7)       \n"
"format: Default,Note,Instrument,Volume,Panning,Pitch,Global,ChannelSeparator  \n"
"if not provided: 7,5,4,2,6,3,1,7                                              \n";

const int DEFAULT_COLORS[] = { 7, 5, 4, 2, 6, 3, 1, 7 };
const std::string HEADER = "ModPlug Tracker ";
const char* FORMATS_M[] = { "MOD", " XM" };
const char* FORMATS_S[] = { "S3M", " IT", "MPT" };

CLIOptions ParseCommandLine(int argc, char* argv[]);
std::vector<std::string> Split(const std::string& s, char delimiter);
std::string GetSGRCode(int color);
int GetEffectCmdColor(char c, Format format);
int GetVolumeCmdColor(char c);
int GetInstrumentColor(char c);
int GetNoteColor(char c);
bool isWhitespace(char c);
bool StartsWith(const char* pre, const char* str);

int main(int argc, char* argv[]) {

    // Find the first non-option command-line argument
    int ColorArgIndex;
    for (ColorArgIndex = 1; ColorArgIndex < argc; ColorArgIndex++) if (argv[ColorArgIndex][0] != '-') break;

    // Get command-line options
    CLIOptions Options = ParseCommandLine(argc, argv);

    // Show help (and then exit) if the help option is provided
    if (Options.HELP) {
        std::cout << HELP_MESSAGE;
        return 0;
    }

    // Use the first non-option command-line argument as the list of colors
    int Colors[8];
    try {
        std::vector<std::string> ColorArgs = Split(argv[ColorArgIndex], ',');
        for (int i = 0; i < ColorArgs.size(); i++) {
            Colors[i] = std::stoi(ColorArgs[i]);
            if (Colors[i] < 0 || Colors[i] > 15) throw std::logic_error("Color value out of range");
        }
    }
    catch (std::exception e) {
        if (!Options.USE_STDOUT) std::cout << "Colors not provided properly. Default colors will be used." << std::endl;
        for (int i = 0; i < 8; i++) {
            Colors[i] = DEFAULT_COLORS[i];
        }
    }

    // Read clipboard/STDIN
    std::string Input;
    std::vector<std::string> Lines;
    if (Options.USE_STDIN) {
        std::string Line;
        while (std::getline(std::cin, Line) && !Line.empty()) Lines.push_back(Line);
        Input = std::accumulate(Lines.begin(), Lines.end(), std::string(), [](std::string a, std::string b) { return a + b + '\n'; });
    }
    else {
        clipboardxx::clipboard clipboard;
        clipboard >> Input;
    }

    // Try to get the module format and check if the data is valid OpenMPT pattern data
    std::string module = Input.substr(HEADER.length(), 3);
    // last 3 characters of the header are the module format
    Format format;
    format.FORMAT_M = std::find(std::begin(FORMATS_M), std::end(FORMATS_M), module) != std::end(FORMATS_M);
    format.FORMAT_S = std::find(std::begin(FORMATS_S), std::end(FORMATS_S), module) != std::end(FORMATS_S);
    if (!format.FORMAT_M && !format.FORMAT_S) {
        if (!Options.USE_STDOUT) std::cout << "Invalid OpenMPT pattern data." << std::endl;
        return 1;
    }
    

    // Remove colors if the input is already syntax-highlighted
    Input = std::regex_replace(Input, std::regex("\u001B\\[\\d+(;\\d+)*m"), "");

    // Add colors if reverse mode is not enabled
    std::string Output;
    if (!Options.REVERSE_MODE) {
        int RelPos = -1;
        int Color = -1;
        int PreviousColor = -1;
        for (int i = 0; i < Input.length(); i++) {
            char c = Input[i];
            if (c == '|') RelPos = 0;
            if (RelPos == 0) Color = Colors[7];
            if (RelPos == 1) Color = Colors[GetNoteColor(c)];
            if (RelPos == 4) Color = Colors[GetInstrumentColor(c)];
            if (RelPos == 6) Color = Colors[GetVolumeCmdColor(c)];
            if (RelPos >= 9) {
                if (!(RelPos % 3)) Color = Colors[GetEffectCmdColor(c, format)];
                if (RelPos % 3 && c == '.' && Input[i - (RelPos % 3)] != '.') c = '0';
            }
            if (!isWhitespace(c)) {
                if (Color != PreviousColor) Output += GetSGRCode(Color);
                PreviousColor = Color;
            }
            Output += c;
            if (RelPos >= 0) RelPos++;
        }
    } else Output = Input;

    // Wrap in code block for Discord if specified
    if (Options.AUTO_MARKDOWN && !Options.REVERSE_MODE) Output = "```ansi\n" + Output + "```";

    // Write to clipboard/STDOUT
    if (Options.USE_STDOUT) std::cout << Output;
    else {
        clipboardxx::clipboard clipboard;
        clipboard << Output;
#if !defined(_WIN_)
        std::this_thread::sleep_for(std::chrono::milliseconds(150));
        // This is a hack to make sure the clipboard is updated before the program exits
#endif
    }
}

bool StartsWith(const char* pre, const char* str) {
    size_t lenpre = strnlen(pre, 256), lenstr = strnlen(str, 256);
    return lenstr < lenpre ? false : !strncmp(pre, str, lenpre);
}

CLIOptions ParseCommandLine(int argc, char* argv[]) {
    CLIOptions options;
    for (int i = 1; i < argc; i++) {
        if (StartsWith("--", argv[i])) {
            options.HELP          = !strncmp(argv[i], "--help", 6);
            options.USE_STDIN     = !strncmp(argv[i], "--stdin", 7);
            options.USE_STDOUT    = !strncmp(argv[i], "--stdout", 8);
            options.AUTO_MARKDOWN = !strncmp(argv[i], "--markdown", 10);
            options.REVERSE_MODE  = !strncmp(argv[i], "--reverse", 9);
        }
        else if (StartsWith("-", argv[i])) {
            for (int j = 1; j < strlen(argv[i]); j++) {
                switch(argv[i][j]) {
                    case 'h': options.HELP = true;          break;
                    case 'i': options.USE_STDIN = true;     break;
                    case 'o': options.USE_STDOUT = true;    break;
                    case 'd': options.AUTO_MARKDOWN = true; break;
                    case 'r': options.REVERSE_MODE = true;  break;
                }
            }
        }
    }
    return options;
}

std::vector<std::string> Split(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) tokens.push_back(token);
    return tokens;
}

bool isWhitespace(char c) {
    return (c == ' ' || c == '\t' || c == '\n' || c == '\r');
}

std::string GetSGRCode(int color) {
    return "\u001B[" + std::to_string(color + ((color < 8) ? 30 : 82)) + "m";
}

int GetNoteColor(char c) {
    return (c >= 'A' && c <= 'G') ? 1 : 0;
}

int GetInstrumentColor(char c) {
    return c >= '0' ? 2 : 0;
}

int GetVolumeCmdColor(char c) {
    int color = 0;
    switch (c) {
        case 'a': case 'b': case 'c': case 'd': case 'v':   color = 3; break; // Volume
        case 'l': case 'p': case 'r':                       color = 4; break; // Panning
        case 'e': case 'f': case 'g': case 'h': case 'u':   color = 5; break; // Pitch
    }
    return color;
}

int GetEffectCmdColor(char c, Format format) {
    int color = 0;
    if (format.FORMAT_S) { // S3M/IT/MPTM
        switch (c) {
            case 'D': case 'K': case 'L': case 'M': case 'N': case 'R':             color = 3; break; // Volume
            case 'P': case 'X': case 'Y':                                           color = 4; break; // Panning
            case 'E': case 'F': case 'G': case 'H': case 'U': case '+': case '*':   color = 5; break; // Pitch
            case 'A': case 'B': case 'C': case 'T': case 'V': case 'W':             color = 6; break; // Global
        }
    }
    else if (format.FORMAT_M) { // MOD/XM
        switch (c) {
            case '5': case '6': case '7': case 'A': case 'C':   color = 3; break; // Volume
            case '8': case 'P': case 'Y':                       color = 4; break; // Panning
            case '1': case '2': case '3': case '4': case 'X':   color = 5; break; // Pitch
            case 'B': case 'D': case 'F': case 'G': case 'H':   color = 6; break; // Global
        }
    }

    return color;
}
