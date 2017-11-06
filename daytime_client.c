#include<stdio.h>
#include<sys/socket.h>
#include<string.h>
#include<netinet/in.h>
#include <arpa/inet.h>
#include<unistd.h>

#define MAXLINE 4096
int main( int argc, char * argv[]) {
int sockfd;
/* the socket file descriptor */
int nbytes;
/* number of bytes actually read from the socket */
char recvline[ MAXLINE + 1]; /* to receive the day and time string */
struct sockaddr_in servaddr; /* socket internet address structure */
if( argc != 2) {
fprintf( stderr, "usage: %s <IPaddress>\n", argv[ 0]);
return 1;
}
if( (sockfd = socket( AF_INET, SOCK_STREAM, 0)) < 0) {
perror( "socket error");
return 1;
}
bzero( &servaddr, sizeof( servaddr));
servaddr.sin_family = AF_INET;
/* should now use PF_INET! */
servaddr.sin_port = htons( 13); /* daytime server 'well known' port */
if( inet_aton( argv[ 1], &servaddr.sin_addr) <= 0) {
/* should now use inet_pton( AF_INET, argv[ 1], &servaddr.sin_addr);
* even older versions use servaddr.sin_addr.s_addr =inet_addr( arrgv[ 1]);!
*/
fprintf( stderr, "inet_aton error for %s\n", argv[ 1]);
return 1;
}
if( connect( sockfd, (struct sockaddr *) &servaddr, sizeof( servaddr)) < 0) {
perror( "connect error");
return 1;
}
while( (nbytes = read( sockfd, recvline, MAXLINE)) > 0) {
recvline[ nbytes] = '\0'; /* end-of-string char */
if( fputs( recvline, stdout) == EOF) {
perror( "fputs error");
return 1;
}
} /* while */
if( nbytes < 0) {
perror( "read error");
return 1;
}
exit( 0);
}