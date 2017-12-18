import java.io.*;

class CreateReaktorMap
{
  static String directoryName;

  public static void main(String[] args)
  {
    if (args.length < 1) {
      System.out.println("missing argument - name of directory");
      System.exit(0);
    }

    directoryName = args[0];

    createDirectory(directoryName);
    createGMStandardDrumMap();
  }

  static void createDirectory(String directoryName)
  {
    File theDir = new File(directoryName);

    // if the directory does not exist, create it
    if (!theDir.exists()) {
      boolean result = false;

      try {
        theDir.mkdir();
        result = true;
      } 
      catch(SecurityException se) {
        //handle it
        System.out.println(se);  
      }        
      if(result) {    
        System.out.println("directory created: " + directoryName);
      }
    }
    else {
      System.out.print("Directory \"" + directoryName + "\" ");  
      System.out.print("already exists\n");  
    }
  }

  static void createGMStandardDrumMap()
  {
    createDirectory(directoryName + "/" + "35_Acoustic_Bass_Drum");
    createDirectory(directoryName + "/" + "36_Bass_Drum_1");
    createDirectory(directoryName + "/" + "37_Side_Stick");
    createDirectory(directoryName + "/" + "38_Acoustic_Snare");
    createDirectory(directoryName + "/" + "39_Hand_Clap");
    createDirectory(directoryName + "/" + "40_Electric_Snare");
    createDirectory(directoryName + "/" + "41_Low_Floor_Tom");
    createDirectory(directoryName + "/" + "42_Closed_Hi_Hat");
    createDirectory(directoryName + "/" + "43_High_Floor_Tom");
    createDirectory(directoryName + "/" + "44_Pedal_High_Hat");
    createDirectory(directoryName + "/" + "45_Low_Tom");
    createDirectory(directoryName + "/" + "46_Open_Hi_Hat");
    createDirectory(directoryName + "/" + "47_Low_Mid_Tom");
    createDirectory(directoryName + "/" + "48_Hi_Mid_Tom");
    createDirectory(directoryName + "/" + "49_Crash_Cymbal_1");
    createDirectory(directoryName + "/" + "50_High_Tom");
    createDirectory(directoryName + "/" + "51_Ride_Cymbal_1");
    createDirectory(directoryName + "/" + "52_Chinese_Cymbal");
    createDirectory(directoryName + "/" + "53_Ride_Bell");
    createDirectory(directoryName + "/" + "54_Tambourine");
    createDirectory(directoryName + "/" + "55_Splash_Cymbal");
    createDirectory(directoryName + "/" + "56_Cowbell");
    createDirectory(directoryName + "/" + "57_Crash_Cymbal_2");
    createDirectory(directoryName + "/" + "58_Vibraslap");
    createDirectory(directoryName + "/" + "59_Ride_Cymbal_2");
    createDirectory(directoryName + "/" + "60_Hi_Bongo");
    createDirectory(directoryName + "/" + "61_Low_Bongo");
    createDirectory(directoryName + "/" + "62_Mute_Hi_Conga");
    createDirectory(directoryName + "/" + "63_Open_Hi_Conga");
    createDirectory(directoryName + "/" + "64_Low_Conga");
  }
}
