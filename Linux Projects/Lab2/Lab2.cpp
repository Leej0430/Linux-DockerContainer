#include <iostream>
#include <fstream>
#include<string>
#include<unistd.h>
#include<sys/types.h>
#include<algorithm>
#include<vector>
#include<sys/wait.h>

using namespace std;
bool compare(char a, char b){return a<b;}
int main(int argc, char *argv[]){
    string allText;
    string str;

 ifstream file(argv[1]);
    //error handling    
    if(!file){
        cerr<< "File doesn't exists \n";
        return 0;
    }
    // read file line by line and make it one string
    while(!file.eof()){   
        getline(file,str);
    
        allText +=str;
    }
    file.close();
    
    //remove all white space
    allText.erase(remove_if(allText.begin(),allText.end(),::isspace),allText.end());


    vector<char> vec;
    for( int i =0;i<allText.length();i++){
        if(isalpha(allText[i])){
            if(islower(allText[i])){allText[i]=allText[i]-32;}
            if(find(vec.begin(),vec.end(),allText[i]) ==vec.end()){
                vec.push_back(allText[i]);
            }
        }
    }
    sort(vec.begin(),vec.end(),compare);
for( int i =0;i<vec.size();i++){
    int fd[2];
    int val = 0;

    
    pipe(fd);
    pid_t pid;
    
    if (fork() == 0)
    {
        
        close(fd[0]);

        for( int k =0;k<allText.length();k++){
            if(allText[k]==vec[i]|| (allText[k]-32)==vec[i]){
                val ++;
            }
        }
    
        write(fd[1], &val, sizeof(val));
      
        

       
        close(fd[1]);
        exit(0);
            }
    else
    {   
        pid = wait(NULL);
        
        close(fd[1]);

       
        read(fd[0], &val, sizeof(val));

        cout<< vec[i]<<"-->"<< val<<endl;

        
        close(fd[0]);   
    }
 
}
return 0;
}
    

    



