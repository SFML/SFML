using System;
using System.Threading;
using SFML;
using SFML.Audio;

namespace sample_soundcapture
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        static void Main(string[] args)
        {
            // Check that the device can capture audio
            if (SoundRecorder.CanCapture == false)
            {
                Console.WriteLine("Sorry, audio capture is not supported by your system");
                return;
            }

            // Choose the sample rate
            Console.WriteLine("Please choose the sample rate for sound capture (44100 is CD quality) : ");
            uint SampleRate = uint.Parse(Console.ReadLine());

            // Wait for user input...
            Console.WriteLine("Press enter to start recording audio");
            Console.ReadLine();

            // Here we'll use an integrated custom recorder, which saves the captured data into a SoundBuffer
            SoundBufferRecorder Recorder = new SoundBufferRecorder();

            // Audio capture is done in a separate thread, so we can block the main thread while it is capturing
            Recorder.Start(SampleRate);
            Console.WriteLine("Recording... press enter to stop");
            Console.ReadLine();
            Recorder.Stop();

            // Get the buffer containing the captured data
            SoundBuffer Buffer = Recorder.SoundBuffer;

            // Display captured sound informations
            Console.WriteLine("Sound information :");
            Console.WriteLine(" " + Buffer.Duration      + " seconds");
            Console.WriteLine(" " + Buffer.SampleRate    + " samples / seconds");
            Console.WriteLine(" " + Buffer.ChannelsCount + " channels");

            // Choose what to do with the recorded sound data
            Console.WriteLine("What do you want to do with captured sound (p = play, s = save) ? ");
            char Choice = char.Parse(Console.ReadLine());

            if (Choice == 's')
            {
                // Choose the filename
                Console.WriteLine("Choose the file to create : ");
                string Filename = Console.ReadLine();

                // Save the buffer
                Buffer.SaveToFile(Filename);
            }
            else
            {
                // Create a sound instance and play it
                Sound Sound = new Sound(Buffer);
                Sound.Play();

                // Wait until finished
                while (Sound.Status == SoundStatus.Playing)
                {
                    // Display the playing position
                    Console.CursorLeft = 0;
                    Console.Write("Playing... " + Sound.PlayingOffset + " sec     ");

                    // Leave some CPU time for other threads
                    Thread.Sleep(100);
                }
            }

            // Finished !
            Console.WriteLine("\nDone !");
        }
    }
}
