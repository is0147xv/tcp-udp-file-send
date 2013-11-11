/*				TCPRCV.C
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <errno.h>
#include <sys/time.h>
#include <stdlib.h>
#include <strings.h>

#define TRUE 1

/* This is the receiver program. It opens a socket, sets the receive
window as requested, and then begins an infinite loop. Each time through 
the loop it accepts a connection and prints out messages from it. When 
the connection breaks or a termination message comes through it, the 
program accepts a new connection.  The form of the command line is:
'tcprcv'  */

main()
{
  int sd, length;
  struct sockaddr_in rcvr;
  int sda;
  int buflen = 512;
  char buf[1500];
  int rcvwin, optlen;
  int rval;
  int seqno;

  rcvwin = buflen;

  /* create socket */
  sd = socket(AF_INET, SOCK_STREAM, 0);
  if(sd < 0) {
    perror("opening stream socket");
    exit(1);
  }

  /* name socket using wildcards */
  rcvr.sin_family = AF_INET;
  rcvr.sin_addr.s_addr = INADDR_ANY;
  rcvr.sin_port = 0;
  if(bind(sd, (struct sockaddr *)&rcvr, sizeof(rcvr))) {
    perror("binding socket name");
    exit(1);
  }

  /* find out assigned port number and print out */
  length = sizeof(rcvr);
  if(getsockname(sd, (struct sockaddr *)&rcvr, &length)) {
    perror("getting socket name");
    exit(1);
  }
  printf("Socket has port #%d\n",ntohs(rcvr.sin_port));


  /* Can get same effect by doing reads of the correct size. */
  rcvwin = buflen;

  /* Accept connections from the transmitter */
  listen(sd, 5);
  do {
    sda = accept(sd,0,0);
    if(sda == -1)
      perror("accept");
    else
      do {
        bzero(buf,sizeof(buf));
        if((rval = recv(sda,buf,rcvwin,MSG_WAITALL))<0)
          perror("reading message stream");
        if(rval == 0)
          printf("Ending connection\n");
        else {
	  sscanf(buf, "%d", &seqno);
          printf("Received packet: seqno = %d length = %d\n", seqno, rval); 
        }
      }while (rval != 0);
    close(sda);
  }while (TRUE);
}
