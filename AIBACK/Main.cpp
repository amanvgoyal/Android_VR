#include <iostream>
#include "Token.h"
#include "Parser.h"
#include <cassert>
#include <string>
#include <sstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>      
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#include <vector>
#include <ctype.h>
#include "Board.h"

using namespace std;

/*string parse(string S);*/
int server();
Parser run;
int main(int argc,  char** argv) {
    server();
/*    printf("--beginning of program\n");
    int counter = 0;
    pid_t pid = fork();

    if (pid == 0)
    {
        // child process
        //game.start();
    }
    else if (pid > 0)
    {
        // parent process
        server();
    }
    else
    {
        printf("fork() failed!\n");
        return 1;
    }
    printf("--end of program--\n");*/
  return 0;
}
/*string parse(string S){
  Parser dp;
  const char * c = S.c_str();
    YY_BUFFER_STATE bp = yy_scan_string(c);
    yy_switch_to_buffer(bp);
    yylex();

    string temp = dp.par_program(tokens,errors);
    yy_delete_buffer(bp);
    errors =0;
    while(!tokens.empty()){
      tokens.pop();
    }
    dp.par_empty();
    return temp;

}*/
int server()
{     
        int sock, connected, bytes_recieved , truea = 1;  
        string send_data;
        char recv_data[1024];       

        struct sockaddr_in server_addr,client_addr;    
        socklen_t sin_size;
        
        if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
            perror("Socket");
            exit(1);
        }

        if (setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&truea,sizeof(int)) == -1) {
            perror("Setsockopt");
            exit(1);
        }
        
        server_addr.sin_family = AF_INET;         
        server_addr.sin_port = htons(5000);     
        server_addr.sin_addr.s_addr = INADDR_ANY; 
        bzero(&(server_addr.sin_zero),8); 

        if (bind(sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr))
                                                                       == -1) {
            perror("Unable to bind");
            exit(1);
        }

        if (listen(sock, 5) == -1) {
            perror("Listen");
            exit(1);
        }
        
          printf("\nTCPServer Waiting for client on port 5000");
        fflush(stdout);


        while(1)
        {  

            sin_size = sizeof(struct sockaddr_in);

            connected = accept(sock, (struct sockaddr *)&client_addr,&sin_size);

            printf("\n Connection from (%s , %d)",
                   inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
            cout<<"\n";
            int count =-1;
            while (1)
            {
              bool done = false;
              if(count ==-1){
                do{
                send_data = "Password:";
                send(connected, send_data.c_str(),send_data.length(), 0);
                memset(recv_data, 0, sizeof recv_data);
                bytes_recieved = recv(connected,recv_data,1024,0);
                recv_data[bytes_recieved] = '\0';
                string temp = recv_data;
                temp.erase(temp.begin()+8,temp.end());
                if(run.check_pass(temp)){
                      done = true;
                      count++;
                }
                else{
                  send_data = "\nPassword Incorrect, Try Again:";
                  send(connected, send_data.c_str(),send_data.length(), 0);
                  memset(recv_data, 0, sizeof recv_data);
                  bytes_recieved = recv(connected,recv_data,1024,0);
                  recv_data[bytes_recieved] = '\0';
                  string temp = recv_data;
                  temp.erase(temp.begin()+8,temp.end());
                  if(run.check_pass(temp)){
                        done = true;
                        count++;
                  }

                }
                  
              }while(!done);
                send_data.clear();
                    send_data = "WELCOME:";
                send(connected, send_data.c_str(),send_data.length(), 0);
              }
              memset(recv_data, 0, sizeof recv_data);
              bytes_recieved = recv(connected,recv_data,1024,0);
              recv_data[bytes_recieved] = '\0';
              string temp;
              string check_pass = recv_data;
               for (std::string::size_type i=0; i<check_pass.length(); ++i)
                  {
                    if (iswprint(check_pass[i]))
                            temp+=check_pass[i];
                  }
              //temp.erase(temp.begin()+temp.length()-2,temp.end());
              cout<<"Parsing this:"<<temp<<endl;
              if(temp.length()>0){
                  string value = run.parse(temp);
                  if(value=="FALSE"){
                    send_data.clear();
                    send_data = "INVALID INPUT\n";
                    send(connected, send_data.c_str(),send_data.length(), 0);
                  }
                  else if(value == "EXIT"){
                        send_data.clear();
                        send_data = "BYE\n";
                        send(connected, send_data.c_str(),send_data.length(), 0);
                        close(connected);
                        break;
                  }
                  else{
                    send_data.clear();
                    send_data = value+"\n";
                    send(connected, send_data.c_str(),send_data.length(), 0);
                  }
              }
              cout<<"\n RECIEVED DATA = " <<temp;
              if(recv_data!=" "||recv_data!="\0"){
              send_data.clear();
              send_data ="OK\n";
              cout<<"\n SENDING: "<<send_data;
              send(connected, send_data.c_str(),send_data.length(), 0); 
            }
              fflush(stdout);
            }
        }       

      close(sock);
      return 0;
} 


