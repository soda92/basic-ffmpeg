#include <string>
#include <iostream>
using std::cin;
using std::cout;
using std::endl;
using std::string;
#include <ctime>
#include <sstream>

#include <fstream>
#include <cstdlib>
#include <filesystem>
namespace fs = std::filesystem;

class videoRecorder
{
public:
    std::time_t now;
    std::tm calendar_time;
    std::string folder_name;
    std::string file_name;
    int get_current_min()
    {
        now = std::time(nullptr); // get the current time point

        // convert it to (local) calendar time
        // http://en.cppreference.com/w/cpp/chrono/c/localtime
        calendar_time = *std::localtime(std::addressof(now));
        return calendar_time.tm_min;
    }

    bool create_folder(int mode)
    {
        std::stringstream ss;
        ss << calendar_time.tm_year << '-' << calendar_time.tm_mon << '-' << calendar_time.tm_mday;

        return !(fs::create_directory(ss.str()));
    };

    std::string update_filename()
    {
        std::stringstream ss;
        ss << calendar_time.tm_year << '-' << calendar_time.tm_mon << '-' << calendar_time.tm_mday;
        ss << '/';
        ss << calendar_time.tm_hour << '-' << calendar_time.tm_min << "mp4";
        return ss.str();
    }
};
