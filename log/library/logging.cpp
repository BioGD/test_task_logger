#include "logging.hpp"
#include <iostream>

// class constructor that takes as input the file name and the default level
Log::Log(const std::string& entered_file_name, importance_levels entered_importance_level)
    : log_file_name(entered_file_name + ".log"), default_level(entered_importance_level) {

    // opening a file for writing
    log_file.open(log_file_name, std::ios::out | std::ios::app);
    if (!log_file.is_open()) {
        std::cerr << "Error: Failed to open log file!" << std::endl;
    }
}

// class destructor
Log::~Log() {

    // closing a file when a class instance is destroyed
    if (log_file.is_open()) {
        log_file.close();
    }
}

// class method to change default level
void Log::change_deffault_level(importance_levels entered_importance_level) {
    default_level = entered_importance_level; // Assigning a new default level value
}

// class method for writing a message
void Log::enter_message(const std::string& message, importance_levels level) {

    // check if the importance level of a message is lower than the default level
    if (level < default_level) {
        std::cout << "Error: The current message's importance level is less than the default importance level. Press Enter to continue\n";
        return;
    }

    std::string log_str; // variable for writing information to a file

    std::string cur_time = Log::get_current_time(); // variable containing the current time
    std::string level_str = Log::get_current_imp_level(level); // variable containing the current level

    log_str = "[" + cur_time + "] [" + level_str + "] " + message; // collecting all the information into one variable

    log_file << log_str << std::endl; // writing information to a file
}

// class method to get current time
std::string Log::get_current_time() const {
    auto now = std::chrono::system_clock::now();
    std::time_t now_time_t = std::chrono::system_clock::to_time_t(now);
    std::tm now_tm = *std::localtime(&now_time_t);

    std::stringstream ss;
    ss << std::put_time(&now_tm, "%Y-%m-%d %H:%M:%S");

    std::string date_time_str = ss.str();

    return date_time_str;
}

// class method to get message importance
std::string Log::get_current_imp_level(importance_levels level) const {

    switch (level) {
        case Info:
            return "INFO";
            break;
        case Warning:
            return "WARNING";
            break;
        case Error:
            return "ERROR";
            break;
        default:
            return "UNKNOWN";
            break;
    }
}