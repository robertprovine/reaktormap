# reaktormap
Reaktor Sample Map Generator

   REAKTORMAP

   Generates a Reaktor Sample Map from a templated folder structure loaded
   with custom samples. Samples are mapped to different velocities for up to
   128 keys, and the number of samples per key are also summarized in a
   comma separated text file that can be used in a table structure within
   Reaktor Core. This has only been tested with Reaktor 6.0.1 (R1526).

   DIRECTIONS

   1. Create a new directory with a unique name of your choice. We will refer
      to this directory as the 'main directory' in further directions.

   2. Inside this main directory, create one or more 'key' directories (no more
      than 128 directories total), and name them as follows:

      NUMBER (between 0 - 127) + UNDERSCORE + CUSTOM NAME

      correct examples: 36_Kicks  4_Swells  08_Snares  126_Wah_Sounds
      incorrect examples: _36Kicks  Snares  25HiHats

      Each numerical prefix represents the note number to which the velocities
      will be mapped. Make sure there are no duplicate number prefixes. The
      prefixes should be distinct. At this point, I would also recommend
      creating a copy of the main directory and its contents elsewhere as a
      template for future use.

   3. Drag/copy/save audio files of your choice into these key directories (no
      more than 127 total audio files per key directory)

      example: drag/copy 25 kick sounds into 36_Kicks, 40 snares into 37_Snares

   4. Open Terminal and navigate to the main directory with the cd command.
      Run reaktormap. This will create a Reaktor Sample Map file and
      corresponding text file. Each note number (mapped by the numerical
      prefix in each key directory name) can store up to 127 different sounds
      (sounds accessed by velocity values between 1 and 127), allowing a
      maximum total of 16,256 sounds to be loaded (128 x 127). The text file
      generated contains 128 comma separated values, each representing the
      number of audio files per key. This file can be loaded into the table
      structure in Reaktor Core to provide modular access to these values.
      This is helpful in order to prevent out of bounds indexing of
      velocities in Reaktor Sample Maps.

   Notes: Sample maps refer to the sound files by absolute path, so if
          the main directory is moved, reaktormap must be run again to update
          the new absolute paths.

          Don't forget to load the corresponding text file into a Reaktor
          Core table module each time you replace a sample map (if you wish
          to use the table data).

          Sample maps (.map) and text files (.txt) can moved elsewhere once they've been
          created if you wish.
