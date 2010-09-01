using System;
using System.Threading;
using SFML;
using SFML.Audio;

namespace sound_capture
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        static void Main(string[] args)
        {
            // Check that the device can capture audio
            if (SoundRecorder.IsAvailable == false)
            {
                Console.WriteLine("Sorry, audio capture is not supported by your system");
                return;
            }

            // Choose the sample rate
            Console.WriteLine("Please choose the sample rate for sound capture (44100 is CD quality) : ");
            uint sampleRate = uint.Parse(Console.ReadLine());

            // Wait for user input...
            Console.WriteLine("Press enter to start recording audio");
            Console.ReadLine();

            // Here we'll use an integrated custom recorder, which saves the captured data into a SoundBuffer
            SoundBufferRecorder recorder = new SoundBufferRecorder();

            // Audio capture is done in a separate thread, so we can block the main thread while it is capturing
            recorder.Start(sampleRate);
            Console.WriteLine("Recording... press enter to stop");
            Console.ReadLine();
            recorder.Stop();

            // Get the buffer containing the captured data
            SoundBuffer buffer = recorder.SoundBuffer;

            // Display captured sound informations
            Console.WriteLine("Sound information :");
            Console.WriteLine(" " + buffer.Duration      + " seconds");
            Console.WriteLine(" " + buffer.SampleRate    + " samples / seconds");
            Console.WriteLine(" " + buffer.ChannelsCount + " channels");

            // Choose what to do with the recorded sound data
            Console.WriteLine("What do you want to do with captured sound (p = play, s = save) ? ");
            char choice = char.Parse(Console.ReadLine());

            if (choice == 's')
            {
                // Choose the filename
                Console.WriteLine("Choose the file to create : ");
                string filename = Console.ReadLine();

                // Save the buffer
                buffer.SaveToFile(filename);
            }
            else
            {
                // Create a sound instance and play it
                Sound sound = new Sound(buffer);
                sound.Play();

                // Wait until finished
                while (sound.Status == SoundStatus.Playing)
                {
                    // Display the playing position
                    Console.CursorLeft = 0;
                    Console.Write("Playing... " + sound.PlayingOffset + " sec     ");

                    // Leave some CPU time for other threads
                    Thread.Sleep(100);
                }
            }

            // Finished !
            Console.WriteLine("\nDone !");
        }
    }
}
