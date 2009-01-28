module soundstream;

import dsfml.system.all;
import dsfml.audio.all;

version (Tango)
{
    import tango.io.Console;
    import tango.io.Stdout;
}
else
{
    import std.stdio;
}

// SoundStream is an abstract class.
// You need to implement onStart() and onGetData()
// Don't forget to call initialize() before any usage or playback will fail.
class MySoundStream : SoundStream
{
    SoundBuffer m_buff;
    short[] m_data;
    size_t m_cursor;
    
    this()
    {
        // We initialize the stream with some sound informations
        super(1, 11025);
        
        // We create a sound buffer to load samples from files
        m_buff = new SoundBuffer("Data/car_idle.wav");
        m_data = m_buff.getSamples[0..m_buff.getSamplesCount];
    }
protected:
    bool onStart()
    {
        // No specifics things to do, just return true.
        return true;
    }
    
    bool onGetData(out short[] data)
    {
        // We ensure that we have enough data to send
        if (m_cursor + this.getSampleRate > m_data.length)
            return false;
        
        // Assign data in the buffer ...
        data = m_data[m_cursor..m_cursor + this.getSampleRate];
        // ... and increment the cursor
        m_cursor += this.getSampleRate;
        return true;
    }
    
}

void main()
{
    MySoundStream stream = new MySoundStream();
    
    display("Playing sound !\n Press enter to stop playback.");
    stream.play();
    read();
    stream.stop();
}

void display(char[] c)
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
*   Dummy function to prevent console closing on windows
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
