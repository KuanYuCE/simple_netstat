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
> It compiles three .h/.c files and creates "hw1" binary file automatically.
