/*				userver.c
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <errno.h>
#include <sys/time.h>           /* for gettimeofday() */
#include <stdlib.h>
#include <strings.h>

#define TRUE 1

/* This is the server program. It opens a socket with the given port
 * and then begins an infinite loop. Each time through the loop it 
 * accepts a pkt and prints out the contents and the current time. 
 * Command line is `userver portnumber'.  
 */

main(int argc,char *argv[])
{
  int sock, length;
  struct sockaddr_in server, client;
  int msgsock;
  char buf[1024];
  int rval, clilen;
  int i, seqno;
  struct timeval rcvtime;
  struct timezone zone;

  /* create socket */

  sock = socket(AF_INET, SOCK_DGRAM, 0);
  if(sock < 0) {
    perror("opening datagram socket");
    exit(1);
  }

  /* name socket using wildcards */

  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons(atoi(argv[1]));
  if(bind(sock, (struct sockaddr *)&server, sizeof(server))) {
    perror("binding socket name");
    exit(1);
  }

  /* find out assigned port number and print out */

  length = sizeof(server);
  if(getsockname(sock, (struct sockaddr *)&server, &length)) {
    perror("getting socket name");
    exit(1);
  }
  printf("Socket has port #%d\n",ntohs(server.sin_port));

  do {
      bzero(buf,sizeof(buf));
      clilen = sizeof(client);
      while ((rval = recvfrom(sock,buf,1024,0,(struct sockaddr *)&client,&clilen))<0){
        perror("receiver recvfrom");
      }
      sscanf(buf, "%d", &seqno);
      /* get current relative time */
      if (gettimeofday(&rcvtime, &zone) < 0) {
    	perror("getting time");
    	exit(1);
      }
      printf(" %ld %ld %d\n", rcvtime.tv_sec, rcvtime.tv_usec, seqno); 
  } while (rval != 0);
  close(sock);
}
