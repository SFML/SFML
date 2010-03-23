
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
    /// \param host Remote host to which send the recording data
    /// \param port Port of the remote host
    ///
    ////////////////////////////////////////////////////////////
    NetworkRecorder(const sf::IpAddress& host, unsigned short port) :
    myHost(host),
    myPort(port)
    {
    }

private :

    ////////////////////////////////////////////////////////////
    /// /see SoundRecorder::OnStart
    ///
    ////////////////////////////////////////////////////////////
    virtual bool OnStart()
    {
        if (mySocket.Connect(myHost, myPort) == sf::Socket::Done)
        {
            std::cout << "Connected to server " << myHost << std::endl;
            return true;
        }
        else
        {
            return false;
        }
    }

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
    /// /see SoundRecorder::OnStop
    ///
    ////////////////////////////////////////////////////////////
    virtual void OnStop()
    {
        // Send a "end-of-stream" packet
        sf::Packet packet;
        packet << endOfStream;
        mySocket.Send(packet);

        // Close the socket
        mySocket.Disconnect();
    }

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    sf::IpAddress  myHost;   ///< Address of the remote host
    unsigned short myPort;   ///< Remote port
    sf::TcpSocket  mySocket; ///< Socket used to communicate with the server
};


////////////////////////////////////////////////////////////
/// Create a client, connect it to a running server and
/// start sending him audio data
///
////////////////////////////////////////////////////////////
void DoClient(unsigned short port)
{
    // Check that the device can capture audio
    if (sf::SoundRecorder::IsAvailable() == false)
    {
        std::cout << "Sorry, audio capture is not supported by your system" << std::endl;
        return;
    }

    // Ask for server address
    sf::IpAddress server;
    do
    {
        std::cout << "Type address or name of the server to connect to : ";
        std::cin  >> server;
    }
    while (server == sf::IpAddress::None);

    // Create an instance of our custom recorder
    NetworkRecorder recorder(server, port);

    // Wait for user input...
    std::cin.ignore(10000, '\n');
    std::cout << "Press enter to start recording audio";
    std::cin.ignore(10000, '\n');

    // Start capturing audio data
    recorder.Start(44100);
    std::cout << "Recording... press enter to stop";
    std::cin.ignore(10000, '\n');
    recorder.Stop();
}
