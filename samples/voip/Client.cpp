
////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <iostream>


const sf::Uint8 audioData   = 1;
const sf::Uint8 endOfStream = 2;


////////////////////////////////////////////////////////////
/// Specialization of audio recorder for sending recorded audio
/// data through the network
////////////////////////////////////////////////////////////
class NetworkRecorder : public sf::SoundRecorder
{
public :

    ////////////////////////////////////////////////////////////
    /// Constructor
    ///
    /// \param Socket : Socket that holds the connection with the server
    ///
    ////////////////////////////////////////////////////////////
    NetworkRecorder(sf::SocketTCP socket) :
    mySocket(socket)
    {

    }

private :

    ////////////////////////////////////////////////////////////
    /// /see SoundRecorder::ProcessSamples
    ///
    ////////////////////////////////////////////////////////////
    virtual bool OnProcessSamples(const sf::Int16* samples, std::size_t samplesCount)
    {
        // Pack the audio samples into a network packet
        sf::Packet packet;
        packet << audioData;
        packet.Append(samples, samplesCount * sizeof(sf::Int16));

        // Send the audio packet to the server
        return mySocket.Send(packet) == sf::Socket::Done;
    }

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    sf::SocketTCP mySocket; ///< Socket used to communicate with the server
};


////////////////////////////////////////////////////////////
/// Create a client, connect it to a running server and
/// start sending him audio data
///
////////////////////////////////////////////////////////////
void DoClient(unsigned short port)
{
    // Check that the device can capture audio
    if (sf::SoundRecorder::CanCapture() == false)
    {
        std::cout << "Sorry, audio capture is not supported by your system" << std::endl;
        return;
    }

    // Ask for server address
    sf::IPAddress serverAddress;
    do
    {
        std::cout << "Type address or name of the server to connect to : ";
        std::cin  >> serverAddress;
    }
    while (!serverAddress.IsValid());

    // Create a TCP socket for communicating with server
    sf::SocketTCP socket;

    // Connect to the specified server
    if (socket.Connect(port, serverAddress) != sf::Socket::Done)
        return;
    std::cout << "Connected to server " << serverAddress << std::endl;

    // Wait for user input...
    std::cin.ignore(10000, '\n');
    std::cout << "Press enter to start recording audio";
    std::cin.ignore(10000, '\n');

    // Create a instance of our custom recorder
    NetworkRecorder recorder(socket);

    // Start capturing audio data
    recorder.Start(44100);
    std::cout << "Recording... press enter to stop";
    std::cin.ignore(10000, '\n');
    recorder.Stop();

    // Send a "end-of-stream" packet
    sf::Packet packet;
    packet << endOfStream;
    socket.Send(packet);

    // Close the socket when we're done
    socket.Close();
}
