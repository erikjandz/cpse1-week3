//Author is Erik de Zeeuw
#ifndef CLOCK_HPP
#define CLOCK_HPP   

#include "sinus.hpp"
#include <array>

class clock{
private:
    hwlib::window & w;               
    hwlib::target::pin_in & hoursButton;
    hwlib::target::pin_in & minutesButton;
    static constexpr auto sinusTable = table<91, int>(sinus);   //sinus lookup table for every integer value between 0 and 90
    int hours;      
    int minutes;
    int seconds;
    hwlib::circle circle = hwlib::circle(hwlib::xy(64, 32), 31);    //clock circle

public:
    clock(hwlib::window & w, hwlib::target::pin_in & hoursButton, hwlib::target::pin_in & minutesButton): 
        w( w ),
        hoursButton( hoursButton ),
        minutesButton( minutesButton )
        {}
    
    void start(){   //start the clock, keep updating it and change the clock values when user input is given
        hours = 0;      //initialise values
        minutes = 0;
        seconds = 0;
        unsigned int totalSeconds = 1;
        for(;;){        //infinity loop
            for(int i = 0; i < 3; i++){     //for loop to read user input multiple times a seconds
                if(!minutesButton.read()){
                    minutes ++;
                    if(minutes % 12 == 0){  //if minutes can be divided through 12, hours has to move a bit
                        hours ++;
                    }
                    if(minutes >= 60){      //prevent minutes from overflowing
                        minutes = 0;
                    }
                }
                if(!hoursButton.read()){
                    hours += 5;
                    if(hours >= 60){        //prevent hours from overflowing
                        hours = 0;
                    }
                    
                }
            hwlib::wait_ms(150);    //wait a bit
            draw();                 //draw the clock, hands and symbols
            }
            for(;;){                //the last section took less than a seconds, now wait till a seconds has passed
                if(hwlib::now_us() >= totalSeconds * 1000000){
                    totalSeconds ++;
                    seconds ++;
                    break;
                }
            }
            if(seconds >= 60){      //prevent seconds from overflowing and move minutes
                seconds = 0;
                minutes ++;
                if(minutes % 12 == 0){  //see earlier comments
                    hours ++;
                }
            }
            if(minutes >= 60){
                minutes = 0;
            }
            if(hours >= 60){
                hours = 0;
            }
        }
    }
    //draw the circle, hands and symbols
    void draw(){
        w.clear();
        circle.draw(w);
        drawHand(hours, 20);
        drawHand(minutes, 24);
        drawHand(seconds, 29);
        drawSymbols();
        w.flush();
    }
    //returns the coordinates which are a length distance away and in the direction of a position * 6 degree angle
    hwlib::xy getCoord(int position, int length){
        hwlib::xy coord;
        if(position >= 0 && position < 15){ 
            coord.x = 64 + sinusTable.get(6 * position) * length / 1000000;
            coord.y = 32 - sinusTable.get(90 - 6 * position) * length / 1000000;
        }else if(position >= 15 && position < 30){
            position -= 15;
            coord.x = 64 + sinusTable.get(90 - 6 * position) * length / 1000000;
            coord.y = 32 + sinusTable.get(6 * position) * length / 1000000;
        }else if(position >= 30 && position < 45){
            position -= 30;
            coord.x = 64 - sinusTable.get(6 * position) * length / 1000000;
            coord.y = 32 + sinusTable.get(90 - 6 * position) * length / 1000000;
        }else if(position >= 45 && position < 60){
            position -= 45;
            coord.x = 64 - sinusTable.get(90 - 6 * position) * length / 1000000;
            coord.y = 32 - sinusTable.get(6 * position) * length / 1000000;
        }
        return coord;
        
    }
    //draw a hand of the clock
    void drawHand(int position, int length){
        hwlib::line(hwlib::xy(64, 32),getCoord(position, length)).draw(w);
    }
    //draw the symbols to make the current time more clear
    void drawSymbols(){
        for(int i = 0; i < 12; i++){
            hwlib::line(getCoord(5 * i, 28), getCoord(5 * i, 32)).draw(w);
        }
    }
    
};

#endif
