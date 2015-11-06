#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define PORT 4897
#define QLEN 200
#define BLEN 256

int visits;

main(int argc, char *argv[])
{
  struct sockaddr_in self, client;
  struct hostent *cp;
  int sd, td;
  int len;
  char buf[BLEN];

  sd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
  
  memset((char *) &self, 0, sizeof(struct sockaddr_in));
  self.sin_family = AF_INET;
  self.sin_addr.s_addr = INADDR_ANY;
  self.sin_port = htons((u_short) PORT);

  bind(sd, (struct sockaddr *) &self, sizeof(struct sockaddr_in));

  listen(sd, QLEN);

  while (1) {
    len = sizeof(struct sockaddr_in);
    td = accept(sd, (struct sockaddr *) &client, &len);

    cp = gethostbyaddr((char *) &client.sin_addr, sizeof(struct in_addr), AF_INET);
    printf("Connected from %s\n", cp->h_name);

    visits++;
    sprintf(buf, "This server has been contacted %d time(s).\n", visits);
    send(td, buf, strlen(buf), 0);

    close(td);
  }
}
