// $Id: main.cpp,v 1.16 2015-07-26 08:23:10-07 - - $

#include <cstdlib>
#include <exception>
#include <iostream>
#include <string>
#include <unistd.h>
#include <fstream>

using namespace std;

#include "listmap.h"
#include "xpair.h"
#include "util.h"

using str_str_map = listmap<string,string>;
using str_str_pair = str_str_map::value_type;

string whitespaces (" \t\f\v\n\r");
const string cin_name = "-";
str_str_map test;

void scan_options (int argc, char** argv) {
   opterr = 0;
   for (;;) {
      int option = getopt (argc, argv, "@:");
      if (option == EOF) break;
      switch (option) {
         case '@':
            traceflags::setflags (optarg);
            break;
         default:
            complain() << "-" << (char) optopt << ": invalid option"
                       << endl;
            break;
      }
   }
}

void printPair(string key,string value){
   cout<<key<<" = "<<value<<endl;
}

void processInstructions(istream& infile, string filename){
   int instructionNo=0;
   string cmd;
   string key;
   string value;
   for(;;){
      //take in input
      //use of getline and check for eof is taken from yshell's main
      //as written by Prof. Mackey
      getline(infile,cmd);
      if(infile.eof())
         break;
      ++instructionNo;
      cout<<filename<<": "<<instructionNo<<": "<<cmd<<endl;
      if(!cmd.empty()){
         //trim the whitespaces
         //find the point where we stop encoutering whitespaces at the
         //front
         //find the point from which we only encounter whitespaces at
         //the back
         cmd = cmd.substr(cmd.find_first_not_of(" "),cmd.size());
         cmd = cmd.substr(0,cmd.find_last_not_of(whitespaces)+1);
         //split on "="
         //before the "="
         key = cmd.substr(0,cmd.find_first_of("="));
         //after the "=",but only initialized if "=" exists
         size_t splitPT = cmd.find("=");
         bool splitable = (int)splitPT!=-1;
         if(splitable)
            value = cmd.substr(cmd.find_first_of("=")+1,cmd.size());
         else
            value="";
         //choose which command to execute
         char f =cmd.front();
         bool emptyVal=value.size()==0;
         //if the command starts with "=" we know we have to print stuff
         if(f=='='){
            if(emptyVal){
               for(auto itor=test.begin();itor!=test.end();++itor){
                  printPair(itor->first,itor->second);
               }
            }
            else{
               for(auto itor=test.begin();itor!=test.end();++itor){
                  string iv=itor->second;
                  if(iv==value)
                     printPair(itor->first,iv);
               }
            }
         }
         //if the command doesn't start with "=" it's either a key 
         //command of a comment (in which case we don't do anything)
         else if(f!='#'){
            auto fk=test.find(key);//returns the pair associated w/ key
            bool kf=fk!=test.end();//true if the key was found
            if(emptyVal){
               if(splitable){
                  if(kf)
                     fk=test.erase(fk);
               }
               else{
                  if(kf)
                     printPair(fk->first,fk->second);
                  else
                     cerr<<key<<": key not found"<<endl;
               }
            }
            else{
               const str_str_pair pair (key,value);
               fk = test.insert(pair);
               printPair(fk->first,fk->second);
            }
         }
      }
   }
}

int main (int argc, char** argv) {
   sys_info::set_execname (argv[0]);
   scan_options (argc, argv);
   if(argc>1){
      for(int argi=1;argi<argc;++argi){
         string filename=argv[argi];
         if(filename==cin_name)
            processInstructions(cin,cin_name);
         else{
            ifstream infile (filename);
            if(infile.fail()){
               cerr<<sys_info::get_execname()<<": "<<filename
                   <<": No such file or direcotry"<<endl;
               sys_info::set_exit_status(1);
            }
            else{
               processInstructions(infile,filename);
               infile.close();
            }
         }
      }
   }
   else
      processInstructions(cin,cin_name);
   return sys_info::get_exit_status();
}
