module socketserver;

import dsfml.system.all;
import dsfml.network.all;

version (Tango)
{
	import tango.io.Console;
	import tango.io.Stdout;
}
else
{
	import std.stdio;
}

void main()
{
	//We create a TCP socket for listening incomming client
	SocketTCP listener = new SocketTCP();
	
	//Set a random port for the listener
	if (!listener.listen(9000))
		assert(0);
	
	//Creation of TCP socket 
	SocketTCP client = new SocketTCP();
	IPAddress ipClient;
	
	display("Waiting for client."w);
	
	if (listener.accept(client, ipClient) == SocketStatus.DONE) //This call blocks until client connection
	{
		display("New client connected."w);
		//The packet for retrieving the client message
		Packet p = new Packet();
		display("Waiting for data"w);
		if (client.receive(p) != SocketStatus.DONE) //Assert on reception error
			assert(0);
		

		//Display the string send by the client
		wchar[] c;
		p.get(c);
		display("Packet received : "w ~ c);
		
		//Clear the packet (We could use a new one)
		p.clear();
		
		//and send response to client
		client.send(p.set("Hello from the server !"w));
	}
	read();
}

/**
*	Multilib string display
*/
void display(wchar[] c)
{
	version (Tango)
	{ 
		Stdout(c).newline;
	}
	else
	{
		writefln("%s", c);
	}
}

/**
*	Dummy function to prevent console closing on windows
*/
void read()
{
	version (Tango)
	{
		Cin.get();
	}
	else
	{
		readln();
	}
}
