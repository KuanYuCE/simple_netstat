#include <stdio.h>
#include <string.h>
#include "lib.h"


int tcp_filter = 0;
int udp_filter = 0;
char *filter_str;

struct connection *rootTCP = 0;  // the head of the link list of TCP connection
struct connection *rootUDP = 0;  // the head of the link list of UDP connection


int main(int argc, char *argv[])
{
	filter_str = getfilterstr(argc, argv);

	setfilter(argc, argv, &tcp_filter, &udp_filter);
	if (!tcp_filter)
	{
		rootTCP = getconnectinfo("tcp", filter_str);
		printf("List of TCP connections:\n");
		showconnection(rootTCP);
	}
	if (!udp_filter)
	{
		rootUDP = getconnectinfo("udp", filter_str);
		printf("\nList of UDP connections:\n");
		showconnection(rootUDP);
	}

	return 0;
}
