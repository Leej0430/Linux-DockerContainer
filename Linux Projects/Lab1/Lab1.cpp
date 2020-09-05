#include <iostream>
#include<unistd.h>
#include <cstring>
#include <fstream>



using namespace std;

string exit(char* input){
	string ans="";
	

	string temp =input;
	if(temp =="-h"){
	ans= "help";
	}
	else{
		string previous =input;
		input =strtok(NULL," ");
		if(input !=NULL){
			ans="ERROR(more than 1 argument)";
		}
		
		
			else{
			
				for(int i =0;i<previous.length();i++){
				if(isdigit(previous[i])==false){
				ans ="ERROR(non digit parameter)";
			cout<<temp<<" <-None integer exit number"<<endl;
				break;
				}

			}
			if(ans==""){
				ans =previous;
			}
	}
	
	
	}
	return ans;
	
}

string prompt(char *point){
	string newPrompt;

	string temp ="-h";
	if( point ==temp){
		cout<<"prompt <new_prompt> -- will change the current shell prompt"<<endl;
		newPrompt ="cwushell";
			}

	else{
		string previous=point;
		point=strtok(NULL," ");
		if(point !=NULL){
			cout<< "ERROR(more than 1 argument)"<<endl;
			newPrompt ="cwushell";
		}
		else{
			newPrompt =previous;
		}
		
	}
	return newPrompt+"> ";
}
void cpuinfo(char *point){

	string temp =point;
	if(temp=="-h"){
	              cout<<"cpuinfo -switch will print on the screen information based on switch"
		      "\n-c will print the cpu clock\n"
		      "-t will print the cpu type\n"
	              "-n will print the number of cores"<<endl;
	}
	else{	
		ifstream file("/proc/cpuinfo");
		string line;
		string target = "DEFAULT";
	

        if (temp == "-c"||temp=="-t"||temp=="-n") { 
		if(temp=="-c") {target = "cpu MHz";} 
		else if (temp == "-t") { target = "model name"; }
		else { target = "cpu cores"; }
	while(getline(file,line)){
		if(line.find(target)==0){
			cout<<line<<endl;
			break;
		}
	}
	}
	else { 
		cout <<temp <<": ERROR No Switch Option" << endl;
       			}
	
	}
}					


string getoutput(string command){
	string output;
	FILE *get;
	char letter[256];
	get=popen(command.c_str(),"r");
	if(get){
		while(!feof(get)){
			if(fgets(letter,256,get) !=NULL){output.append(letter);};
	
	}
	pclose(get);
	return output;

}
}


string meminfo(char *point){                                                                                                   string ans;

	string temp =point;
	int target=0;
	if(temp =="-h"){
	ans ="help";
	}
	else if(temp =="-c"){
	ans = getoutput("getconf LEVEL2_CACHE_SIZE");
	}
	else if (temp =="-t" || temp =="-u"){
		
		if (temp =="-t"){
		target =6;
		}
		else if (temp =="-u") {target = 2;}

		string s =getoutput("free |grep Mem:");	
		char box[s.length()+1];
	        strcpy(box,s.c_str());
		char *chunk =strtok(box," ");
		
		for( int i =0;i<target;i++){
		chunk =strtok(NULL," ");
		}
		
		ans =chunk;
	}
	else{ans ="Wrong switch";}

	return ans;
}

	
																								      

int main(){
	string command[5]={"exit","prompt","cpuinfo","meminfo","manual"};
	int runningflag=0;
	string prmpt="cwushell> ";

	cout<<"Welcome to cwushell by JuneYeob Lee \n"
	 	"need Help? type 'manual'"<<endl;
	while(runningflag==0){
		cout<<prmpt;
	char input[100];
	cin.getline(input,100);
 	string forSystem =input;	
	char *point;
	point=strtok(input," ");
	if(point ==command[0]){
		point =strtok(NULL," ");
		if(point ==NULL){cout<< "exit code 0"<<endl;
		return 0;}
		else{
		string ans =exit(point);
		 if(ans == "ERROR(more than 1 argument)"){
			cout<<ans<<endl;
		}
		else if (ans =="ERROR(non digit parameter)"){
			cout<<"exit with 0"<<endl;
		       return 0;	
		}
		else if (ans =="help"){
	cout<< "exit <exit number>--terminate this shell with exit number"<<endl;
		}
		else{
			
			cout<<"exit with "<<stoi(ans)<<endl;
			return stoi(ans);
		} 
		}
	}
	else if (point==command[1]){
		point=strtok(NULL," ");

		if(point ==NULL){
		prmpt ="cwushell>";
		}	
		else{
		string ans2=prompt(point);

			prmpt=ans2;
	
		}
	}
	else if (point ==command[2]){
		point=strtok(NULL," ");
	
		if(point ==NULL){
		cout<< "Need Switch; ** Need help? 'cpuinfo -h'"<<endl;
		}
		else{
		cpuinfo(point);}	
	}
	else if (point ==command[3]){
	
		point=strtok(NULL," ");

		if(point ==NULL){
		cout<< "Need switch; ** Need help?  'meminfo -h'"<<endl;
		
		}
		else{
		string x ;
		x= meminfo(point);
		 
	   if (x=="ERROR need switch" || x =="Wrong switch"){
		   cout<<"  -> "<<forSystem<<": " <<x<<endl;		
       		   
		    }
		   else if (x=="help"){
		   cout<<"meminfo -switch -- will print on the screen about memory information based on the switch\n" 
			  " -t will print the total RAM memory available in bytes \n"
			  " -u will print used RAM memory\n "
			  " -c will print the size of the L2 cache/core in bytes"<<endl;
		   }
		  else{ 
		  int tem = stoi(x)*1000;
	   cout<<tem<<" Bytes"<<endl;}
		}
	}
	else if (point ==command[4]){
	
		cout<<"Manual Page\n "
			"1. exit -> exit <Integer>-- will terminates the shell\n"
			" only one argument will be accepted if you put non-Integer number"
			"exit code will be 0\n"
			"2. prompt <new_prompt>-- will change the current shell prompt to the new_prompt\n"
			"default prompt will be 'cwushell'\n"
			"3. cpuinfo <-switch>-- will print on the screen different cpu related information based on the switch.\n"
			"switch options  '-c'(print cpu clock), '-t'(cpu type),and '-n'(number of cores)\n"
			"4. meminfo <-switch>-- will print on the screen different memory related information based on the switch.\n"
		"switch options '-t'(total RAM memory available in byte), '-u'(used RAM memory in bytes), and '-c'(size of the L2 cache in bytes)\n"
			"need more help type 'man -help'"<<endl; 
	
	}
	else{   
 		
		const char *c = forSystem.c_str();		
		system(c);
	
	}
	

	}	
	

	return 0;
}
