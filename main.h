#include <stdio.h>
#include <stdlib.h>

int gpio_export(unsigned int gpio){
    int len;
    char buff[64];
    FILE *fp ;
    fp = fopen("/sys/class/gpio/export","w");
    if (fp == NULL){        
        perror("gpio/export");
        return fp;
    }

    len = snprintf(buff,sizeof(buff),"%d",gpio);
    fwrite(buff,1,sizeof(buff),fp);
    fclose(fp);
    return 0;    
}
int gpio_unexport(unsigned int gpio){
    int len;
    char buff[64];
    FILE *fp = fopen("/sys/class/gpio/unexport","w");
    if (fp==NULL){
        perror("gpio/unexport");
        return fp;
    }

    len = snprintf(buff,sizeof(buff),"%d",gpio);
    fwrite(buff,sizeof(buff),1,fp);
    fclose(fp);
    return 0;    
}
int gpio_set_dir(unsigned int gpio,char  *dirStatus){
    char buff[64];
    
    snprintf(buff,sizeof(buff),"/sys/class/gpio/gpio%d/direction",gpio);

    FILE *fp = fopen(buff,"w");
    if(fp==NULL){
        perror("gpio/direction");
        return fp;
    }
    if( strcmp(dirStatus,"out") == 0){
        
        fwrite(dirStatus,sizeof(dirStatus),1,fp);
    }else{
        char s[]="in";
        fwrite(s,sizeof(s),1,fp);
    }
    fclose(fp);
    return 0;
    
}
int gpio_set_value(unsigned int gpio,int value){
    char buff[64];
    snprintf(buff,sizeof(buff),"/sys/class/gpio/gpio%d/value",gpio);

    FILE *fp=fopen(buff,"w");
    if(fp==NULL){
        //perror("gpio/set-value");
        return fp;
    }
    if(value == 0 ){
        char s[] =  "0";
        fwrite(s,sizeof(s),1,fp);
    }else{
        char s[] =  "1";
        fwrite(s,sizeof(s),1,fp);   
    }
    fclose(fp);
    return 0;
}

int gpio_get_value(unsigned int gpio){
    char buff[64];
    snprintf(buff,sizeof(buff),"/sys/class/gpio/gpio%d/value",gpio);

    FILE *fp=fopen(buff,"r");
    if(fp==NULL){
        //perror("gpio/get-value");
        return fp;
    }
    char value;
    fread(&value, sizeof(char), 1, fp);
    
    fclose(fp);
    return (int)(value-'0');
}
