/**
	@mainpage SOIL

	Jonathan Dummer
	2007-07-26-10.36

	Simple OpenGL Image Library

	A tiny c library for uploading images as
	textures into OpenGL.  Also saving and
	loading of images is supported.

	I'm using Sean's Tool Box image loader as a base:
	http://www.nothings.org/

	I'm upgrading it to load TGA and DDS files, and a direct
	path for loading DDS files straight into OpenGL textures,
	when applicable.

	Image Formats:
	- BMP		load & save
	- TGA		load & save
	- DDS		load & save
	- PNG		load
	- JPG		load

	OpenGL Texture Features:
	- resample to power-of-two sizes
	- MIPmap generation
	- compressed texture S3TC formats (if supported)
	- can pre-multiply alpha for you, for better compositing
	- can flip image about the y-axis (except pre-compressed DDS files)

	Thanks to:
	* Sean Barret - for the awesome stb_image
	* Dan Venkitachalam - for finding some non-compliant DDS files, and patching some explicit casts
	* everybody at gamedev.net
**/

#ifndef HEADER_SIMPLE_OPENGL_IMAGE_LIBRARY
#define HEADER_SIMPLE_OPENGL_IMAGE_LIBRARY

#ifdef __cplusplus
extern "C" {
#endif

/**
	The format of images that may be loaded (force_channels).
	SOIL_LOAD_AUTO leaves the image in whatever format it was found.
	SOIL_LOAD_L forces the image to load as Luminous (greyscale)
	SOIL_LOAD_LA forces the image to load as Luminous with Alpha
	SOIL_LOAD_RGB forces the image to load as Red Green Blue
	SOIL_LOAD_RGBA forces the image to load as Red Green Blue Alpha
**/
enum
{
	SOIL_LOAD_AUTO = 0,
	SOIL_LOAD_L = 1,
	SOIL_LOAD_LA = 2,
	SOIL_LOAD_RGB = 3,
	SOIL_LOAD_RGBA = 4
};

/**
	The types of images that may be saved.
	(TGA supports uncompressed RGB / RGBA)
	(BMP supports uncompressed RGB)
	(DDS supports DXT1 and DXT5)
**/
enum
{
	SOIL_SAVE_TYPE_TGA = 0,
	SOIL_SAVE_TYPE_BMP = 1,
	SOIL_SAVE_TYPE_DDS = 2
};

/**
	Loads an image from disk into an array of unsigned chars.
	Note that *channels return the original channel count of the
	image.  If force_channels was other than SOIL_LOAD_AUTO,
	the resulting image has force_channels, but *channels may be
	different (if the original image had a different channel
	count).
	\return 0 if failed, otherwise returns 1
**/
unsigned char*
	SOIL_load_image
	(
		const char *filename,
		int *width, int *height, int *channels,
		int force_channels
	);

/**
	Loads an image from memory into an array of unsigned chars.
	Note that *channels return the original channel count of the
	image.  If force_channels was other than SOIL_LOAD_AUTO,
	the resulting image has force_channels, but *channels may be
	different (if the original image had a different channel
	count).
	\return 0 if failed, otherwise returns 1
**/
unsigned char*
	SOIL_load_image_from_memory
	(
		const unsigned char *const buffer,
		int buffer_length,
		int *width, int *height, int *channels,
		int force_channels
	);

/**
	Saves an image from an array of unsigned chars (RGBA) to disk
	\return 0 if failed, otherwise returns 1
**/
int
	SOIL_save_image
	(
		const char *filename,
		int image_type,
		int width, int height, int channels,
		const unsigned char *const data
	);

/**
	Frees the image data (note, this is just C's "free()"...this function is
	present mostly so C++ programmers don't forget to use "free()" and call
	"delete []" instead [8^)
**/
void
	SOIL_free_image_data
	(
		unsigned char *img_data
	);

/**
	This function resturn a pointer to a string describing the last thing
	that happened inside SOIL.  It can be used to determine why an image
	failed to load.
**/
const char*
	SOIL_last_result
	(
		void
	);


#ifdef __cplusplus
}
#endif

#endif /* HEADER_SIMPLE_OPENGL_IMAGE_LIBRARY	*/
