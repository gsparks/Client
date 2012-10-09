#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include "xml.c"

#define MAXBUFFER	512

static char Server[] = "127.0.0.1";

//To DO:
//XML as input to command line
//Server location as option, default to localhost
//XML validation
//web wrapper with icons and pretty stuff
//Actually format XML response
//recognize Airport code vs lat/lon vs City code (google api? lat/lon)

/*
XML my_xml = XML_tag("wwxtp",
	NULL,  // No attributes
	XML_tag("query",
		NULL,  // No attributes
		XML_tag("command",
		  NULL,
		  "TEST",
		  XML_tag("position",
        "lat", "34.4261944",
			 "long", "-119.8415000",
			 NULL,
			NULL
		  ),
		NULL
		),
	NULL
	),
	NULL // Done with children
);
*/

/*
XML my_xml = XML_tag("wwxtp",
	NULL,
	XML_tag("request",
		NULL,  
		XML_tag("command",
		TEST,
		),
		XML_tag("position",
		  "lat", "34.4261944",
		  "long", "-119.8415000",
		  NULL
		),
		NULL
	),
	NULL 
);
*/

//static char Request[] = XML_as_text(my_xml);
static char Request[] = "<wwxtp><query><command>TEST</command><id>IZA</id></query></wwxtp>";

int main(int argc, char ** argv)
{
	int fd;
	int ret;
	struct sockaddr_in sa;
	char buffer[MAXBUFFER];

	printf("creating socket...\n");
	/* Create socket */
	fd = socket(AF_INET, SOCK_STREAM, 0);
	
	if(fd == -1) {
		printf("socket() failed with errno = %d", errno);
		return errno;
	}
	
	/* Setup sockaddr */
	inet_pton(AF_INET, Server, &(sa.sin_addr));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(4114); /* host to network endian swap */

	/* Connect */
	ret = connect(fd, (struct sockaddr *) &sa, sizeof(sa));

	if(ret == -1) {
		printf("connect() failed with errno = %d", errno);
		return errno;
	}

	/* Send request */
	printf("sending request...\n");
	ret = send(fd, Request, strlen(Request), 0);
	
	if(ret == -1) {
		printf("send() failed with errno = %d", errno);
		return errno;
	}
	
	/* Recieve response */
	do
	{
		ret = recv(fd, &buffer, MAXBUFFER - 1, 0);

		if(ret > 0)
		{
			buffer[ret] = '\0'; /* null terminate the string */
			printf("%s",buffer);
			//printf("%s",XML_get_addr(XML_get_child(XML_get_child(XML_parse(buffer)))), "lat");
			//printf("%s",XML_get_addr(XML_get_child(XML_get_child(XML_parse(buffer)))), "long");
		}
	} while(ret > 0);

	if(ret == -1) {
		printf("recv() failed with errno = %d", errno);
		return errno;
	}

	/* Close */
	ret = close(fd);
	if(ret == -1) {
		printf("close() failed with errno = %d", errno);
		return errno;
	}

}	

