#include <iostream>

class PasswordChecker {
private:
    std::string password;
    std::string secondPassword;
    int charCount[5];
    int passwordLength;

public:
    PasswordChecker() {
        while (password.empty() || secondPassword.empty() || !matching(password, secondPassword)) {
            password = "";
            passwordLength = 0;
            for (int i = 0; i < 5; i++) {
                charCount[i] = 0;
            }

            while (!checkRules()) {
                password = "";
                passwordLength = 0;
                for (int i = 0; i < 5; i++) {
                    charCount[i] = 0;
                }

                displayRules();

                askForPassword();

                while (passwordLength < 8 || passwordLength > 32) {
                    std::cout << "Your password's length must be between 8-32 characters.\n";
                    askForPassword();
                }

                for (int i = 0; i < passwordLength; i++) {
                    charCount[checkOneChar(password[i])]++;
                }

                if (!checkRules()) {
                    displayFail();
                }
            }

            askForSecondPassword();
            if (!matching(password, secondPassword)) {
                displayMatchFail();
            }
        }

        displaySuccess();
    }

    void displayRules() {
        std::cout
                << "Password Rules:\n1) The password length: 8-32 characters\n2) At least one numerical, i.e., 0, 1, 2,..\n3) At least one upper case letter, i.e., A, B, C, ...\n4) At least one lower case letter, i.e., a, b, c, ...\n5) At least one of the special characters, but it must be within the set:{ # $ * ( ) % & ^} a total of eight (8) special characters. no other special characters are allowed.\n";
    }

    void askForPassword() {
        std::cout << "Please enter your password.\n";
        std::cin >> password;
        passwordLength = password.length();
    }

    void askForSecondPassword() {
        std::cout << "Please re-enter your password.\n";
        std::cin >> secondPassword;
    }


    void displaySuccess() {
        std::cout << "Your password will be updated in 2 minutes.\n";
    }

    void displayFail() {
        std::cout << "Your password failed one or more password rules.\n";
    }

    void displayMatchFail() {
        std::cout << "Match fail...\n";
    }

    int checkOneChar(char c) {
        if (c >= '0' && c <= '9') {
            return 1;
        } else if (c >= 'a' && c <= 'z') {
            return 2;
        } else if (c >= 'A' && c <= 'Z') {
            return 3;
        } else if (c == '#' || c == '$' || c == '*' || c == '(' || c == ')' || c == '%' || c == '&' || c == '^') {
            return 4;
        } else return 0;
    }

    bool checkRules() {
        return charCount[0] == 0 && charCount[1] >= 1 && charCount[2] >= 1 && charCount[3] >= 1 && charCount[4] >= 1;
    }

    bool matching(std::string s1, std::string s2) {
        return s1 == s2;
    }
};

int main() {
    PasswordChecker passwordChecker;
}