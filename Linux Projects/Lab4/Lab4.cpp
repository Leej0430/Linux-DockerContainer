#include<fstream>
#include<iostream>
#include<vector>
#include<iterator>
#include<unistd.h>
using namespace std;
struct Processor{
 char processname[16];      //16char
 int process_id;        //32 bit int
 char activitystatus;    //1 char
 int CPUbursttime;      //32 bit int
 int baseregister;      //32 bit int
 long  limitregister;    //64 bit long
 char priority;          //1 char
 
};
void printinfo(Processor &pa){

     cout<<"ProcessId -> "<<pa.process_id<<" executed"<<endl;
    cout<<"CPUbursttime -> "<<pa.CPUbursttime<<endl;
    cout<<"Priority -> "<<(int)pa.priority<<endl;
    cout<<endl;

}
void sortvector(vector<Processor> &v1){

    for(int i =0;i<v1.size()-1;i++){
        for(int j=0;j<v1.size()-1-i;j++){
            if((int)v1[j].priority>(int)v1[j+1].priority){
                Processor temp=v1[j];
                v1[j]=v1[j+1];
                v1[j+1]=temp;
            }
        }
    }

}
bool finish(fstream & r,int processors,Processor &p){
r.clear();
r.seekp(0);

for( int i =0;i<processors;i++){
r.read((char*)&p.processname,16);
r.read((char*)&p.process_id,sizeof(int));
r.read((char*)&p.activitystatus,sizeof(char));
r.read((char*)&p.CPUbursttime,sizeof(int));
r.read((char*)&p.baseregister,sizeof(int));
r.read((char*)&p.limitregister,sizeof(long));
r.read((char*)&p.priority,sizeof(char));

if((int)p.activitystatus==1) {return false;}
}

return true;
}

void readprocessor(Processor &p,int location,vector<Processor> &v,fstream &r){
r.clear();
r.seekp(location*38);

r.read((char*)&p.processname,16);
r.read((char*)&p.process_id,sizeof(int));
r.read((char*)&p.activitystatus,sizeof(char));
r.read((char*)&p.CPUbursttime,sizeof(int));
r.read((char*)&p.baseregister,sizeof(int));
r.read((char*)&p.limitregister,sizeof(long));
r.read((char*)&p.priority,sizeof(char));
v.push_back(p);
}
void readprocessor(Processor &p,int location,fstream &r){
r.clear();
r.seekp(location*38);

r.read((char*)&p.processname,16);
r.read((char*)&p.process_id,sizeof(int));
r.read((char*)&p.activitystatus,sizeof(char));
r.read((char*)&p.CPUbursttime,sizeof(int));
r.read((char*)&p.baseregister,sizeof(int));
r.read((char*)&p.limitregister,sizeof(long));
r.read((char*)&p.priority,sizeof(char));
}
int find(Processor &p,int process_id,int processers,fstream &r){

        r.clear();
    r.seekp(0);

    for( int i =0;i<processers;i++){
r.read((char*)&p.processname,16);
r.read((char*)&p.process_id,sizeof(int));
r.read((char*)&p.activitystatus,sizeof(char));
r.read((char*)&p.CPUbursttime,sizeof(int));
r.read((char*)&p.baseregister,sizeof(int));
r.read((char*)&p.limitregister,sizeof(long));
r.read((char*)&p.priority,sizeof(char));

    if((int)p.process_id==process_id){
        return i;
    }
    }
    return -1;
}
void writeprocessor(Processor &p,int location,fstream &r){
           r.clear();
           r.seekp(location*38);

           r.write((char*)&p.processname,sizeof(p.processname));
           r.write((char*)&p.process_id,sizeof(p.process_id));
           r.write((char*)&p.activitystatus,sizeof(p.activitystatus));
           r.write((char*)&p.CPUbursttime,sizeof(p.CPUbursttime));
           r.write((char*)&p.baseregister,sizeof(p.baseregister));
           r.write((char*)&p.limitregister,sizeof(p.limitregister));
           r.write((char*)&p.priority,sizeof(p.priority));
}

Processor pa;
vector<Processor>v;

int main(int argc, char *argv[]){

fstream r;
r.open(argv[1],ios::in|ios ::out|ios::binary);

r.seekg(0,ios::end);
int processers =r.tellg()/38;
cout<<"number of available process ->"<<processers<<endl;
r.seekg(0,ios::beg);


     int totalmemory=0;
    for( int i =0;i<processers;i++){
     readprocessor(pa,i,r);
     totalmemory+= (pa.limitregister-pa.baseregister);
    }
    
cout<<"Total memory allocated by Processors -> "<<totalmemory<<endl;


while(finish(r,processers,pa)==false){

    for( int i =0;i<processers;i++){
     readprocessor(pa,i,v,r);
    }

 int count=0;
 int location=0;
 cout<<"Round Robbin Scheduling!"<<endl;
 while(count<30){
     if(finish(r,processers,pa)==true)break;
     if(v[location].CPUbursttime>=0&&(int)v[location].activitystatus==1){
        
        if(v[location].CPUbursttime>0)v[location].CPUbursttime--;   
         if((int)v[location].CPUbursttime==0){
             char t ='0';
             cout<<v[location].processname<<"-> finish all the bursttime "<<endl;
             v[location].activitystatus=t-48;
             
         }
           writeprocessor(v[location],location,r);
           printinfo(v[location]);
           count++;
           location++;
           sleep(0.5);
     }
     else{location++;}
     if(location==processers)location=0;
     }
    cout<<"Round Robbin Scheduling Done!"<<endl;  
    sortvector(v);
    count=0;location=0;
    int originallocation=0;

 cout<<"Priority Scheduling!"<<endl;
    while(count<30){
    if(finish(r,processers,pa)==true)break;

     if(v[location].CPUbursttime>=0&&(int)v[location].activitystatus==1){
            originallocation=find(pa,v[location].process_id,processers,r);
        if(v[location].CPUbursttime>0)v[location].CPUbursttime--;   
         if((int)v[location].CPUbursttime==0){
             char t ='0';
             cout<<v[location].processname<<"-> finish all the bursttime "<<endl;             
             v[location].activitystatus=t-48;
         }
         writeprocessor(v[location],originallocation,r);
        printinfo(v[location]);
         count++;
         location++;
         
         sleep(0.5);

        if(count%2==0){
            for( int i =0;i<processers;i++){
            if((int)v[i].priority>0){
              v[i].priority=(char)v[i].priority -1;
              if((int)v[i].priority==0){
                 v[i].priority ='\0';
             }
              originallocation = find(pa,v[i].process_id, processers,r);
             writeprocessor(v[i],originallocation,r);
           }
            }
          cout<<"Aging algorithm is implemented"<<endl;
         }

     }
     else{location++;}
     if(location==processers)location=0;
     }
     cout<<"Priority Scheduling Done! "<<endl;
     v.clear();
 }


 cout<<"All processors are done with working"<<endl;

}



  





  



