#ifndef strdo_h
#define strdo_h

#include <string>    // std::string
#include <algorithm> // std::transform
#include <sstream>   // std::stringstream
#include <iomanip>   // std::setprecision

namespace strdo {
    inline void trim(std::string& str) {
        str.erase(0, str.find_first_not_of(" \n\r\t") + 1); // remove leading spaces
        str.erase(str.find_last_not_of(" \n\r\t"));         // remove tailing spaces
    }

    inline bool ends_with(const std::string& str, const std::string& ending) {
        if (ending.size() > str.size()) return false;
        return std::equal(ending.rbegin(), ending.rend(), str.rbegin());
    }

    inline bool starts_with(const std::string& str, const std::string& start) {
        if (start.size() > str.size()) return false;
        return std::equal(start.begin(), start.end(), str.begin());
    }

    inline bool equals_insensetive(std::string a, std::string b) {
        std::transform(a.begin(), a.end(), a.begin(), ::tolower);
        std::transform(b.begin(), b.end(), b.begin(), ::tolower);

        return a == b;
    }

    inline int comp_insensetive(std::string a, std::string b) {
        std::transform(a.begin(), a.end(), a.begin(), ::tolower);
        std::transform(b.begin(), b.end(), b.begin(), ::tolower);

        return a.compare(b);
    }

    inline std::string build(const std::string& left, const std::string& center, const std::string& right, size_t len) {
        size_t spaces = len - (left.size() + center.size() + right.size());
        size_t i      = 0;

        std::string result = left;

        if (spaces > 0) {
            while (i < spaces / 2) {
                result += ' ';
                ++i;
            }
        }

        result += center;

        if (spaces > 0) {
            while (i < spaces) {
                result += ' ';
                ++i;
            }
        }

        result += right;

        return result.substr(0, len);
    }

    template<class T>
    inline std::string to_str(T value) {
        std::stringstream ss;

        ss << value;
        return ss.str();
    }

    template<class T>
    inline std::string num_to_str(T value) {
        std::stringstream ss;

        if (value > 1000000000)
            ss <<  std::fixed << std::setprecision(2) << (double(value)/double(1000000000)) << 'G';
        else if (value > 1000000)
            ss <<  std::fixed << std::setprecision(2) << (double(value)/double(1000000)) << 'M';
        else if (value > 1000)
            ss << std::fixed <<  std::setprecision(2) << (double(value)/double(1000)) << 'K';
        else
            ss << value;

        return ss.str();
    }

    template<class T>
    inline std::string time_to_str(T value) {
        std::stringstream ss;

        if (value > (1000 * 60 * 60))
            ss << (value)/(1000 * 60 * 60) << 'h';
        else if (value > (1000 * 60))
            ss << (value)/(1000 * 60) << 'm';
        else if (value > 1000)
            ss << (value)/(1000) << 's';
        else
            ss << value << "ms";

        return ss.str();
    }
}

#endif /* ifndef strdo_h */