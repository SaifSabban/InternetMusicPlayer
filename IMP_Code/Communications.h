/* InternetMusicPlayer Coomunication Header

  This Header is used to set Vaiables and Communication Functions

  See the included readme for additional information.
*/


/*---------------------------*//* Particle/Amp Variables *//*---------------------------*/


String Music = "stopped"; // Variable to show state of music; Playing, Paused, or Stopped.
int VolLevel = 100; // Variable to Volume Level.
int CycleState = 0; // Variable to show state of Cycleign music; Enabled Or Not.
String SpeakerStat = "on"; // Variable to show state of Speaker; Enabled Or Not.
String PlayerState = "Awake"; // Variable to show state of MP3 Player; Sleeping, Awake.
int Song = 1; // Variable showing current song.
int Folder = 0; // Variable showing current folder.


/*---------------------------*//* MP3 Player communicaion Variables *//*---------------------------*/
const byte Speaker = WKP; // Pin Used To Control If Speaker Is Enabled Or Not.
uint8_t VOL; // Variable To Set Volume Level
static int8_t Send_buf[8] = {0} ; // The MP3 player undestands orders in a 8 integer

#define NEXT_SONG 0X01 // Command For going to next song
#define PREV_SONG 0X02 // Command For going to previous song

#define CMD_PLAY_W_INDEX 0X03 //DATA IS REQUIRED (number of song)

#define VOLUME_UP_ONE 0X04 // Command For going up one volume level (0-1E)
#define VOLUME_DOWN_ONE 0X05 // Command For going down one volume level (0-1E)
#define CMD_SET_VOLUME 0X06 // Command For going to exact volume level (0-1E)

#define CMD_SINGLE_CYCLE 0X08  // Command For playing a single cycle of folder, starting with the first song

#define CMD_SEL_DEV 0X09 // SELECT STORAGE DEVICE, DATA IS REQUIRED (Dont Play With unless you know what you're doing)

#define SLEEP_MODE_START 0X0A // Command For making player go to sleep
#define SLEEP_MODE_WAKEUP 0X0B // Command For waking player from sleep

#define CMD_RESET 0X0C // Command For resetting MP3 Player
#define CMD_PLAY 0X0D  // Command For Playing current Song
#define CMD_PAUSE 0X0E // Command For Pausing current Song

#define CMD_PLAY_WITHFOLDER 0X0F // Command For choosing a folder and song to play, 0x7E 06 0F 00 yy xx EF;(xx number for song)(yy number for folder)

#define STOP_PLAY 0X16 // Command For Stopping current song

#define PLAY_FOLDER 0X17// data is needed 0x7E 06 17 00 01 XX EF;(play the 01 folder)(value xx we dont care)/////////////////////////////////////////////////////////////////////

#define SET_CYCLEPLAY 0X19 // Command For cycling folder; 00 start, 01 close

#define CMD_PLAY_WITHVOLUME 0X22  // Command For Playing a spcific song at a predetermined volume 0x7E 06 22 00 xx yy EF;(xx volume)(yy number of song)


#define DEV_TF 0X02 // Ignore Ths Code, This Is just Needed For Initialization

/*---------------------------*//* MP3 Player communicaion Function *//*---------------------------*/
void sendCommand(int8_t command, int16_t dat) { // Function Used To Send M3 Player Command
  delay(20);
  Send_buf[0] = 0x7e; // Starting byte
  Send_buf[1] = 0xff; // Player version
  Send_buf[2] = 0x06; // The number of bytes of the command without starting byte and ending byte
  Send_buf[3] = command; // Main command requested
  Send_buf[4] = 0x00; // Feedback Request; 0x00 = no feedback, 0x01 = feedback
  Send_buf[5] = (int8_t)(dat >> 8); // Data 0
  Send_buf[6] = (int8_t)(dat); // Data 1
  Send_buf[7] = 0xef; // Ending byte
  Serial.print(F("Sending Code: "));
  for (uint8_t i = 0; i < 8; i++) { // Prints Command To Bonth Serial Monitor, and Mp3 Player
    Serial1.write(Send_buf[i]) ; // Sends bit to MP3 Player
    Serial.print(Send_buf[i], HEX); // Sends bit to serial monitor
  }
  Serial.println(F("\n"));
}

/*---------------------------*//* MP3 Player communicaion Function *//*---------------------------*/
void Coms() { // Serial communication protocol
  if (Serial.available() > 0) { // If serial information is available
    String Com_Command = Serial.readString(); // Reads commans
    int underscoreIndex = Com_Command.indexOf('_'); // Sets a code divider condition

    String SquenceText = Com_Command.substring(0, underscoreIndex); // First code split
    String SquenceNumber2 = Com_Command.substring(underscoreIndex + 1, underscoreIndex + 8); // Second code split
    unsigned int SquenceNumber = SquenceNumber2.toInt(); // Converts second code Split to integer
    SquenceText.toUpperCase(); // Converts first code Split to uppercase letters

    Serial.print(F("Received Serial Command: "));
    Serial.println(SquenceText + "_" + SquenceNumber);

    if (SquenceText == "PLAY") { // Play Song.
      Music = "PLAYING";
      sendCommand(CMD_PLAY, 0X00);
    }
    else if (SquenceText == "PAUSE") { // Pause Song.
      Music = "Paused";
      sendCommand(CMD_PAUSE, 0X00);
    }
    else if (SquenceText == "STOP") { // Go To Stop and Reset Song.
      Music = "STOPPED";
      sendCommand(STOP_PLAY, 0X00);
    }
    else if (SquenceText == "NEXT") { // Go To & Play Next Song.
      Song ++;
      sendCommand(NEXT_SONG, 0X00);
    }
    else if (SquenceText == "PREV" || SquenceText == "PREVIOUS") { // Go To & Play Previous Song.
      Song --;
      sendCommand(PREV_SONG, 0X00);
    }
    else if (SquenceText == "VOL-UP") { // Go One Level Up In Volume.
      if (VolLevel < 100) {
        VolLevel = VolLevel + 3.333;
      }
      else if (VolLevel >= 100) {
        VolLevel = 100;
      }
      sendCommand(VOLUME_UP_ONE, 0X00);
    }
    else if (SquenceText == "VOL-DW") { // Go One Level Down In Volume.
      if (VolLevel > 0) {
        VolLevel = VolLevel - 3.333;
      }
      else if (VolLevel <= 0) {
        VolLevel = 0;
      }
      sendCommand(VOLUME_DOWN_ONE, 0X00);
    }
    else if ((SquenceText == "VOL") && (SquenceNumber >= 0) && (SquenceNumber <= 100)) { // Go to Chosen Volume level, (0-100).
      VolLevel = SquenceNumber;
      int Percent = (SquenceNumber * 30);
      Percent = (Percent / 100);
      sendCommand(CMD_SET_VOLUME, Percent);
    }
    else if ((SquenceText == "PLAY-CF") && (SquenceNumber >= 0) && (SquenceNumber <= 255)) { // Play & Repeats All Songs From Chosen Folder, Always Stars With First Song On Flie.
      Song = 1;
      Folder = SquenceNumber;
      sendCommand(PLAY_FOLDER, (SquenceNumber * 256));
    }
    else if ((SquenceText == "PLAY-FF") && (SquenceNumber >= 0) && (SquenceNumber <= 65535))  { // Play Chosen Song At Chosen File.
      int Value2 = (SquenceNumber % 100);
      Folder = Value2;
      int Value1 = (SquenceNumber - Value2) / 100;
      Song = Value1;
      sendCommand(CMD_PLAY_WITHFOLDER, ((Value1 * 256) + Value2));
    }
    else if ((SquenceText == "PLAY-S") && (SquenceNumber >= 0) && (SquenceNumber <= 65535)) { // Play Chosen Song From Current File.
      Song = SquenceNumber;
      sendCommand(CMD_PLAY_W_INDEX, (SquenceNumber));
    }
    else if ((SquenceText == "PLAY-V") && (SquenceNumber >= 0) && (SquenceNumber <= 3099)) { // Play Chosen Song At Chosen Volume, (Volume goes up to 30, song goes up to 255).
      int Value2 = (SquenceNumber % 100);
      Song = Value2 * 100;
      Song = VolLevel / 30;
      int Value1 = (SquenceNumber - Value2) / 100;
      VolLevel = Value1;
      sendCommand(CMD_PLAY_WITHVOLUME, ((Value1 * 256) + Value2));
    }
    else if ((SquenceText == "CYCLESONG") && (SquenceNumber >= 0) && (SquenceNumber <= 65535)) {  // Continually Cycle Folder & Starts From A Chosen Song.
      CycleState = 1;
      sendCommand(CMD_SINGLE_CYCLE, (SquenceNumber));
    }
    else if (SquenceText == "CYCLE-ON") { // Continually Cycle The Current Song, when playing.
      CycleState = 1;
      sendCommand(SET_CYCLEPLAY, 0X00);
    }
    else if (SquenceText == "CYCLE-OFF") { // Stop Cycling The Current Song, when playing (WON'T STOP SONG ONLY FROM SONG REPLAYING).
      CycleState = 0;
      sendCommand(SET_CYCLEPLAY, 0X01);
    }
    else if (SquenceText == "SLEEP") { // Go to Sleep Mode.
      PlayerState = "Sleeping";
      sendCommand(SLEEP_MODE_START, 0X00);
    }
    else if (SquenceText == "WAKE") { // Wake From Sleep Mode.
      PlayerState = "Awake";
      sendCommand(SLEEP_MODE_WAKEUP, 0X00);
    }
    else if (SquenceText == "RESET") { // Reset The Chip.
      sendCommand(CMD_RESET, 0X00);
      delay(200);
      sendCommand(CMD_SEL_DEV, DEV_TF);
      delay(200);
      sendCommand(CMD_SET_VOLUME, 0X1E);
      delay(200);
    }
    else if (SquenceText == "SPON") { // SPEAKER is On.
      SpeakerStat = "ON";
      digitalWrite(Speaker, HIGH);
    }
    else if (SquenceText == "SPOFF") { // SPEAKER is OFF.
      SpeakerStat = "OFF";
      digitalWrite(Speaker, LOW);
    }
    else if (SquenceText == "RST") { // Reset Microcontroller
      void(* resetFunc) (void) = 0;
      resetFunc();
    }
    else { // If anything else is inputted an 999 is returned
      Serial.println(F("Unknown Serial Command Received\n"));
    }
  }
}