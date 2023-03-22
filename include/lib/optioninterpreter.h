#pragma once

#include <map>
#include <string>
#include <vector>
#include <functional>
#include <iostream>

class OptionInterpreter {
public:
    OptionInterpreter() {};
    virtual ~OptionInterpreter() {};

    using FunctionWithArgment = std::function<void(std::string)>;
    using FunctionWithoutArgment = std::function<void(void)>;

    bool add(std::string key, std::string usage, FunctionWithArgment func) {
        std::string keyWithDash = std::string("-") + key;

        auto it = _functionsWithArgment.find(keyWithDash);
        if(it != _functionsWithArgment.end()) return false;

        _functionsWithArgment[keyWithDash] = func;

        std::string usageWithDash = std::string(" ") + keyWithDash + usage;
        _usages.push_back(usageWithDash);

        return true;
    }
    bool add(std::string key, std::string usage, FunctionWithoutArgment func) {
        std::string keyWithDash = std::string("-") + key;

        auto it = _functionsWithoutArgment.find(keyWithDash);
        if(it != _functionsWithoutArgment.end()) return false;

        _functionsWithoutArgment[keyWithDash] = func;

        std::string usageWithDash = std::string(" ") + keyWithDash + usage;
        _usages.push_back(usageWithDash);

        return true;
    }

    virtual void analize(int argc, char *argv[]) {
        if(argc == 1) return;

        for(int i = 1; i < argc; i++) {
            auto option = std::string(argv[i]);
            if(option[0] != '-') continue;

            auto func = _functionsWithoutArgment.find(option);
            if(func != _functionsWithoutArgment.end()) func->second();
            else {
                auto arg = ((i < argc - 1) && (argv[i + 1][0] != '-')) ?
                    std::string(argv[i + 1]) : "";
                if(arg == "") {
                    std::cout << "error: " << option << " is invalid option" << std::endl;
                    exit(0);
                }

                auto func = _functionsWithArgment.find(option);
                if(func == _functionsWithArgment.end()) {
                    std::cout << "error: " << option << " is invalid option" << std::endl;
                    exit(0);
                }
                func->second(arg);
            }
        }
    }

    static void warning(std::string option, std::string arg) {
        std::cout << "\"" << arg << "\" is invalid argument " << "for option \"-" << option << "\"" << std::endl;
    }

    virtual void showUsages() {
        std::cout << "usage" << std::endl;
        std::cout << "options" << std::endl;
        for(auto& usage : _usages)
            std::cout << usage << std::endl;
    }

protected:
    std::map<std::string, FunctionWithArgment> _functionsWithArgment;
    std::map<std::string, FunctionWithoutArgment> _functionsWithoutArgment;
    std::vector<std::string> _usages;
};
