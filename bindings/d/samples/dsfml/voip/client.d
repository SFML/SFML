module client;

import util;


// Specialization of audio recorder for sending recorded audio
// data through the network
class NetworkRecorder : SoundRecorder
{
public:

	// Constructor
	this(SocketTCP Socket)
	{
		mySocket = Socket;
	}

	~this()
	{
		delete mySocket;
	}
protected:
	override bool onStart()
	{
		return true;
	}

	override void onStop()
	{
		
	}
	
	override bool onProcessSamples(short[] samples)
	{
		// Pack the audio samples into a network packet
		Packet PacketOut = new Packet();
		PacketOut.set(AudioData);
		PacketOut.append((cast(byte*)samples.ptr)[0..samples.length * short.sizeof]);
		// Send the audio packet to the server
		return mySocket.send(PacketOut) == SocketStatus.DONE;
	}

	SocketTCP mySocket; ///< Socket used to communicate with the server
}

void runClient(IPAddress adr, int port)
{
	// Create a TCP socket for communicating with server
	SocketTCP Socket = new SocketTCP();

	// Connect to the specified server
	if (!Socket.connect(port, adr))
		return;

	// Wait for user input...
	Cout("Press enter to start recording audio").newline;
	Cin.get();

	// Create a instance of our custom recorder
	NetworkRecorder Recorder = new NetworkRecorder(Socket);

	// Start capturing audio data
	Recorder.start(44100);
	Cout("Press enter to stop recording audio").newline;
	Cin.get();
	Recorder.stop();

	// Send a "end-of-stream" packet
	Packet PacketOut = new Packet();
	PacketOut.set(EndOfStream);
	Socket.send(PacketOut);
}
