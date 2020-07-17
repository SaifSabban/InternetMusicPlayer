/* InternetMusicPlayer

  This Work Was Uses a Serial MP3 PLayer From Catalex, and a Particle Photon.
  InternetMusicPlayer was coded by Saif Sabban. This is an opensource Project.

  This is the main body of the InternetMusicPlayer code which contains most
  of the functions needed to run the InternetMusicPlayer.

  See the included readme for additional information.
*/

/*---------------------------*//* Statments Functions *//*---------------------------*/
#include "Communications.h" // Calls On Communication Header.

/*---------------------------*//* Particle Functions *//*---------------------------*/
int MusicState(String command) { // Choose if you want to pay pause or stop a song. 0 = Pause, 1 = Play, 2 = Stop. Set Using The Particle Function.
  command = command.toUpperCase(); // Makes the incomming command all uppercase.
  if (command == "PUASE" || command == "0") { // If any of these are typed in, songs will pause.
    Music = "Paused"; // Print Paused on music particle variable.
    Serial.println(F("Received Internet Command: PUASE"));
    sendCommand(CMD_PAUSE, 0X00);
    return 0;
  }
  else if (command == "PLAY" || command == "1") { // If any of these are typed in, songs will play.
    Music = "PLAYING"; // Print Playing on music particle variable.
    Serial.println(F("Received Internet Command: PLAYING"));
    sendCommand(CMD_PLAY, 0X00);
    return 1;
  }
  else if (command == "STOP" || command == "2") { // If any of these are typed in, songs will Stop.
    Music = "STOPPED"; // Print Stopped on music particle variable.
    Serial.println(F("Received Internet Command: STOPPED"));
    sendCommand(STOP_PLAY, 0X00);
    return 2;
  }
  else {// If anything else is inputted an 999 is returned.
    return 999;
  }
}

int SkipSong(String command) { // Choose if you want to skip to next or previous song. 0 = Previous Song, 1 = Next Song. Set Using The Particle Function.
  command = command.toUpperCase(); // Makes the incomming command all uppercase.
  if (command == "PREVIOUS" || command == "0" || command == "PREV") { // If any of these are typed in, player will go to Previous Song.
    Serial.println(F("Received Internet Command: PREVIOUS"));
    sendCommand(PREV_SONG, 0X00);
    return 0;
  }
  else if (command == "NEXT" || command == "1" || command == "NXT") { // If any of these are typed in, player will go to Next Song.
    Serial.println(F("Received Internet Command: NEXT"));
    sendCommand(NEXT_SONG, 0X00);
    return 1;
  }
  else {// If anything else is inputted an 999 is returned.
    return 999;
  }
}

int Volume(String command) { // Choose what volme level you want, you can only go between 0 and 100. Set Using The Particle Function.
  unsigned int commandint = command.toInt(); // Makes the incomming command into an integer.
  if (commandint >= 0 && commandint <= 100) {
    VolLevel = commandint;
    int Percent = (commandint * 30);
    Percent = (Percent / 100);
    Serial.print(F("Received Internet Command: Set Volume To ")); Serial.println(VolLevel);
    sendCommand(CMD_SET_VOLUME, Percent);
    return VolLevel;
  }
  else {// If anything else is inputted an 999 is returned.
    return 999;
  }
}

int Cycle(String command) { // Choose if you want to cycle the current folder. 0 = Stop Cycle, 1 = Allow Cycle. Set Using The Particle Function.
  command = command.toUpperCase(); // Makes the incomming command all uppercase.
  if (command == "OFF" || command == "0") { // If any of these are typed in, player Stop Cycling folder.
    CycleState = 0;
    Serial.println(F("Received Internet Command: Music Cycle OFF"));
    sendCommand(SET_CYCLEPLAY, 0X01);
    return 0;
  }
  else if (command == "ON" || command == "1") { // If any of these are typed in, player Start Cycling folder.
    CycleState = 1;
    Serial.println(F("Received Internet Command: Music Cycle ON"));
    sendCommand(SET_CYCLEPLAY, 0X00);
    return 1;
  }
  else {// If anything else is inputted an 999 is returned.
    return 999;
  }
}

int SpeakerState(String command) { // Choose if you want to enable or disable speaker. Set Using The Particle Function.
  command = command.toUpperCase(); // Makes the incomming command all uppercase.
  if (command == "OFF" || command == "0") { // If any of these are typed in, speaker will be disabled.
    SpeakerStat = "OFF";
    Serial.println(F("Received Internet Command: SPEAKER OFF"));
    digitalWrite(Speaker, LOW);
    return 0;
  }
  else if (command == "ON" || command == "1") { // If any of these are typed in, speaker will be enabled.
    SpeakerStat = "ON";
    Serial.println(F("Received Internet Command: SPEAKER ON"));
    digitalWrite(Speaker, HIGH);
    return 1;
  }
  else {// If anything else is inputted an 999 is returned.
    return 999;
  }
}

int MP3PlayerState(String command) { // Choose if you want to have player go to sleep, wakeup, or reset. Set Using The Particle Function.
  command = command.toUpperCase(); // Makes the incomming command all uppercase.
  if (command == "SLEEP" || command == "0" || command == "SLP") { // If any of these are typed in, player goes to sleep.
    PlayerState = "Sleeping";
    Serial.println(F("Received Internet Command: SLEEP Mode"));
    sendCommand(SLEEP_MODE_START, 0X00);
    return 0;
  }
  else if (command == "WAKE" || command == "1" || command == "WKE") { // If any of these are typed in, player Wakes from sleep.
    PlayerState = "Awake";
    Serial.println(F("Received Internet Command: WAKE Mode"));
    sendCommand(SLEEP_MODE_WAKEUP, 0X00);
    return 1;
  }
  else if (command == "RESET" || command == "2" || command == "RST") { // If any of these are typed in, player resets.
    PlayerState = 1;
    Serial.println(F("Received Internet Command: RESET Mode"));
    sendCommand(CMD_RESET, 0X00);
    delay(200);
    sendCommand(CMD_SEL_DEV, DEV_TF);
    delay(200);
    sendCommand(CMD_SET_VOLUME, 0X1E);
    delay(200);
    return 2;
  }
  else {// If anything else is inputted an 999 is returned.
    return 999;
  }
}

int PlaySong(String command) { // Choose what song you want to play from current file, you can only go between 0 and 255. Set Using The Particle Function.
  unsigned int commandint = command.toInt(); // Makes the incomming command into an integer.
  if (commandint >= 0 && commandint <= 255) {
    Song = commandint;
    Serial.print(F("Received Internet Command: Set play Song ")); Serial.println(Song);
    sendCommand(CMD_PLAY_W_INDEX, (Song));
    return Song;
  }
  else {// If anything else is inputted an 999 is returned.
    return 999;
  }
}

int PlayFolder(String command) { // Choose a file & play the first song, you can only go between 0 and 255. Set Using The Particle Function.
  unsigned int commandint = command.toInt(); // Makes the incomming command into an integer.
  if (commandint >= 0 && commandint <= 255) {
    Folder = commandint;
    Serial.print(F("Received Internet Command: Set Folder ")); Serial.println(Folder);
    sendCommand(PLAY_FOLDER, (Folder));
    return Folder;
  }
  else {// If anything else is inputted an 999 is returned.
    return 999;
  }
}

int PlayFolderSong(String command) { // Choose a song & file to play, you can only go between 0 and 65535. Set Using The Particle Function. song limit 0-654, folder limit 0-99
  unsigned int commandint = command.toInt(); // Makes the incomming command into an integer.
  if (commandint >= 0 && commandint <= 65535) {
    int Value2 = (commandint % 100);
    Folder = Value2;
    int Value1 = (commandint - Value2) / 100;
    Song = Value1;
    Serial.print(F("Received Internet Command: Set Folder ")); Serial.print(Folder); Serial.print(F(" & Set Song ")); Serial.println(Song);
    sendCommand(CMD_PLAY_WITHFOLDER, ((Value1 * 256) + Value2));
    return Folder;
  }
  else {// If anything else is inputted an 999 is returned.
    return 999;
  }
}

int Restart_0(String command) { // Restarts Particle Photon, Yes Or 0. Set Using The Particle Function.
  command = command.toUpperCase(); // Makes the incomming command all uppercase.
  if (command == "YES" || command == "0") { // If yes or 0 are entered then the SevenSegClock will restart.
    void(* resetFunc) (void) = 0;
    resetFunc();
  }
  else { // If anything else is inputted an 999 is returned.
    return 999;
  }
}

/*---------------------------*//* Player Functions *//*---------------------------*/
void setup() {
  Serial.begin(9600); // Start our Serial coms for serial monitor in our pc.
  Serial1.begin(9600); // Start our Serial coms for the MP3 player.

  pinMode(Speaker, OUTPUT); // Set Speaker Pin As Output.
  digitalWrite(Speaker, HIGH); // Set Speaker pin on high.

  delay(1000); // Wait chip initialization is complete.
  sendCommand(CMD_RESET, 0X00);
  delay(200);
  sendCommand(CMD_SEL_DEV, DEV_TF); // select the TF card.
  delay(200);
  sendCommand(CMD_SET_VOLUME, 0X1E);
  delay(200);

  Particle.function("01_MusicState", MusicState); // Plays, Pause, or Stop Current Song.
  Particle.function("02_SkipSong", SkipSong); // Skips Current Song.
  Particle.function("03_Volume", Volume); // Sets Volume Level.
  Particle.function("04_Cycle", Cycle); // Sets Cycle State.
  Particle.function("05_SpeakerState", SpeakerState); // Enable/Disable Speaker.
  Particle.function("06_MP3PlayerState", MP3PlayerState); // Sleep, Wake, Reset Player.
  Particle.function("07_PlaySong", PlaySong); // Go To Sepcific Song On Current Folder.
  Particle.function("08_PlayFolder", PlayFolder); // Go To Sepcific Folder, & First Song on That Folder.
  Particle.function("09_PlayFolderSong", PlayFolderSong); // Go To Specific Song & Sepcific Folder,.
  Particle.function("10_Restart_0", Restart_0); // Restarts Particle Photon.

  Particle.variable("1_Music", Music); // Shows what state music is at Playing, Paused, or Stopped.
  Particle.variable("2_VolLevel", VolLevel); // Shows Current Volume Level.
  Particle.variable("3_CycleState", CycleState); // Shows Current Cycle State.
  Particle.variable("4_SpeakerState", SpeakerStat); // Shows Current Speaker State.
  Particle.variable("5_PlayerState", PlayerState); // Shows Current MP3 Player State.
  Particle.variable("6_Song", Song); // Shows Current Song.
  Particle.variable("7_Folder", Folder); // Shows Current Folder (Folder 0 is all folders).

}

void loop() { // Main Loop.
  Coms();  // Serial Communication Protocol.
}