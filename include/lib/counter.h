#pragma once

#include <map>
#include <math.h>
#include <string>
#include <sstream>
#include <fstream>
#include <numeric>
#include <iomanip>

template<class IndexType = int>
class Counter {
public:
    void count(IndexType index) {
        auto i = _histogram.find(index);
        if(i == _histogram.end()) _histogram[index] = 0;

        _histogram[index]++;
    }

    void erase(IndexType index) {
        _histogram.erase(index);
    }

    double getRateOf(IndexType index) const {
        unsigned long long int sum = 0;
        for(auto it = _histogram.begin(); it != _histogram.end(); it++)
            sum += it->second;

        return (double)_histogram.find(index)->second / (double)sum;
    }

    std::string getStringPercentageOf(IndexType index) const {
        std::string result;
        result += std::to_string(getRateOf(index) * 100) + "%";

        unsigned long long int sum = 0;
        for(auto it = _histogram.begin(); it != _histogram.end(); it++)
            sum += it->second;

        if(sum == 0) return "NaN";

        result += "(" + std::to_string(_histogram.find(index)->second) +
            "/" + std::to_string(sum) + ")";

        return result;
    }

    void reset() {
        _histogram.clear();
    }

    unsigned long long int operator[](IndexType index) const {
        return _histogram[index];
    }

    void saveDataTo(std::string fileName) const {
        std::ofstream ofs(fileName);
        for(auto it = _histogram.begin(); it != _histogram.end(); it++) {
            ofs << it->first << "," << it->second << std::endl;
        }
    }

    std::string toString() const {
        std::stringstream ss;

        int i = 0;
        for(auto it = _histogram.begin(); it != _histogram.end(); it++) {
            ss << it->first << ": " << it->second;
            i++;
            if(i < size()) ss << std::string("\n");
        }

        return ss.str();
    }

    std::string toStringInDescendingOrder() const {
        std::stringstream ss;
        auto sortedHistogram = getMapExchanedKeysAndValues();

        unsigned long long int prevEntryCounter = 0;
        int ranking = 0;
        for(auto it = sortedHistogram.rbegin(); it != sortedHistogram.rend(); it++) {
            ranking++;
            ss << getRankingString(it->first, prevEntryCounter, ranking);

            ss << " ";
            ss << std::left << std::setw(10) << it->second;
            ss << std::right << std::setw(10) << it->first;
            ss << getPercentStringOf(it->second);

            prevEntryCounter = it->first;
        }

        auto result = getResultString();
        for(int i = 0; i < result.length(); i++) {
            ss << "-";
        }
        ss << "\n";
        ss << result;
        return ss.str();
    }

    int size() const {return _histogram.size();}

private:
    std::map<IndexType, unsigned long long int> _histogram;

    int getDigitOfEntries() const {return log10(this->size()) + 1;}

    std::multimap<unsigned long long int, IndexType> getMapExchanedKeysAndValues() const {
        std::multimap<unsigned long long int, IndexType> result;

        for(auto it = _histogram.begin(); it != _histogram.end(); it++) {
            std::pair<unsigned long long int, IndexType> pair;
            pair.first = it->second;
            pair.second = it->first;

            result.insert(pair);
        }

        return result;
    }

    std::string getRankingString(unsigned long long count, unsigned long long prevEntryCounter, int ranking) const {
        std::stringstream ss;

        const int digit = getDigitOfEntries();
        if(count != prevEntryCounter) {
            ss << "[" << std::right << std::setw(digit) << ranking << "]";
            return ss.str();
        }

        for(int i = 0; i < digit + 2; i++) ss << " ";
        return ss.str();
    }

    std::string getResultString() const {
        std::stringstream ss;
        ss << "total        ";
        for(int i = 0; i < getDigitOfEntries(); i++) {
            ss << " ";
        }

        unsigned long long sum = 0;
        double totalPercentage = 0;
        for(auto it = _histogram.begin(); it != _histogram.end(); it++) {
            sum += it->second;
            totalPercentage += getRateOf(it->first) * 100;
        }

        ss << std::right << std::setw(10) << sum;
        ss << " (" << std::setw(6) << std::right << std::fixed;
        ss << std::setprecision(3) << totalPercentage << "%)";

        return ss.str();
    }

    std::string getPercentStringOf(IndexType target) const {
        std::stringstream ss;
        ss << " (" << std::setw(7) << std::right << std::fixed;
        const auto percent = getRateOf(target) * 100;
        ss << std::setprecision(3) << percent << "%)" << std::endl;

        return ss.str();
    }
};
