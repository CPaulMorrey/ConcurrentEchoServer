/******************************************************************************
	concurrentEchoServer.h
******************************************************************************/

#ifndef _CONCURRENTECHOSERVER_H
#define _CONCURRENTECHOSERVER_H

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
	The range of values for valid ports is the range of ports that IANA
	leaves for private use (they will never be assigned to an application).
	The range is: ( 2^14 + 2^15 = 49152 , 2^16 - 1 = 65535 )
*/
#define MIN_VALID_PORT 49152
#define MAX_VALID_PORT 65535
#define MAX_MESSAGE_LENGTH 2048
#define QUEUE_SIZE 2
#define DEFAULT_PORT MIN_VALID_PORT

void programHeaderDisplay( unsigned int port );
void errorCheck( const int value, const char* message );

#endif
