module server;

import util;

class NetworkAudioStream : SoundStream
{
public:
	static this()
	{
		s_sync = new Object();
	}

	// Default constructor
	this()
	{
		myListener = new SocketTCP();
		myClient = new SocketTCP();
		// Set the sound parameters
		super(1, 44100);
	}

	// Destructor
	~this()
	{
		// Close the sockets
		delete myClient;
		delete myListener;
	}

	// Run the server, stream audio data from the client
	void start(int Port)
	{
		if (!myHasFinished)
		{
			// Listen to the given port for incoming connections
			if (!myListener.listen(Port))
				return;
			Cout("Listening").newline;
			myListener.accept(myClient);
			Cout("New Client").newline;
			// Start playback
			play();

			// Start receiving audio data
			receiveLoop();
		}
		else
		{
			// Start playback
			play();
		}
	}

protected:

	override bool onStart()
	{
		// Reset the playing offset
		myOffset = 0;

		return true;
	}

	override bool onGetData(out short[] data)
	{
		// We have reached the end of the buffer and all audio data have been played : we can stop playback
		if ((myOffset == mySamples.length) && myHasFinished)
			return false;
		// No new data has arrived since last update : wait until we get some
		while (myOffset == mySamples.length && !myHasFinished)
			sleep(0.01f);
				
		synchronized(s_sync)
		{
			myTempBuffer = mySamples[myOffset..mySamples.length];
			// Update the playing offset
			myOffset += myTempBuffer.length;
		}
	
		data = myTempBuffer;
		return true;
	}

private:

	void receiveLoop()
	{
		while (!myHasFinished)
		{
			// Get waiting audio data from the network
			Packet PacketIn = new Packet();
			if (myClient.receive(PacketIn) != SocketStatus.DONE)
				break;

			// Extract the message ID
			ubyte Id;
			PacketIn.get(Id);

			if (Id == AudioData)
			{
				// Extract audio samples from the packet, and append it to our samples buffer
				
				synchronized(s_sync)
				{
					byte* temp = PacketIn.getData().ptr;
					temp++;

					mySamples ~= (cast(short*)temp)[0..(PacketIn.getDataSize - byte.sizeof ) / short.sizeof];
				}
			}
			else if (Id == EndOfStream)
			{
				// End of stream reached : we stop receiving audio data
				myHasFinished = true;
			}
			else
			{
				// Something's wrong...
				myHasFinished = true;
			}
		}
	}

	SocketTCP		myListener;
	SocketTCP		myClient;
	short[]		 mySamples;
	short[]		 myTempBuffer;
	size_t		  myOffset;
	bool			myHasFinished;
	
	static Object	s_sync; 
};



// Launch a server and wait for incoming audio data from
// a connected client
void runServer(int Port)
{
	// Build an audio stream to play sound data as it is received through the network
	NetworkAudioStream audioStream = new NetworkAudioStream;
	audioStream.start(Port);

	// Loop until the sound playback is finished
	while (audioStream.getStatus() != SoundStatus.STOPPED)
	{
		// Leave some CPU time for other threads
		sleep(0.1f);
	}
	
	Cout("Enter to replay").newline;
	Cin.get();
	// Replay the sound (just to make sure replaying the received data is OK)
	audioStream.play();

	// Loop until the sound playback is finished
	while (audioStream.getStatus() != SoundStatus.STOPPED)
	{
		// Leave some CPU time for other threads
		sleep(0.1f);
	}
}
