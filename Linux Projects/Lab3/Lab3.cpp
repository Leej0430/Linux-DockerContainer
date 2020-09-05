#include <string>
#include <iostream>
#include <thread>
#include<mutex>
#include<fstream>
#include<vector>
#include<fcntl.h>
#include <unistd.h>
#include<string.h>
using namespace std;
int numberofnumbers;
mutex m;
int fd;
int fd2;

void lockfile(){
    fd =open("Sorted.txt",O_RDWR);
    struct flock lock;
 
    memset(&lock,0,sizeof(lock));
    
    lock.l_type =F_RDLCK;
    fcntl(fd,F_SETLKW,&lock); 
}

void unlockfile(){
    struct flock lock;
    lock.l_type =F_UNLCK;
    fcntl(fd,F_SETLKW,&lock);
    close(fd);
}
void lockfile2(){
    fd =open("After.txt",O_RDWR);
    struct flock lock;
 
    memset(&lock,0,sizeof(lock));
   
    lock.l_type =F_RDLCK;
    fcntl(fd2,F_SETLKW,&lock); 
}

void unlockfile2(){
    struct flock lock;
    lock.l_type =F_UNLCK;
    fcntl(fd2,F_SETLKW,&lock);
    close(fd2);
}
bool finish(){
    lockfile();
    fstream myFile;
    myFile.open("Sorted.txt");
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
    fstream f ("Sorted.txt");
    string a;
    cout<<"Element in Sortedfile: ";
    while(f>>a){
        cout<<a<<"    ";
    }
    cout<<"\n";
}

void sub1(int i,int j,float numbers[]){
 
    lockfile();

    fstream myFile ("Sorted.txt");
    int count =0;
    int count2 =0;
    string a;

    while(myFile>>a){
        if( count>=i && count<=j){
 
            numbers[count2] = stof(a);
            count2++;
            
        }
        count++;
    }

    myFile.close();

    unlockfile();
 
}

void sub2(int ran, float numbers[],int size){

    if(ran==0) {bubble(numbers,size); }   
    if(ran==1){insertion(numbers,size); }
    if(ran==2){quick(numbers,0,size-1); }

}

void sub3(int i, int j, float numbers[]){
lockfile();
lockfile2();
fstream mFile ("Sorted.txt");
ofstream fixed("After.txt");
  int  count=0;
  int  count2=0;
    string a;
   while(mFile>>a){
        if( count>=i && count<=j){
            float tmp = numbers[count2];
            count2++;
            fixed<<tmp<<"    ";
        }
        else{fixed<<a<<"    ";   }

        count++;
    }
    mFile.close();
   rename("After.txt","Sorted.txt");
   unlockfile();
   unlockfile2();
 }

void task(int id )
{   
    while( finish()==false){
       
            int i =rand() %numberofnumbers;
            int j = (rand()%(numberofnumbers-i))+i;
            int ran =rand()%3;    
            int size=j-i+1;

            float numbers[size];

            string kind;
            if(ran ==0){kind ="BubblesSort";}
            if(ran==1){kind="InsertionSort";}
            if(ran==2){kind="QuickSort";}
           
            m.lock();
            cout<<"Thread ["<<id<<"] take the data for "<<kind<<"(Range "<<i<<" ~ "<<j<<")\n";
            Displayfile();
            if(finish()==false){
            if(size !=1){     
            sub1(i,j,numbers);

            if(checkSorted(numbers,size)!=true){
            cout<<"Before: ";
            for( int k=0;k<size;k++){
                cout<<numbers[k]<<"    ";
            }
            cout<<"\n";

            sub2(ran,numbers,size); 
                cout<<"After:  ";
                        for( int k=0;k<size;k++){
                cout<<numbers[k]<<"    ";
            }
            cout<<"\n";
            
            sub3(i,j,numbers); 

            }else{ cout<<"This range is already sorted"<<endl;} 

            }else{cout<< "Don't need to Sort because the size is 1"<<endl;}
           
            }else{cout<<"All the element in the file is sorted"<<endl;}
            
            Displayfile();
            m.unlock();
           cout<<"\n";       
    }
  
}


int main(int argc, char *argv[])
{   

    ofstream MyFile("Sorted.txt");
    ofstream Unsorted("Unsorted.txt");
    numberofnumbers =stoi(argv[1]);
    
    for( int i =0;i<numberofnumbers;i++){
        float r = (float)(rand())/(float)(RAND_MAX)*10;
       MyFile<< r<<"    ";
       Unsorted<< r<<"    ";
    }

    MyFile.close();
    Unsorted.close();
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