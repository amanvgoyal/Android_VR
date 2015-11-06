#include <sys/types.h>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <string>
#include <vector>
using namespace std;

void parse(string S);

int main()
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
                if(temp=="password"){
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
                  //cout<<"TEMP:"<<temp;
                  if(temp=="password"){
                        done = true;
                        count++;
                  }

                }
                  
              }while(!done);
                send_data.clear();
                send_data = "WELCOME:";
                send(connected, send_data.c_str(),send_data.length(), 0);
                //bytes_recieved = recv(connected,recv_data,1024,0);
                //recv_data[bytes_recieved] = '\0';
                //string temp = recv_data;
                //cout<<"\n"<<temp<<endl;
              }
              memset(recv_data, 0, sizeof recv_data);
              bytes_recieved = recv(connected,recv_data,1024,0);
              recv_data[bytes_recieved] = '\0';
              string temp = recv_data;
              cout<<"\n RECIEVED DATA = " <<recv_data;
              if(recv_data!=" "||recv_data!="\0"){
              send_data.clear();
              send_data ="OK\n";
              cout<<"\n SENDING: "<<send_data;
             /* vector<string> lol;
              for(int i =0;i<10;++i){
                string temp;
                lol.push_back("-------"+to_string(i)+"-----------\n");
              }
              for(int i =0;i<10;++i){
                send(connected, lol[i].c_str(),lol[i].length(), 0);
              }  */            
              send(connected, send_data.c_str(),send_data.length(), 0); 
            }
              fflush(stdout);
            }
        }       

      close(sock);
      return 0;
} 
