#include "Akinator.hpp"

#define TX_USE_SPEAK
#include "TXLib.h"

#include <cstdlib>

void flushStdin();

int main()
{
    Akinator akinator;

    txSpeak("\vHi! I am an akinator.\n");
    txSpeak("\vWould you like to [l]oad an existing database or [c]reate a new one?\n");
    bool answered = false;
    while (!answered) {
        std::printf("Would you like to [l]oad an existing database or [c]reate a new one?\n");
        switch (std::getchar()) {
            case 'l': {
                answered = true;
                akinator.loadDb();
                break;
            }
            case 'c': {
                answered = true;
                akinator.createDb();
                break;
            }
            default: {
                flushStdin();
            }
        }
    }

    flushStdin();
    txSpeak("\vWhat would you like to do? Possible actions: [g]uess, [d]efine, [c]ompare, show [t]ree, [s]ave database and quit or\n"
                "just [q]uit.\n");
    while (true) {
        flushStdin();
        switch (std::getchar()) {
            case 'g': {
                flushStdin();
                akinator.guess();
                break;
            }
            case 'd': {
                flushStdin();
                akinator.define();
                break;
            }
            case 'c': {
                flushStdin();
                akinator.compare();
                break;
            }
            case 't': {
                flushStdin();
                akinator.showTree();
                break;
            }
            case 's': {
                akinator.saveDb();
            }
            case 'q': {
                return EXIT_SUCCESS;
            }
            default: {
                flushStdin();
            }
        }
        std::printf("What would you like to do? Possible actions: [g]uess, [d]efine, [c]ompare, show [t]ree, [s]ave database and quit or\n"
                    "just [q]uit.\n");
    }
}

void flushStdin()
{
    int ch = 0;
    while (((ch = getchar()) != '\n') && (ch != EOF));
}
