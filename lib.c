#include <getopt.h>
#include <sys/stat.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <libgen.h>
#include "lib.h"


void split(char **arr, char *str, const char *del)
{
	char *s = strtok(str, del);
   
	while(s != NULL) {
		*arr++ = s;
		s = strtok(NULL, del);
	}
}


/*
usage:
	if the str starts with pre, return true. Otherwise, return false.
*/
int startswith(const char *str, const char *pre)
{
    return strncmp(pre, str, strlen(pre)) == 0;
}


/*
usage: 
	return the address of filter string from argv array.
	return 0 if no filter string in the argv array.
*/
char* getfilterstr(int argc, char *argv[])
{
	char *dest;
	for (int i=1;i<argc;i++)
	{
		if (!startswith(argv[i], "-"))
		{
			dest = malloc(sizeof(strlen(argv[i])));		
			strcpy(dest, argv[i]);
			return dest;
		}
	}
	return 0;
}


/*
usage: it transfer the hexadecimal char to int
*/
int hex2int(char ch)
{
    if (ch >= '0' && ch <= '9')
        return ch - '0';
    if (ch >= 'A' && ch <= 'F')
        return ch - 'A' + 10;
    if (ch >= 'a' && ch <= 'f')
        return ch - 'a' + 10;
    return -1;
}


/*
Parameters:
	dest: Pointer to the destination array, which should contain IP Address.
	src: Pointer to the source array, which should contain the string of hexadecimal.
	ver: choose ipv4 or ipv6
Usage:
	For example, convert "2A02A8C0:C2DA" to "192.168.2.42:52044".
*/
void hexstr2ip(char *dest, char *src, char *ver)
{
	if (strcmp(ver, "ipv4")==0)
	{
		struct sockaddr_in sa;
		struct in_addr ip;
		char str[INET_ADDRSTRLEN];
		char *token_arr[2];
		int port;
		char portStr[5];
		char str_ip_port[INET_ADDRSTRLEN+6] = "";
		split(token_arr, src, ":");				// token[1]'s format is [ip:port]
		port = (int)strtol(token_arr[1], NULL, 16);		// convert hex string of port to integer
		sprintf(portStr, "%d", port);					// convert integer of port to decimal string
		ip.s_addr = (int)strtol(token_arr[0], NULL, 16);// convert hex string of IP to integer
		sa.sin_addr = ip;
		inet_ntop(AF_INET, &(sa.sin_addr), str, INET_ADDRSTRLEN);	// convert integer of ip to IP address into "str"
		strcat(str_ip_port, str);
		strcat(str_ip_port, ":");
		strcat(str_ip_port, portStr);
		strcpy(dest, str_ip_port);
	}
	else if (strcmp(ver, "ipv6")==0)
	{
		struct sockaddr_in6 sa;
		struct in6_addr ip;
		char str[INET6_ADDRSTRLEN];
		char *token_arr[2];
		int port;
		char portStr[5];
		char str_ip_port[INET6_ADDRSTRLEN+6] = "";
		unsigned char ip_char[16];

		split(token_arr, src, ":");						// token[1]'s format is [ip:port]
		port = (int)strtol(token_arr[1], NULL, 16);		// convert hex string of port to integer
		sprintf(portStr, "%d", port);					// convert integer of port to decimal string
		
		//token_arr[0] = "20010DB8000000000123456789ABCDEF";
		for(int i=0;i<=15;i++)
		{
			// turn 2 digits (4 bytes per digit) of hex string to 1 unsigned char(8 bytes per unsigned char)
			ip_char[i] = (hex2int(token_arr[0][2*i])*16)+hex2int(token_arr[0][2*i+1]);
		}

		// fix the problem: 
		// 2001:0DB8:		:: 0123:4567:89AB:CDEF  could come out as:
		// B80D:0120:0000:0000:6745:2301:EFCD:AB89
		for(int i=0;i<=15;i++)
		{
			unsigned char temp;
			if (i%4 != 0)
				continue;
			else if (i%4 == 0)
			{
				temp = ip_char[i];
				ip_char[i] = ip_char[i+3];
				ip_char[i+3] = temp;
				
				temp = ip_char[i+1];
				ip_char[i+1] = ip_char[i+2];
				ip_char[i+2] = temp;
			}
		}
		
		memcpy(ip.s6_addr, ip_char, sizeof(unsigned char)*16);			// ** can not use strcpy on unsigned char, it will go wrong. **
		sa.sin6_addr = ip;
		inet_ntop(AF_INET6, &(sa.sin6_addr), str, INET6_ADDRSTRLEN);	// convert integer of ip to IP address into "str"
		strcat(str_ip_port, str);
		strcat(str_ip_port, ":");
		strcat(str_ip_port, portStr);
		strcpy(dest, str_ip_port);
	}
}



/*
parameters:
	protocol: choose "tcp" or "udp"
	filter: show only the information which program name is filter name. if filter address is 0, it means user didn't use filter stirng.
usage:
	it will return the head of the connection's link list, the type is struct connection.
*/
struct connection* getconnectinfo(char *protocol, char *filter)
{
	struct connection *root = 0;
	struct connection *current = 0;
	for(int iter=0;iter<=1;iter++)	// first iteration for ipv4, second iteration for ipv6
	{
		char *proto;
		char *path;
		
		if (strcmp(protocol, "tcp") == 0)
		{
			if (iter==0)
			{
				path = "/proc/net/tcp";
				proto = "tcp";
			}
			else if (iter==1)
			{
				path = "/proc/net/tcp6";
				proto = "tcp6";
			}
		}
		else if (strcmp(protocol, "udp") == 0)
		{
			if (iter==0)
			{
				path = "/proc/net/udp";
				proto = "udp";
			}
			else if (iter==1)
			{
				path = "/proc/net/udp6"; 
				proto = "udp6";
			}
		}

		//======== read the /proc/net/xxx file ========
		char buf[20000];
		memset(buf, 0, 20000);
		size_t result;
		FILE * pFile;
		pFile = fopen(path, "r");
		if (pFile != NULL)
		{
			result = fread(buf, 1, 20000, pFile);
		}
		fclose(pFile);

		//======== split the file context to array ========
		char *arr[1000];
		memset(arr, 0, 1000);
		const char *del = "\n";
		int uid;
		uid = getuid();
		split(arr, buf, del);

		//======== store connection data into link list of strcut ============
		
		for(int i=1;arr[i]!=NULL;i++)	// iterate each row of connection data
		{
			char *token[20];
			const char *del = " ";
			split(token, arr[i], del);  // the data per row are split by space and stroe in "token"
			struct connection *con = malloc(sizeof(struct connection));
			con->next = NULL;

			strcpy(con->proto, proto);	// store protocol name to structure
			
			// ===== convert hex string to IP address and store IP address into structure ======
			if (strcmp(proto, "tcp")==0 || strcmp(proto, "udp")==0)
			{
				hexstr2ip(con->local_addr, token[1], "ipv4");
				hexstr2ip(con->foreign_addr, token[2], "ipv4");
			}
			else if (strcmp(proto, "tcp6")==0 || strcmp(proto, "udp6")==0)
			{
				hexstr2ip(con->local_addr, token[1], "ipv6");
				hexstr2ip(con->foreign_addr, token[2], "ipv6");
			}

			
			// ===== store the PID, program name, parameter of the connection in struct connection ==================

			if (getpname(con->pidprogram, token[9], filter)==-1)	// token[9] is inode number, if getpname return -1, it means
			{														// this connection's program name is not filter name.
				free(con);
				continue;
			}

			//======= set the pointer of link list =====
			if (current != NULL)
				current->next = con;
			else if (current == NULL)  //the first iteration of this loop
				root = con;
			current = con;
		
		}
	}
	return root;
}


/*
parameter:
	root: the root of the connection's link list.
Usage:
	print the connection row by row to standard out.
*/
void showconnection(struct connection *root)
{
	printf("Proto\tLocal Address\t\t\t\tForeign Address\t\t\t\tPID/Program name and arguments\n");
	struct connection *current = 0;
	current = root;
	while(current != NULL)
	{
		printf("%s\t%-40s%-40s%-40s\n", current->proto, current->local_addr, current->foreign_addr, current->pidprogram);
		current = current->next;
	}
}


/*
parameter:
	argc: the length of argv
	argv: the parameters of the main program
	tcp: the address of the tcp flag
	udp: the address of the udp flag
usage:
	it will set the tcp and udp flag to 1 or 0 according to the parameter "argv".
*/
void setfilter(int argc, char *argv[], int *tcp, int *udp)
{
	char opt;
	const char *optstring = "tu";
	struct option opts[] = 
	{
		{"tcp", 0, NULL, 't'},
		{"udp", 0, NULL, 'u'},
		{ NULL, 0, NULL, 0 }
	};
	while ((opt = getopt_long(argc, argv, optstring, opts, NULL)) != -1)
	{
		switch(opt)
		{
			case 't':
				*udp = 1;
				break;
			case 'u':
				*tcp = 1;
				break;
			default:
				break;
		}
	}
	if (*tcp == 1 && *udp == 1)
	{
		*tcp = 0;
		*udp = 0;
	}
		
}


/*
parameters:
	dest: the pointer you want to store the result in
	src: the inode number of the connection which you want to find its pid and pname
	filter: show only the information which program name is filter name. if filter address is 0, it means user didn't use filter stirng.
usage:
	you will get the result which format is [PID/Program parameter], if return value is 0.
return value:
 	0 means get pname successfully or no filter string, -1 means the pname is not the filter string
*/
int getpname(char *dest, char *src, char *filter)
{
	DIR *dir;
	struct dirent *file;
	dir = opendir("/proc/");
	while((file = readdir(dir)) != NULL)				// to find all the process in /proc/
	{
		if (strtol(file->d_name, NULL, 10) != 0)		// identify PID
		{
			char path[20];
			char path_base[20];
			FILE * fcomm;
			memset(path, 0, 20);
			strcat(path, "/proc/");
			strcat(path, file->d_name);
			strcpy(path_base, path);					// path_base: /proc/[pid]
			strcat(path, "/fd/");						// path: /proc/[pid]/fd/
			

			if (access(path, R_OK) != 0)				// no permission to access the dir
				continue;
			else if (access(path, R_OK) == 0)
			{
				DIR *dir2;
				struct dirent *file2;
				dir2 = opendir(path);					// path : /proc/[pid]/fd/
				while((file2 = readdir(dir2))!= NULL)	// to find all the file descriptors of the process
				{
					struct stat st;
					char path2[25];
					memset(path2, 0, 25);
					strcpy(path2, path);
					strcat(path2, file2->d_name);		// path2 : /proc/[pid]/fd/[fd_number].
					stat(path2, &st);
					
					if (st.st_ino == atoi(src))	// the inode of the file descriptor and inode of the connection are same
					{
						FILE *f;
						char buf[100];					// store information of /prop/[pid]/cmdline
						char *token[30];				// store the strings split by " " in /prop/[pid]/cmdline
						char *pname;					// store the basename of the process which created the connection
						memset(buf, 0, 100);
						memset(token, 0, 30);

						strcpy(dest, file->d_name);		// copy pid to dest
						strcat(dest, "/");
						strcat(path_base, "/cmdline");	// path_base: /proc/[pid]
						f = fopen(path_base, "r");
						fread(buf, 1, 100, f);
						fclose(f);
						split(token, buf, " ");
						pname = basename(token[0]);

						// this part check whether the filter name is equals to pname, if not, 
						if(filter!=0)
						{
							if (strcmp(pname, filter)!=0)
								return -1;
						}
						strcat(dest, pname);
						
						// extract the first parameter and copy it to dest
						if (token[1] != NULL)
						{
							strcat(dest, " ");
							for(int i=0; i<strlen(token[1]);i++)
							{
								if (token[1][i] == '=')
									token[1][i+1] = '\0';

							}
							strcat(dest, token[1]);
						}
						closedir(dir2);
						closedir(dir);
						return 0;
					}
				}
				closedir(dir2);
			}
		}
		
	}

	closedir(dir);
	strcpy(dest, "-");	// if the connection are created by the file which the user has no permission to access, then go here
	if(filter!=0)
		return -1;
	return 0;
}
