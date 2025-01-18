#include "../library/logging.hpp"
#include <thread>
#include <iostream>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>

// Checking the level input for correctness
Log::importance_levels input_level(std::string input){
    if (input == "0"){
            return Log::Info;
        }
        else if(input == "1"){
            return Log::Warning;
        }
        else if (input == "2"){
            return Log::Error;
        }
        else{
            return Log::Unknown;
        }
}

std::queue<std::pair<std::string, Log::importance_levels>> message_queue; // Queue for storing messages
std::mutex queue_mutex; // Mutex for queue protection
std::condition_variable queue_condition;    // Condition variable for notifications
std::atomic<bool> stop_thread(false);   // Flag to terminate the thread

void recording_messages(Log& logger) {
    while (true) {
        std::pair<std::string, Log::importance_levels> message; // Variable for writing message
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            //If the queue is empty and the thread is not completed, we receive notifications
            queue_condition.wait(lock, [] { return !message_queue.empty() || stop_thread; });

            // Terminate the thread if the queue is empty and the stop flag is set
            if (stop_thread && message_queue.empty()) {
                break;
            }

            //We receive a message from the queue
            message = message_queue.front();
            message_queue.pop();
        }

        logger.enter_message(message.first, message.second); //Write the message to a file
    }
}

int main(int argc, char* argv[]) {

    // Check for correct number of arguments
    if (argc < 3) {
        std::cerr << "Usage: <journal name> <level of log>" << std::endl;
        return 1;
    }

    std::string log_file_name = argv[1];  // File name
    int default_level = std::stoi(argv[2]);  // Default importance level

    Log::importance_levels log_level = static_cast<Log::importance_levels>(default_level);

    // Checking that the default_level value is within the acceptable range
    if (log_level < Log::Info || log_level > Log::Error) {
        std::cerr << "Invalid log level. Please use a value between "
                  << Log::Info << " and " << Log::Error << std::endl;
        return 1;
    }

    Log logger(log_file_name, log_level); // Initializing a class object for writing messages
    std::thread worker(recording_messages, std::ref(logger));  // Starting a thread to write messages to a file

    std::string input_message; // Variable for entering messages
    std::string mode; // variable for selecting a function

    while(true){
        std::cout << "Choose function:\n0)Enter message\n1)Change default importance level\n2)Exit\n";
        std::getline(std::cin, mode);

        if (mode == "0"){
            std::string importance_level;
            std::cout << "Choose importance level:\n0)INFO\n1)WARNING\n2)ERROR\n";
            std::getline(std::cin, importance_level);

            Log::importance_levels level = input_level(importance_level); // Initializing the message importance level

            // Handling the case when the level is incorrect
            if (level == Log::Unknown) {
                level = logger.default_level;
                std::cout << "The default importance level will be used for the current message.\n";
            }

            std::cout << "Enter message:\n";
            std::getline(std::cin, input_message);

            {
                std::lock_guard<std::mutex> lock(queue_mutex);  // Capturing the mutex
                message_queue.push({input_message, level});  // Adding a message to the queue
            }
            queue_condition.notify_one();  // Notify the thread about a new message
        }

        // Changing the default importance level
        else if(mode == "1"){
            std::string importance_level;
            std::cout << "Choose importance level:\n0)INFO\n1)WARNING\n2)ERROR\n";
            std::getline(std::cin, importance_level);

            Log::importance_levels level = input_level(importance_level);

            if (level == Log::Unknown) {
                std::cout << "Incorrect input\n";
                continue;
            }

            logger.change_deffault_level(level); // Assigning a new default level value

            std::cout << "Default level has been changed.\n";
            continue;
        }

        else if(mode == "2"){
            {
                std::lock_guard<std::mutex> lock(queue_mutex);
                stop_thread = true;  // Set the termination flag
            }
            queue_condition.notify_one();  // Notifying the thread to terminate
            break;
        }

        else{
            continue;
        }
    }

    worker.join();  // Join the thread before the program ends

    return 0;
}

