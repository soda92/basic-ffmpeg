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

#include <fmt/core.h>
#include <boost/filesystem.hpp>

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
        auto current_dir = boost::filesystem::current_path().string();
        fmt::print("current dir: {}\n", current_dir);
        std::string folder = fmt::format(
            "{}-{:02}-{:02}",
            calendar_time.tm_year + 1900,
            calendar_time.tm_mon + 1,
            calendar_time.tm_mday);
        return !(boost::filesystem::create_directory(folder));
    };

    std::string update_filename()
    {
        auto filename = fmt::format("{}-{:02}-{:02}/{:02}_{:02}_{:02}.mp4",
                                    calendar_time.tm_year + 1900,
                                    calendar_time.tm_mon + 1,
                                    calendar_time.tm_mday,
                                    calendar_time.tm_hour,
                                    calendar_time.tm_min,
                                    calendar_time.tm_sec);
        return filename;
    }
};
