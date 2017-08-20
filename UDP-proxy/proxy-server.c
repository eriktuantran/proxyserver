#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#include <stdio.h>
#include <pthread.h>
# include "/usr/include/libsocket/libinetsocket.h"

#define MAX_THREADS 32767
#define PACK_SIZE 270			// maximum size of a packet string
#define BUF_SIZE 256			// maximum size of the buffer in a packet
#define SERVER_PORT 7747

typedef struct Request{
	socklen_t fromlen;
	struct sockaddr_in from;
	char buf[PACK_SIZE];
} Request;

/* Socket Variables - required to be used by all threads */
int sock, length, n;           
struct sockaddr_in server;
int thread_no;

void* handle_request(void*);

int main(int argc, char **argv)
{
	/* create the socket */
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0)
		error("Opening socket");
	length = sizeof(server);
	bzero(&server,length);
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port=htons(SERVER_PORT);

	/* bind port to the IP */
	if(bind(sock, (struct sockaddr *)&server, length)<0) 
		error("binding");

	/* thread variables */
	pthread_t threads[MAX_THREADS];
	int rc = 0;
	
	/* continuously listen on the specified port */
	while(1){
		struct Request *request = (Request*)malloc(sizeof(Request));
		request->fromlen = sizeof(struct sockaddr_in);
		
		n = recvfrom(sock, request->buf, PACK_SIZE, 0, (struct sockaddr *)&request->from, &request->fromlen);
		if(n < 0)
			error("recvfrom");
		
		printf("this is ERROR");
		printf("\nTUAN: [%s][%d]\n", request->buf, sizeof(request->buf) );
		/* invoke a new thread and pass the recieved request to it */
		rc = pthread_create(&threads[thread_no], NULL, handle_request, (void*)request);
		if(rc){
			printf("A request could not be processed\n");
		}
		else{
			thread_no++;
		}
	}
	return 0;
}
void* handle_request(void *req){
	Request *request = (Request*)req;

	char buffer[BUF_SIZE], filename[BUF_SIZE];

		printf("\n\n\nBEGIN------%d",1);

		// Send REQUEST_ACK/REQUEST_REGRET //
		n = sendto(sock,request->buf, strlen(request->buf),0,(const struct sockaddr *)&request->from,request->fromlen);

		//sendToHercules();

			int sock2;
			struct sockaddr_in server, from;
			struct hostent *hp;
			printf("HERE---------");
			/* creating socket */
			sock2 = socket(AF_INET, SOCK_DGRAM, 0);
			if (sock2 < 0)
				printf("socket");

			server.sin_family = AF_INET;
			hp = gethostbyname("127.0.0.1");
			if(!hp)
				printf("Unknown host");

			bcopy((char *)hp->h_addr, (char *)&server.sin_addr, hp->h_length);
			server.sin_port = htons(8877);
			length = sizeof(struct sockaddr_in);
			int ret  = sendto(sock2,request->buf, strlen(request->buf),0,(const struct sockaddr *)&server,length);
					if(ret < 0)
						printf("Sendto");

			strcpy(request->buf, "");

			char recv[PACK_SIZE];
			strcpy(recv, "");
			n = recvfrom(sock2, recv, PACK_SIZE, 0, (struct sockaddr *)&from, &length);
					if(n < 0)
						error("recvfrom");

			printf("\nTUANTRACE: [%s][]\n", recv );

		printf("REQUEST COMPLETED : %d\n", 0);

		sleep(5);
		printf("EXIT THREAD");
	pthread_exit(NULL);
}
