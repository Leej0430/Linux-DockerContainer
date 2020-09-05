#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<fcntl.h>
#include<unistd.h>
#include<errno.h>
#include<sstream>
#include<string>
#include<iostream>
using namespace std;
int main(){
    //2,3,4,5
     int fd = open("test.txt",O_RDWR);    
    string s ="1111111";
    int count =0;
    for( int k=0;k<50;k++){
     char buff[7];  
   //  ostringstream ss;
  //   ss<<numbers[count];
    lseek(fd,11*k,SEEK_SET);
  //   string s(ss.str());
     if(s.length()==6){ s+="0";}
    strcpy(buff,s.c_str());
    write(fd,buff,strlen(buff));
    count++;
    }
}