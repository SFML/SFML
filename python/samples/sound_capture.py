#!/usr/bin/env python

from PySFML import sf

def Main():
	# Check that the device can capture audio
	if sf.SoundRecorder.IsAvailable() == False:
		print "Sorry, audio capture is not supported by your system"
		return

	# Choose the sample rate
	SampleRate = 0
	SampleRate = int(raw_input("Please choose the sample rate for sound capture (44100 is CD quality) : "))

	# Wait for user input...
	print "Press enter to start recording audio"
	raw_input()

	# Here we'll use an integrated custom recorder, which saves the captured data into a sfSoundBuffer
	Recorder = sf.SoundBufferRecorder()

	# Audio capture is done in a separate thread, so we can block the main thread while it is capturing
	Recorder.Start(SampleRate)
	print "Recording... press enter to stop"
	raw_input()
	Recorder.Stop()

	# Get the buffer containing the captured data
	Buffer = Recorder.GetBuffer()

	# Display captured sound informations
	print "Sound information :"
	print " " + str(Buffer.GetDuration())      + " seconds"
	print " " + str(Buffer.GetSampleRate())    + " samples / seconds"
	print " " + str(Buffer.GetChannelsCount()) + " channels"

	# Choose what to do with the recorded sound data
	Choice = str(raw_input("What do you want to do with captured sound (p = play, s = save) ? "))

	if Choice == 's':
		# Choose the filename
		Filename = str(raw_input("Choose the file to create : "))

		# Save the buffer
		Buffer.SaveToFile(Filename);
	else:
		# Create a sound instance and play it
		Sound = sf.Sound(Buffer)
		Sound.Play()

		# Wait until finished
		while Sound.GetStatus() == sf.Sound.Playing:
			# Display the playing position - I don't know how to do this in python
			# std::cout << "\rPlaying... " << std::fixed << std::setprecision(2) << Sound.GetPlayingOffset() << " sec";

			# Leave some CPU time for other threads
			sf.Sleep(0.1)

	# Finished !
	print "Done !"

	# Wait until the user presses 'enter' key
	print "Press enter to exit..."
	raw_input()

	return

Main()
