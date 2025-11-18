#include "mbed.h"
#include "TextLCD.h"

#define LETTER_GAP 0.5  //seconds
#define WORD_GAP 1      //seconds

//Pin configuration
DigitalIn dotButton(PA_0);
DigitalIn dashButton(PA_1);
DigitalOut dotLed(PB_0);
DigitalOut dashLed(PB_7);
TextLCD MyLCD(PC_0, PC_1, PC_2, PC_3, PC_4, PC_5);

Timer gapTimer;

//Morse code implementation
char morseInput[10], message[100];
int morseIndex = 0, messageIndex = 0;

struct MorseSymbol{
    const char* code;
    char letter;
};

const MorseSymbol morseDict[] = {
    {".-", 'A'}, {"-...", 'B'}, {"-.-.", 'C'}, {"-..", 'D'}, {".", 'E'}, {"..-.", 'F'}, {"--.", 'G'}, {"....", 'H'},
    {"..", 'I'}, {".---", 'J'}, {"-.-", 'K'}, {".-..", 'L'}, {"--", 'M'}, {"-.", 'N'}, {"---", 'O'}, {".--.", 'P'},
    {"--.-", 'Q'}, {".-.", 'R'},  {"...", 'S'}, {"-", 'T'}, {"..-", 'U'}, {"...-", 'V'}, {".--", 'W'}, {"-..-", 'X'},
    {"-.--", 'Y'}, {"--..", 'Z'},

    {"-----", '0'}, {".----", '1'}, {"..---", '2'}, {"...--", '3'}, {"....-", '4'}, {".....", '5'}, {"-....", '6'}, 
    {"--...", '7'}, {"---..", '8'}, {"----.", '9'}
};
const int MORSE_COUNT = sizeof(morseDict)/sizeof(MorseSymbol);

char decodeMorse(char* code){
    for(int i = 0; i < MORSE_COUNT; i++){
        if(strcmp(code, morseDict[i].code) == 0) return morseDict[i].letter;
    }
    return '?';
}

int main(){

    char lcdBuffer[33];
    memset(lcdBuffer, ' ', 32);
    lcdBuffer[32] = '\0';
    int lcdIndex = 0;

    bool gapTriggered = false;
    gapTimer.start();
    while(1){
        if(dotButton == 1){
            dotLed = 1;
            morseInput[morseIndex++] = '.';
            if(morseIndex > 9){
                morseIndex = 0;
                memset(morseInput, 0, sizeof(morseInput));
            }
            while(dotButton == 1); //debounce
            dotLed = 0;
            gapTimer.reset();
        }
        else if (dashButton == 1) {
            dashLed = 1;
            morseInput[morseIndex++] = '-';
            if(morseIndex > 9){
                morseIndex = 0;
                memset(morseInput, 0, sizeof(morseInput));
            }
            while(dashButton == 1); //debounce
            dashLed = 0;
            gapTimer.reset();
        }
        
        if(gapTimer.read() >= LETTER_GAP && morseIndex > 0){
            morseInput[morseIndex] = '\0';
            message[messageIndex] = decodeMorse(morseInput);
            if(message[messageIndex] == '?'){
                morseIndex = 0;
                memset(morseInput, 0, sizeof(morseInput)); //reset morseInput values
                gapTimer.reset();
            }
            else{
                messageIndex++;
                memset(morseInput, 0, sizeof(morseInput)); //reset morseInput values
                gapTriggered = 1;
                morseIndex = 0;
                if(messageIndex > 0 && lcdIndex == 15){
                    for(int i = 0; i < messageIndex; i++){
                        lcdBuffer[lcdIndex-i] = ' ';
                    }
                    lcdIndex = 16;
                    for(int i = 0; i < messageIndex; i++){
                        lcdBuffer[lcdIndex++] = message[i];
                    }

                }
                if(lcdIndex >= 32){
                    lcdIndex = 0;
                    memset(lcdBuffer, ' ', 32);
                    lcdBuffer[32] = '\0';
                    for(int i = 0; i < messageIndex; i++){
                        lcdBuffer[lcdIndex++] = message[i];
                    }
                }
                else{
                    lcdBuffer[lcdIndex++] = message[messageIndex-1];
                    MyLCD.printf("%s", lcdBuffer);
                }
            }

        }
        if(gapTimer.read() >= WORD_GAP && gapTriggered){
            lcdBuffer[lcdIndex++] = ' ';
            MyLCD.printf("%s", lcdBuffer);

            gapTriggered = false;
            memset(morseInput, 0, sizeof(morseInput)); //reset morseInput values
            memset(message, 0, sizeof(message)); //reset morseInput values
            morseIndex = 0;

        }
    }
}
