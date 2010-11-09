using System;
using System.Threading;
using SFML;
using SFML.Audio;

namespace sound
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        static void Main(string[] args)
        {
            // Play a sound
            PlaySound();
            Console.Clear();

            // Play a music
            PlayMusic();
        }

        /// <summary>
        /// Play a sound
        /// </summary>
        private static void PlaySound()
        {
            // Load a sound buffer from a wav file
            SoundBuffer buffer = new SoundBuffer("resources/canary.wav");

            // Display sound informations
            Console.WriteLine("canary.wav :");
            Console.WriteLine(" " + buffer.Duration      + " sec");
            Console.WriteLine(" " + buffer.SampleRate    + " samples / sec");
            Console.WriteLine(" " + buffer.ChannelsCount + " channels");

            // Create a sound instance and play it
            Sound sound = new Sound(buffer);
            sound.Play();

            // Loop while the sound is playing
            while (sound.Status == SoundStatus.Playing)
            {
                // Display the playing position
                Console.CursorLeft = 0;
                Console.Write("Playing... " + sound.PlayingOffset + " sec     ");

                // Leave some CPU time for other processes
                Thread.Sleep(100);
            }
        }

        /// <summary>
        /// Play a music
        /// </summary>
        private static void PlayMusic()
        {
            // Load an ogg music file
            Music music = new Music("resources/orchestral.ogg");

            // Display music informations
            Console.WriteLine("orchestral.ogg :");
            Console.WriteLine(" " + music.Duration      + " sec");
            Console.WriteLine(" " + music.SampleRate    + " samples / sec");
            Console.WriteLine(" " + music.ChannelsCount + " channels");

            // Play it
            music.Play();

            // Loop while the music is playing
            while (music.Status == SoundStatus.Playing)
            {
                // Display the playing position
                Console.CursorLeft = 0;
                Console.Write("Playing... " + music.PlayingOffset + " sec     ");

                // Leave some CPU time for other processes
                Thread.Sleep(100);
            }
        }
    }
}
