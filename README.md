## The file content<br>
└── HW1_107062650 <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;├── main.c <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;├── lib.c <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;├── lib.h <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;├── makefile <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;└── README.md <br>

## Makefile usage
* ### make
	> produce the binary "hw1"
* ### make clean
	> remove the binary "hw1"

## The features I have completed in my own netstat
* [10%] List TCP and UDP connetions (IPv4).
* [10%] List TCP and UDP connetions (IPv6).
* [30%] Show corresponding program names and arguments for each identified connection.
> The program names are extracted from /proc/[PID]/cmdline.<br>
> It only shows the first arguments for each program, because original netstat does so,too.<br>
> If the connections are created by other users, program names will be displayed as "-", because it has no permission to examine other users' connection and original netstat do so,too.<br>
* [10%] Implement -u and --udp option using getopt_long(3).
* [10%] Implement -t and --tcp option using getopt_long(3).
> It use getopt_long(3) to parse both the short and long command-line arguments.<br>
> It supports to use two types of flag in the same time, e.g.,"-tu","-ut","-t -u","--tcp --udp".<br>
* [10%] Translate network address into user-friendly formats, e.g., from 0100007F to 127.0.0.1/
> It use inet_ntop(3) to accomplish the feature.<br>
> It support both IPv4 and IPv6.<br>
> The functionality has been tested by "017AA8C0 <===> 192.168.122.1" and<br>
												 > > >"BACD0120000000000000000052965732 <===> 2001:cdba::3257:9652"
* [10%] Implement basic filter string feature.
> It filter the program name of the connection.<br>
> It can only filter "one" string.<br>
* [10%] Use Makefile to manage the building process of your program.
> It compiles three .h/.c files and creates "hw1" binary file automatically.<br>

## Usage example 
```console
$ ./hw1
List of TCP connections:
Proto	Local Address				Foreign Address				PID/Program name and arguments
tcp		127.0.0.1:5939              0.0.0.0:0                   -                                       
tcp		127.0.1.1:53                0.0.0.0:0                   -                                       
tcp		127.0.0.1:631               0.0.0.0:0                   -                                       
tcp		192.168.2.42:52000          172.217.160.110:443         2381/chrome --type=                     
tcp		192.168.2.42:50362          176.9.127.15:443            -                                       
tcp		192.168.2.42:52696          64.233.188.188:5228         2381/chrome --type=                     
tcp		192.168.2.42:52838          13.35.3.158:443             2381/chrome --type=                     
tcp		192.168.2.42:34042          119.161.14.18:443           2381/chrome --type=                     
tcp		192.168.2.42:60658          198.252.206.25:443          2381/chrome --type=                     
tcp		192.168.2.42:48120          172.217.24.10:443           2381/chrome --type=                     
tcp		192.168.2.42:49460          216.58.200.35:443           2381/chrome --type=                     
tcp		192.168.2.42:52476          172.217.160.110:443         2381/chrome --type=                     
tcp		192.168.2.42:53778          13.115.86.198:443           2381/chrome --type=                     
tcp		192.168.2.42:39864          119.161.16.12:443           2381/chrome --type=                     
tcp		192.168.2.42:40402          3.217.150.63:443            2381/chrome --type=                     
tcp		192.168.2.42:40404          3.217.150.63:443            2381/chrome --type=                     
tcp		192.168.2.42:44150          172.217.27.142:443          2381/chrome --type=                     
tcp		192.168.2.42:49502          172.217.24.3:443            2381/chrome --type=                     
tcp		192.168.2.42:39858          119.161.16.12:443           2381/chrome --type=                     
tcp6	::1:631                     :::0                        -                                       

List of UDP connections:
Proto	Local Address				Foreign Address				PID/Program name and arguments
udp		224.0.0.251:5353            0.0.0.0:0                   2335/chrome                             
udp		0.0.0.0:5353                0.0.0.0:0                   -                                       
udp		0.0.0.0:47080               0.0.0.0:0                   -                                       
udp		0.0.0.0:40046               0.0.0.0:0                   -                                       
udp		127.0.1.1:53                0.0.0.0:0                   -                                       
udp		0.0.0.0:68                  0.0.0.0:0                   -                                       
udp		0.0.0.0:631                 0.0.0.0:0                   -                                       
udp6	:::44348                    :::0                        -                                       
udp6	:::5353                     :::0                        -   
```

```console
$ ./hw1 --tcp chrome
List of TCP connections:
Proto	Local Address				Foreign Address				PID/Program name and arguments
tcp		192.168.2.42:52000          172.217.160.110:443         2381/chrome --type=                                                         
tcp		192.168.2.42:52696          64.233.188.188:5228         2381/chrome --type=                     
tcp		192.168.2.42:52838          13.35.3.158:443             2381/chrome --type=                     
tcp		192.168.2.42:34042          119.161.14.18:443           2381/chrome --type=                     
tcp		192.168.2.42:60658          198.252.206.25:443          2381/chrome --type=                     
tcp		192.168.2.42:48120          172.217.24.10:443           2381/chrome --type=                     
tcp		192.168.2.42:49460          216.58.200.35:443           2381/chrome --type=                     
tcp		192.168.2.42:52476          172.217.160.110:443         2381/chrome --type=                     
tcp		192.168.2.42:53778          13.115.86.198:443           2381/chrome --type=                     
tcp		192.168.2.42:39864          119.161.16.12:443           2381/chrome --type=                     
tcp		192.168.2.42:40402          3.217.150.63:443            2381/chrome --type=                     
tcp		192.168.2.42:40404          3.217.150.63:443            2381/chrome --type=                     
tcp		192.168.2.42:44150          172.217.27.142:443          2381/chrome --type=                     
tcp		192.168.2.42:49502          172.217.24.3:443            2381/chrome --type=                     
tcp		192.168.2.42:39858          119.161.16.12:443           2381/chrome --type=             
```
