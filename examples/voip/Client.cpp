////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Audio.hpp>

#include <SFML/Network.hpp>

#include <iostream>


const std::uint8_t clientAudioData   = 1;
const std::uint8_t clientEndOfStream = 2;


////////////////////////////////////////////////////////////
/// Specialization of audio recorder for sending recorded audio
/// data through the network
////////////////////////////////////////////////////////////
class NetworkRecorder : public sf::SoundRecorder
{
public:
    ////////////////////////////////////////////////////////////
    /// Constructor
    ///
    /// \param host Remote host to which send the recording data
    /// \param port Port of the remote host
    ///
    ////////////////////////////////////////////////////////////
    NetworkRecorder(const sf::IpAddress& host, unsigned short port) : m_host(host), m_port(port)
    {
    }

    ////////////////////////////////////////////////////////////
    /// Destructor
    ///
    /// \see SoundRecorder::~SoundRecorder()
    ///
    ////////////////////////////////////////////////////////////
    ~NetworkRecorder() override
    {
        // Make sure to stop the recording thread
        stop();
    }

private:
    ////////////////////////////////////////////////////////////
    /// \see SoundRecorder::onStart
    ///
    ////////////////////////////////////////////////////////////
    bool onStart() override
    {
        if (m_socket.connect(m_host, m_port) == sf::Socket::Status::Done)
        {
            std::cout << "Connected to server " << m_host << std::endl;
            return true;
        }
        else
        {
            return false;
        }
    }

    ////////////////////////////////////////////////////////////
    /// \see SoundRecorder::onProcessSamples
    ///
    ////////////////////////////////////////////////////////////
    bool onProcessSamples(const std::int16_t* samples, std::size_t sampleCount) override
    {
        // Pack the audio samples into a network packet
        sf::Packet packet;
        packet << clientAudioData;
        packet.append(samples, sampleCount * sizeof(std::int16_t));

        // Send the audio packet to the server
        return m_socket.send(packet) == sf::Socket::Status::Done;
    }

    ////////////////////////////////////////////////////////////
    /// \see SoundRecorder::onStop
    ///
    ////////////////////////////////////////////////////////////
    void onStop() override
    {
        // Send a "end-of-stream" packet
        sf::Packet packet;
        packet << clientEndOfStream;

        if (m_socket.send(packet) != sf::Socket::Status::Done)
        {
            std::cerr << "Failed to send end-of-stream packet" << std::endl;
        }

        // Close the socket
        m_socket.disconnect();
    }

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    sf::IpAddress  m_host;   ///< Address of the remote host
    unsigned short m_port;   ///< Remote port
    sf::TcpSocket  m_socket; ///< Socket used to communicate with the server
};


////////////////////////////////////////////////////////////
/// Create a client, connect it to a running server and
/// start sending him audio data
///
////////////////////////////////////////////////////////////
void doClient(unsigned short port)
{
    // Check that the device can capture audio
    if (!sf::SoundRecorder::isAvailable())
    {
        std::cout << "Sorry, audio capture is not supported by your system" << std::endl;
        return;
    }

    // Ask for server address
    std::optional<sf::IpAddress> server;
    do
    {
        std::cout << "Type address or name of the server to connect to: ";
        std::cin >> server;
    } while (!server.has_value());

    // Create an instance of our custom recorder
    NetworkRecorder recorder(server.value(), port);

    // Wait for user input...
    std::cin.ignore(10000, '\n');
    std::cout << "Press enter to start recording audio";
    std::cin.ignore(10000, '\n');

    // Start capturing audio data
    if (!recorder.start(44100))
    {
        std::cerr << "Failed to start recorder" << std::endl;
        return;
    }

    std::cout << "Recording... press enter to stop";
    std::cin.ignore(10000, '\n');
    recorder.stop();
}
