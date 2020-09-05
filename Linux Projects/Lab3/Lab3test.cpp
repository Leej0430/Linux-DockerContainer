#include <string>
#include <iostream>
#include <thread>
#include<string>
#include<mutex>
#include<algorithm>
#include<fstream>
#include <sstream>
#include<cstdio>
#include<fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <vector>
#include <sys/types.h>
#include<sys/stat.h>
#include<string.h>
#include<unordered_set>

using namespace std;
int numberofnumbers;
mutex m;
int fd;
unordered_set<int> v;
void lockfile(){
    fd =open("test.txt",O_RDWR);
    struct flock lock;
    memset(&lock,0,sizeof(lock));
    lock.l_type =F_RDLCK;
    fcntl(fd,F_SETLKW,&lock); // i can set the range
}

void unlockfile(){
    struct flock lock;
    lock.l_type =F_UNLCK;
    fcntl(fd,F_SETLKW,&lock);
    close(fd);
}
void lockfile2(int i , int j ){
    fd =open("test.txt",O_RDWR);
    struct flock lock;
    memset(&lock,0,sizeof(lock));
    //F_RDLCK FOR READING F_WRLCK for writing
    lock.l_start=11*i;
    lock.l_len=11*(j-i+1)-1;
    lock.l_type =F_RDLCK-1;
    fcntl(fd,F_SETLKW,&lock); 
}

void unlockfile2(int i, int j){
    struct flock lock;
    lock.l_type =F_UNLCK;
    lock.l_start=11*i;
    lock.l_len=11*(j-i+1);
    fcntl(fd,F_SETLKW,&lock);
    close(fd);
}
bool finish(){
   lockfile();
    fstream myFile;
    myFile.open("test.txt");
    float numbers[numberofnumbers];
    string a;
    int count =0;
    while(myFile>>a){
            numbers[count] = stof(a);
        if(count>=1){
            if (numbers[count]<numbers[count-1]){
                return false;
            }
        }

        count++;
    }
    myFile.close();
    unlockfile();
    return true;

}
bool checkSorted(float numbers[],int size){
    for( int i =0;i<size-1;i++){
        if(numbers[i]>numbers[i+1]){
            return false;
        }
    }
    return true;
 }
void bubble (float msg[],int size ){
    int i ,j;
    for(  i =0;i<size-1;i++){
        for(  j =0;j<size-i-1;j++){
            if(msg[j]>msg[j+1]){
                swap(msg[j],msg[j+1]);
            }
        }
    }

    
}
void insertion(float msg[],int size){
    int i,j;
    float temp;
    for(  i =1;i<size;i++){
        temp = msg[i];
        j=i-1;
        while(j>=0&&msg[j]>temp){
            msg[j+1]=msg[j];
            j=j-1;
        }
        msg[j+1]=temp;
    }
  
}
void swap(float* a, float* b) 
{ 
    float t = *a; 
    *a = *b; 
    *b = t; 
} 
int partition(float msg[],int low,int high){
    int pivot = msg[high];
    int i =(low-1);
    for( int j =low;j<=high-1;j++){
        if( msg[j]<=pivot){
            i++;
            swap(&msg[i],&msg[j]);
        }
    }
            swap(&msg[i+1],&msg[high]);
    return (i+1);
}
void quick(float *msg, int low, int high) 
{ 
    if(low<high){
    int p = partition(msg,low,high);
    quick(msg,low,p-1);
    quick(msg,p+1,high);
    }

} 
void Displayfile(){
    fstream f ("test.txt");
    string a;
    cout<<"Element in testfile: ";
    while(f>>a){
        cout<<a<<"    ";
    }
    cout<<"\n";
}

void sub1(int i,int j,float numbers[]){
    fd = open("test.txt", O_RDWR); 
    char buff[7];
    char trash[4];
    int size =j-i+1;
    lseek(fd,11*(i),SEEK_SET);
    for( int k=0;k<size;k++){
        read(fd,buff,7);
        string str(buff);
        read(fd,trash,4);
        numbers[k]=stof(str);        
    }
    close(fd);

}

void sub2(int ran, float numbers[],int size){

    if(ran==0) {bubble(numbers,size); }   
    if(ran==1){insertion(numbers,size); }
    if(ran==2){quick(numbers,0,size-1); }

}

void sub3(int i, int j, float numbers[]){

    int size = j-i+1;
    fd = open("text.txt",O_RDWR);       
    int count =0;
    char buff[7]; 
    for( int k=i;k<=j;k++){          
     string s = to_string(numbers[count]);
    lseek(fd,11*k,SEEK_SET);
     while(s.length()<7){s+="0";}
     while(s.length()>7){s.pop_back();}
    strcpy(buff,s.c_str());
    write(fd,buff,strlen(buff));
    count++;
    }
    close(fd);

 }
 bool overlap(int i, int j){
     for( int x=i;x<=j;x++){
         if(v.find(x) != v.end()){
                return true;
         }
     }
     return false;
 }

void task(int id )
{   
     while( finish()==false){
       
             int i =rand() %numberofnumbers;
             int j = (rand()%(numberofnumbers-i))+i;       
             int ran =rand()%3;    
            int size=j-i+1;
            float numbers[size];
            int mutexflag=0;
          //  m.lock();
       //    if(finish()==false){
            lockfile();
           sub1(i,j,numbers);
           
           sub2(ran,numbers,size);
           
           sub3(i,j,numbers);
             
           unlockfile();       
        //    }
            
        //   m.unlock();
            
            

   }
  
}


int main(int argc, char *argv[])
{  
    ofstream MyFile("test.txt");
    ofstream Original("Original.txt");
    numberofnumbers =stoi(argv[1]);

    for( int i =0;i<numberofnumbers;i++){
        float r = ((float)rand()/(float)RAND_MAX)*10;

        string s =to_string(r);
        while(s.length()<7){s+="0";}
        while(s.length()>7){ s.pop_back();}
       MyFile<< s<<"    ";
       Original<< s<<"    ";
    }

    MyFile.close();
    Original.close();
    cout<< "How many thread you want?";
    int numThread;
    cin>>numThread;
    
    vector<thread> v;

      for( int k =1;k<=numThread;k++){
             v.push_back(thread(task,k));
       
             }
	for (thread & th : v)
	{   

		if (th.joinable()){ 

            th.join();
            
        }
    }
    
    cout<< "Finish Sorting!!"<<endl;

    


}
