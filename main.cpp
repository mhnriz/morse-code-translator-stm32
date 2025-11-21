#include "mbed.h"
#include "TextLCD.h"
#include "cstring"
#include <string.h>

TextLCD MyLCD(PA_0, PA_1, PA_4, PB_0, PC_1, PC_0);
DigitalIn dotButton(PC_6);
DigitalIn dashButton(PC_8);
Timer gapTimer;

#define LETTER_GAP 0.5
#define WORD_GAP 1.5

//morse code configuration
char morseInput[10], tempWord[32];
int morseIndex = 0, tempIndex = 0, index = 0;

struct morseSymbol {
    const char* code;
    const char letter;
};
const morseSymbol morseTable[] = {
    {".-", 'A'}, {"-...", 'B'}, {"-.-.", 'C'}, {"-..", 'D'}, {".", 'E'}, {"..-.", 'F'}, {"--.", 'G'}, {"....", 'H'},
    {"..", 'I'}, {".---", 'J'}, {"-.-", 'K'}, {".-..", 'L'}, {"--", 'M'}, {"-.", 'N'}, {"---", 'O'}, {".--.", 'P'},
    {"--.-", 'Q'}, {".-.", 'R'},  {"...", 'S'}, {"-", 'T'}, {"..-", 'U'}, {"...-", 'V'}, {".--", 'W'}, {"-..-", 'X'},
    {"-.--", 'Y'}, {"--..", 'Z'},

    {"-----", '0'}, {".----", '1'}, {"..---", '2'}, {"...--", '3'}, {"....-", '4'}, {".....", '5'}, {"-....", '6'}, 
    {"--...", '7'}, {"---..", '8'}, {"----.", '9'}
};
const int MORSE_COUNT = sizeof(morseTable)/sizeof(morseSymbol);

char decodeMorse(char* input){
    for(int i = 0; i < MORSE_COUNT; i++){
        if(strcmp(input,morseTable[i].code) == 0) return morseTable[i].letter;
    }
    return '?';
}

void bootScreen(){
    gapTimer.start();
    while(1){
        MyLCD.printf("BY ITERATION.   Please wait");
        wait(0.5);
        for(int i = 0; i < 3; i++){
            MyLCD.printf(".");
            wait(0.5);   
        }
        MyLCD.cls();
        if(gapTimer.read() >= 5) break;
    }
    gapTimer.stop();
}

void move_range(char *array, int srcStart, int srcEnd, int dstStart) {
    int length = srcEnd - srcStart + 1;
    memmove(&array[dstStart], &array[srcStart], length * sizeof(char));
    for(int i = srcStart; i <= srcEnd-1; i++) array[i] = ' ';
}


int main(){
    dotButton.mode(PullDown);
    dashButton.mode(PullDown);
    bootScreen();
    gapTimer.reset();
    gapTimer.start();

    char lcdBuffer[32];
    int lcdIndex = 0;
    bool dotPressed = false;
    bool dashPressed = false;
    bool inputOccured = false;
    bool gapTriggered = false;

    memset(morseInput, 0, sizeof(morseInput));
    memset(lcdBuffer, ' ', sizeof(lcdBuffer));

    while(1){
        //dot button
        if(dotButton == 1 && !dotPressed){
            dotPressed = true;
            gapTriggered = false;
            wait(0.02f);

            if(dotButton == 1){
                if(morseIndex < 9){
                    morseInput[morseIndex++] = '.';
                    morseInput[morseIndex] = '\0';
                }

                inputOccured = true;
                gapTimer.reset();
            }
        }
        if(dotButton == 0) dotPressed = false;

        //dash button
        if(dashButton == 1 && !dashPressed){
            dashPressed = true;
            gapTriggered = false;
            wait(0.02f);

            if(dashButton == 1){
                if(morseIndex < 9){
                    morseInput[morseIndex++] = '-';
                    morseInput[morseIndex] = '\0';
                }

                inputOccured = true;
                gapTimer.reset();
            }
        }
        if(dashButton == 0) dashPressed = false;

        if(lcdIndex > 32) {
                move_range(lcdBuffer, lcdIndex-tempIndex, lcdIndex-1, 0);
                lcdIndex = 0 + tempIndex;
                for(int i = 32; i > tempIndex; i--) lcdBuffer[i] = ' ';
                MyLCD.cls();
                
                for(int i = 0; i < 16; i++)
                    MyLCD.putc((i < lcdIndex) ? lcdBuffer[i] : ' ');
                for(int i = 16; i < 32; i++)
                    MyLCD.putc((i < lcdIndex) ? lcdBuffer[i] : ' ');
            }

        //letter gap
        if(inputOccured && gapTimer.read() > LETTER_GAP){
            
            
            char decoded = decodeMorse(morseInput);
            tempWord[tempIndex++] = decoded;
            lcdBuffer[lcdIndex++] = decoded;

            if(lcdIndex-tempIndex < 16 && lcdIndex-1 >= 16){
                move_range(lcdBuffer, lcdIndex-tempIndex, lcdIndex-1, 16);
                lcdIndex = 16 + tempIndex;
            }

            //LCD PRINTING
            for(int i = 0; i < 16; i++)
                MyLCD.putc((i < lcdIndex) ? lcdBuffer[i] : ' ');
            for(int i = 16; i < 32; i++)
                MyLCD.putc((i < lcdIndex) ? lcdBuffer[i] : ' ');

            memset(morseInput, 0, sizeof(morseInput));
            morseIndex = 0;
            
            inputOccured = false;
            gapTriggered = true;
            

        }
        if(gapTimer.read() > WORD_GAP && gapTriggered){
            lcdBuffer[lcdIndex++] = ' ';
            
            

            memset(tempWord, 0, sizeof(tempWord));
            tempIndex = 0;
            inputOccured = false;
            gapTriggered = false;
            gapTimer.reset();

        }
    }
}