#include<stdio.h>
#include<sys/socket.h>
#include<string.h>
#include<netinet/in.h>
#include<unistd.h>

#include <time.h>
#define MAXLINE 4096
#define MAXQ 10
int main( int argc, char * argv[]) {
int listenfd;
int connfd;
struct sockaddr_in servaddr;
struct sockaddr_in cliaddr;
int lenstruct;
char buff[ MAXLINE];
time_t ticks;
listenfd = socket( AF_INET, SOCK_STREAM, 0);
if( listenfd < 0) {
perror( "socket error");
return 1;
}
bzero( &servaddr, sizeof( servaddr));
servaddr.sin_family = AF_INET;
/* should now use PF_INET! */
servaddr.sin_port = htons( 60000);

servaddr.sin_addr.s_addr = htonl( INADDR_ANY);
lenstruct = sizeof( servaddr);
/* bind the inet address structure to the listening socket ready for use */
if( (bind( listenfd, (struct sockaddr *) &servaddr, lenstruct)) < 0) {
perror( "bind error");
return 1;
}
/* actually put the socket in listen mode and set the q length */
if( listen( listenfd, MAXQ) < 0) {
perror( "listen error");
return 1;
}

for( ; ; ) { 

connfd = accept( listenfd, (struct sockaddr *) &cliaddr, &lenstruct);
if( connfd < 0) {
	perror( "accept error");
return 1;
}
/* get the date and time, and write it to the connected socket */
ticks = time( NULL);
snprintf( buff, sizeof( buff), "%.24s\r\n", ctime( &ticks));
if( write( connfd, buff, strlen( buff)) < 0) {
fprintf( stderr, "write error\n");
return 1;
}
/* close the connected socket */
close( connfd);
} /* for ever */
close( listenfd);
}