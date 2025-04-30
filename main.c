#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include "main.h"

pthread_mutex_t mutex, led_mutex;
int pin[] = {2,3,4,17,27,22,10,9};
int LEDstatus = 0b00000001;
int button_pin = 14;

#define SECOND_IN_MS 1000000

int buttonState=0;

int lastButtonPressedState = 0;

void turnOn(unsigned int target){
    gpio_export(target);
    gpio_set_dir(target,"out");
    gpio_set_value(target,1);
}
void turnOff(unsigned int target){  
    gpio_set_value(target,0);
    gpio_unexport(target);
}

int buttonPressed(unsigned int target) {
    gpio_export(target);
    gpio_set_dir(target,"in");
    return gpio_get_value(target);
}

void buttonState0Loop() {
    for(int j = 0 ; j < 8 ; j++){        
        if(LEDstatus >> j == 1){
            turnOn(pin[j]);
        }else{
            turnOff(pin[j]);
        }
    }
    pthread_mutex_lock(&led_mutex);
    LEDstatus = LEDstatus == 0b10000000 ? 0b00000001 : LEDstatus << 1;
    pthread_mutex_unlock(&led_mutex);
    usleep(SECOND_IN_MS);
}

void buttonState1Loop() {

    for(int j = 0 ; j < 8 ; j++){        
        if(LEDstatus >> j & 1 == 1){
            turnOn(pin[j]);
        }else{
            turnOff(pin[j]);
        }
    }
    pthread_mutex_lock(&led_mutex);
    LEDstatus =  (LEDstatus << 1) | (LEDstatus >> (8 - 1));
    pthread_mutex_unlock(&led_mutex);
    
    usleep(SECOND_IN_MS);
}

void buttonState2Loop() {
    pthread_mutex_lock(&led_mutex);
    LEDstatus = 0b00000000;
    pthread_mutex_unlock(&led_mutex);
    for(int j = 0 ; j < 8 ; j++){        
        if(LEDstatus >> j & 1 == 1){
            turnOn(pin[j]);
        }else{
            turnOff(pin[j]);
        }
    }
    usleep(SECOND_IN_MS/2);
    pthread_mutex_lock(&led_mutex);
    LEDstatus = 0b11111111;
    pthread_mutex_unlock(&led_mutex);
    for(int j = 0 ; j < 8 ; j++){
        if(LEDstatus >> j & 1 == 1){
            turnOn(pin[j]);
        }else{
            turnOff(pin[j]);
        }
    }
    usleep(SECOND_IN_MS/2);
    pthread_mutex_lock(&led_mutex);
    LEDstatus = 0b00001111;
    pthread_mutex_unlock(&led_mutex);
    for(int j = 0 ; j < 8 ; j++){
        if(LEDstatus >> j & 1 == 1){
            turnOn(pin[j]);
        }else{
            turnOff(pin[j]);
        }
    }
    usleep(SECOND_IN_MS/2);
    pthread_mutex_lock(&led_mutex);
    LEDstatus = 0b11110000;
    pthread_mutex_unlock(&led_mutex);
    
    for(int j = 0 ; j < 8 ; j++){
        if(LEDstatus >> j & 1 == 1){
            turnOn(pin[j]);
        }else{
            turnOff(pin[j]);
        }
    }
    usleep(SECOND_IN_MS/2);
}

void buttonState3Loop() {
    pthread_mutex_lock(&led_mutex);
    LEDstatus = 0b11111111;
    pthread_mutex_unlock(&led_mutex);
    for(int j = 0 ; j < 8 ; j++){        
        if(LEDstatus >> j & 1 == 1){
            turnOn(pin[j]);
        }else{
            turnOff(pin[j]);
        }
    }
    usleep(SECOND_IN_MS);
    pthread_mutex_lock(&led_mutex);
    LEDstatus = 0b00000000;
    pthread_mutex_unlock(&led_mutex);
    for(int j = 0 ; j < 8 ; j++){        
        if(LEDstatus >> j & 1 == 1){
            turnOn(pin[j]);
        }else{
            turnOff(pin[j]);
        }
    }
    usleep(SECOND_IN_MS);
}

void *child(){ 

    while(1) {
        int state;
        pthread_mutex_lock(&mutex);
        state = buttonState;
        pthread_mutex_unlock(&mutex);
        

        if (state == 0) {
            printf("state 0 \n");
            buttonState0Loop();
        }
        else if (state == 1) {
            printf("state 1 \n");
            buttonState1Loop();
        }
        else if (state == 2) {
            printf("state 2\n");
            buttonState2Loop();
        }
        else {
            printf("state 3 \n");
            buttonState3Loop();
        }
        
        
    }
    pthread_exit(NULL);
}

void *button_function(){ 

    while(1) {
        // printf("btn\n");
        int pressed;
        pthread_mutex_lock(&mutex);
        pressed = buttonPressed(14);
        pthread_mutex_unlock(&mutex);
        
        if (pressed && lastButtonPressedState == 0) {
            printf("pressed = %i\n", pressed);
            lastButtonPressedState=1;
            buttonState = (buttonState+1) % 4;

            pthread_mutex_lock(&led_mutex);
            if (buttonState == 0) {
                LEDstatus = 0b00000001;
            }
            else if (buttonState == 1) {
                LEDstatus = 0b00000011;
            }
            else if (buttonState == 2) {
                LEDstatus = 0b00000011;
            }
            else if (buttonState == 3) {
                LEDstatus = 0b00000000;
            }
            
            pthread_mutex_unlock(&led_mutex);

        }
        else {
            lastButtonPressedState = 0;
        }
        
        usleep(SECOND_IN_MS/4);
        
    }
    pthread_exit(NULL);

}

int main(){    
    printf("main");
    pthread_mutex_init(&mutex,0);
     pthread_mutex_init(&led_mutex,0);
    pthread_t led_thread;
    pthread_t button_thread;
    
    pthread_create(&button_thread,NULL,button_function,NULL); 
    pthread_create(&led_thread,NULL,child,NULL); 
    
    pthread_join(led_thread,NULL);
    pthread_join(button_thread,NULL);
    return 0;
}
