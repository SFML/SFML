module entry;

import util;
import server;
import client;

int main(char[][] args)
{
	char[][] argc = args.dup;
	if (argc.length > 1)
	{
		if (	argc[1] == "-c" && 
				argc.length == 4)
		{
			IPAddress adr = IPAddress(argc[2]);
			
			if (adr.isValid() && 
				parse(argc[3]) <= 60000 && 
				parse(argc[3]) >= 1000)
			{
				runClient(adr, parse(argc[3]));
			}
			else
				printUsage();
		}
		else if (	argc[1] == "-s" && 
					argc.length == 3 &&
					parse(argc[2]) <= 60000 &&
					parse(argc[2]) >= 1000)
		{
				runServer(parse(argc[2]));

		}
		else
			printUsage();
	}
	else
		printUsage();
		
	return 0;
}

void printUsage()
{
	Cout("Usage :\n voip.exe [-c [ip address] | -s] [port] \n -c = run as client\n -s = run as server\n ip address = address of the server\n port = port between 1000 and 65000\n").newline;
}
