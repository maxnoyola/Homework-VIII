#include <iostream>
#include <bitset>
#include <cstdlib>
#include <ctime>

class MotorStatus {
private:
    uint8_t motorState; // Each bit represents the status of a motor (on/off)
    uint8_t overheatingMotors; // Motors that are overheating
    int turnCount;

    void initialize() {
        srand(time(0));
        motorState = rand() % 256; // Randomly turn motors on/off
        updateOverheating();       // Randomly overheat one motor
        turnCount = 0;
        displayStatus();
    }

    void updateOverheating() {
        int newMotor;
        do {
            newMotor = rand() % 8; // Randomly select a motor
        } while (((1 << newMotor) & motorState) == 0 || // Ensure the motor is ON
                 ((1 << newMotor) & overheatingMotors) != 0); // Ensure the motor is not already overheating
        overheatingMotors |= (1 << newMotor); // Set the selected motor to overheat
        turnCount++;
    }

    void displayStatus() {
        std::cout << "Motor State (ON/OFF): " << std::bitset<8>(motorState) << std::endl;
        std::cout << "Overheating Motors: " << std::bitset<8>(overheatingMotors) << std::endl;
        std::cout << "Turn Count: " << turnCount << std::endl;
    }

public:
    MotorStatus() {
        initialize();
    }

    bool turnOff(uint8_t guess) {
        motorState ^= (overheatingMotors & guess);

        if ((motorState & overheatingMotors) == 0) {
            return true; // All overheating motors are off
        }

        updateOverheating();
        displayStatus();
        return false;
    }
};

int main() {
    MotorStatus motorStatus;
    uint8_t guess;

    std::cout << "Enter a guess for the motor(s) to turn off (in binary, 8 bits): ";

    while (true) {
        // Prompt user for a guess
        std::string guessInput;
        std::cin >> guessInput;

        // Convert input to a uint8_t
        guess = std::bitset<8>(guessInput).to_ulong();

        // Attempt to turn off motors with the guessed input
        if (motorStatus.turnOff(guess)) {
            std::cout << "Congratulations! You've successfully turned off all overheating motors." << std::endl;
            break;
        } else {
            std::cout << "Incorrect guess. Try again." << std::endl;
        }
    }

    return 0;
}
