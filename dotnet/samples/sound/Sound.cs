using System;
using System.Threading;
using SFML;
using SFML.Audio;

namespace sample_sound
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
            SoundBuffer Buffer = new SoundBuffer("datas/sound/footsteps.wav");

            // Display sound informations
            Console.WriteLine("footsteps.wav :");
            Console.WriteLine(" " + Buffer.Duration      + " sec");
            Console.WriteLine(" " + Buffer.SampleRate    + " samples / sec");
            Console.WriteLine(" " + Buffer.ChannelsCount + " channels");

            // Create a sound instance and play it
            Sound Sound = new Sound(Buffer);
            Sound.Play();

            // Loop while the sound is playing
            while (Sound.Status == SoundStatus.Playing)
            {
                // Display the playing position
                Console.CursorLeft = 0;
                Console.Write("Playing... " + Sound.PlayingOffset + " sec     ");

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
            Music Music = new Music("datas/sound/lepidoptera.ogg");

            // Display music informations
            Console.WriteLine("lepidoptera.ogg :");
            Console.WriteLine(" " + Music.Duration      + " sec");
            Console.WriteLine(" " + Music.SampleRate    + " samples / sec");
            Console.WriteLine(" " + Music.ChannelsCount + " channels");

            // Play it
            Music.Play();

            // Loop while the music is playing
            while (Music.Status == SoundStatus.Playing)
            {
                // Display the playing position
                Console.CursorLeft = 0;
                Console.Write("Playing... " + Music.PlayingOffset + " sec     ");

                // Leave some CPU time for other processes
                Thread.Sleep(100);
            }
        }
    }
}
