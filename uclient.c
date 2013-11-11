/*				Uclient.c
*/


#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <errno.h>
#include <sys/time.h>		/* for gettimeofday() */
#include <stdlib.h>
#include <strings.h>

/* This is the client program. It creates a socket and initiates a
 * connection with the socket given in the command line. 
 * The form of the command line is `uclient hostname portnumber'. 
 */

main(int argc,char *argv[])
{
  int sock;
  struct sockaddr_in server, client;
  struct hostent *hp, *gethostbyname();
  char buf[1024];
  int rval;

  /* create socket */

  sock = socket(AF_INET, SOCK_DGRAM, 0);
  if(sock < 0) {
    perror("Opening datagram socket");
    exit(1);
  }
  
  /* now bind client address. Port is wildcard, doesn't matter. */
  client.sin_family = AF_INET;
  client.sin_addr.s_addr = INADDR_ANY;
  client.sin_port = 0;
  if (bind(sock, (struct sockaddr *)&client, sizeof(client)) < 0) {
    perror("binding socket name");
    exit(1);
  }

  server.sin_family = AF_INET;
  hp = gethostbyname(argv[1]);
  if(hp == 0) {
    printf("%s: unknown host\n",argv[1]);
    exit(2);
  }

  bcopy(hp->h_addr, &server.sin_addr, hp->h_length);
  server.sin_port = htons(atoi(argv[2]));

  /* create a packet */
  sprintf(buf, "%d\0", 100);

  if ((rval=sendto(sock,buf,sizeof(buf),0,(struct sockaddr *)&server,sizeof(server))) < 0) {
    perror("writing on datagram socket");
  } else {
    printf("Sent packet of length %d\n", rval);
  }

  close(sock);
}
