struct connection
{
	char proto[5];
	char local_addr[50];
	char foreign_addr[50];
	char pidprogram[30];
	struct connection *next;
};

void split(char **arr, char *str, const char *del);
int hex2int(char ch);
void hexstr2ip(char *dest, char *src, char *ver);
struct connection* getconnectinfo(char *protocol, char *filter);
void showconnection(struct connection *root);
void setfilter(int argc, char *argv[], int *tcp, int *udp);
int getpname(char *dest, char *src, char *filter);
int startswith(const char *str, const char *pre);
char* getfilterstr(int argc, char *argv[]);
