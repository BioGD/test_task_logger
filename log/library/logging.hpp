#ifndef LOGGING_H
#define LOGGING_H

#include <string>
#include <fstream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

// Log class for creating a log and writing a message to it
class Log {
private:
    std::string log_file_name; // file name
    std::ofstream log_file; // message log file

public:

    // message importance levels
    enum importance_levels {
        Info,
        Warning,
        Error,
        Unknown
    };

    importance_levels default_level; //default importance level

    // class constructor that takes as input the file name and the default level
    Log(const std::string& entered_file_name, importance_levels entered_importance_level = Info);

    // class destructor
    ~Log(); 

    // class method to change default level
    void change_deffault_level(importance_levels entered_importance_level);

    // class method for writing a message
    void enter_message(const std::string& message, importance_levels level);

private:
    // class method to get current time
    std::string get_current_time() const;

    // class method to get message importance
    std::string get_current_imp_level(importance_levels level) const;
};

#endif // LOGGING_H