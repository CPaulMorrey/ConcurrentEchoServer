/******************************************************************************
	concurrentEchoServer.c

	version 1.0

	C. Paul Morrey     paul@morrey.com

	This is a simple example of an echo server that uses concurrent
	processes to handle multiple connections.
******************************************************************************/

#include "concurrentEchoServer.h"

int main(int argc, char *argv[]) {
	pid_t processID = -1;
	int listeningSocket = -1;
	int connectionSocket = -1;
	int portNumber = -1;
	struct sockaddr_in serverAddress;
	struct sockaddr_in clientAddress;
	const unsigned int defaultPortNumber = DEFAULT_PORT;
	const int listeningQueueSize = QUEUE_SIZE; /* A concurrent server shouldn't need a large queue */
	int bindValue = -1;
	int listenValue = -1;
	char message[MAX_MESSAGE_LENGTH] = "";
	const char* confirmationMessage = "Your message was recieved by the concurrentEchoServer program.\n";
	int messageLength = -1;

	/* Determine the port number to use */
	if( argc == 2 ) {
		portNumber = atoi( argv[1] );
		if(( portNumber <= MIN_VALID_PORT ) || ( portNumber >= MAX_VALID_PORT )) {
			printf( "ERROR -- Illegal port number attempted. Using default port number instead.\n");
			printf( "Valid ports are between %i and %i.\n", MIN_VALID_PORT, MAX_VALID_PORT );
			portNumber = defaultPortNumber;
		}
	} else {
		portNumber = defaultPortNumber;
	}

	programHeaderDisplay( portNumber );

	listeningSocket = socket( AF_INET, SOCK_STREAM, 0 );  /* SOCK_STREAM means that it uses TCP, this enables connections from a telnet client */
	errorCheck( listeningSocket, "Bad listening socket." );

	serverAddress.sin_port = htons(portNumber);
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);

	bindValue = bind( listeningSocket, (struct sockaddr *) &serverAddress, sizeof(serverAddress) );
	errorCheck( bindValue, "Bad bind value." );

	listenValue = listen( listeningSocket, listeningQueueSize );
	errorCheck( listenValue, "Bad listening value." );

	while( 1 ) { /* Run forever */
		socklen_t clientAddressLength = sizeof( clientAddress );
		connectionSocket = accept(listeningSocket, (struct sockaddr *) &clientAddress, &clientAddressLength );
		errorCheck( connectionSocket, "Bad connection socket." );

		if( (processID = fork()) == 0 ) {
			/* The following lines are executed ONLY by a child process. */
			close( listeningSocket );

			if( ( messageLength = recv( connectionSocket, message, MAX_MESSAGE_LENGTH-1, 0)) > 0 ) {
				message[messageLength] = 0;
				printf( "MESSAGE RECEIVED: %s", message );
				send( connectionSocket, confirmationMessage, strlen( confirmationMessage ), 0 );
			}

			close( connectionSocket );
			exit( 0 );  /* This ends the child process */
		}

		close( connectionSocket );
	}

	return 0;
}

/*****************************************************************************/
void programHeaderDisplay( const unsigned int port ) {
	printf( "Concurrent Echo Server -- version 1\nby C. Paul Morrey (paul@morrey.com)\n" );
	printf( "Running on port %i\n", port );

	return;
}

/*****************************************************************************/
void errorCheck( const int value, const char* message ) {
	if( value < 0 ) {
		printf( "ERROR -- %s\nProrgam terminated.\n", message );
		exit( -1 );
	}
}

/*****************************************************************************/

