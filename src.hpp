// src.hpp - Implementation for Problem 112 (shell 模拟)
#include <string>
#include <cstring>
#include <vector>
#include <map>
#include <functional>

using std::pair;
using std::string;
using std::vector;
using std::map;
using std::function;

namespace final {
    class arguments {
    private:
        // WARNING: You cannot add more member variables.
        int _argc;
        char** _argv;

        static vector<string> split_by_spaces(const string& s) {
            vector<string> tokens;
            const char* p = s.c_str();
            size_t n = s.size();
            size_t i = 0;
            while (i < n) {
                while (i < n && p[i] == ' ') ++i;
                if (i >= n) break;
                size_t j = i;
                while (j < n && p[j] != ' ') ++j;
                tokens.emplace_back(s.substr(i, j - i));
                i = j;
            }
            return tokens;
        }

        static char** alloc_argv_from_tokens(const vector<string>& tokens, int& out_argc) {
            out_argc = static_cast<int>(tokens.size());
            if (out_argc == 0) return nullptr;
            char** arr = new char*[out_argc];
            for (int i = 0; i < out_argc; ++i) {
                const string& t = tokens[static_cast<size_t>(i)];
                size_t len = t.size();
                arr[i] = new char[len + 1];
                // Use memcpy/strcpy from <cstring>
                std::memcpy(arr[i], t.c_str(), len);
                arr[i][len] = '\0';
            }
            return arr;
        }

        static char** deep_copy_argv(char** src, int argc) {
            if (argc <= 0 || src == nullptr) return nullptr;
            char** arr = new char*[argc];
            for (int i = 0; i < argc; ++i) {
                if (src[i] == nullptr) { arr[i] = nullptr; continue; }
                size_t len = std::strlen(src[i]);
                arr[i] = new char[len + 1];
                std::memcpy(arr[i], src[i], len + 1);
            }
            return arr;
        }

        static void free_argv(char** argv, int argc) {
            if (!argv) return;
            for (int i = 0; i < argc; ++i) {
                delete[] argv[i];
            }
            delete[] argv;
        }

    public:
        arguments() : _argc(0), _argv(nullptr) {}

        explicit arguments(const string& cmd) : _argc(0), _argv(nullptr) {
            vector<string> tokens = split_by_spaces(cmd);
            _argv = alloc_argv_from_tokens(tokens, _argc);
        }

        // Copy constructor (deep copy)
        arguments(const arguments& other) : _argc(other._argc), _argv(nullptr) {
            _argv = deep_copy_argv(other._argv, _argc);
        }

        // Move constructor
        arguments(arguments&& other) noexcept : _argc(other._argc), _argv(other._argv) {
            other._argc = 0;
            other._argv = nullptr;
        }

        // Copy assignment
        arguments& operator=(const arguments& other) {
            if (this == &other) return *this;
            free_argv(_argv, _argc);
            _argc = other._argc;
            _argv = deep_copy_argv(other._argv, _argc);
            return *this;
        }

        // Move assignment
        arguments& operator=(arguments&& other) noexcept {
            if (this == &other) return *this;
            free_argv(_argv, _argc);
            _argc = other._argc;
            _argv = other._argv;
            other._argc = 0;
            other._argv = nullptr;
            return *this;
        }

        ~arguments() {
            free_argv(_argv, _argc);
            _argc = 0;
            _argv = nullptr;
        }

        // WARNING: You cannot modify the following functions
        int argc() const { return _argc; }
        char** argv() const { return _argv; }
    };

    // You don't need to modify shell.
    class shell {
    private:
        map<int, arguments> running_list;
    public:
        shell() = default;

        void run(int pid, const string& cmd, const function<void(int, char**)>& invoked) {
            running_list.emplace(pid, cmd);
            invoked(running_list[pid].argc(), running_list[pid].argv());
        }

        int subprocessExit(int pid, int return_value) {
            running_list.erase(pid);
            return return_value;
        }
        vector<int> getRunningList() const {
            vector<int> rt;
            for (auto& pair:running_list)rt.push_back(pair.first);
            return rt;
        }
    };
}

