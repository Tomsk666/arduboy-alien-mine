/*  Alien Mine by Tom Millichamp
    Alien Graphics by Jamie Millichamp
    February 2020
    Copyright 2020 Tom Millichamp
    Thanks to @Pharap for code improvements
*/

//TODO:
// Make 5 areas instead of 4
#include <Arduboy2.h>
#include <ArduboyTones.h>
#include <ArduboyTonesPitches.h>
#include <EEPROM.h>
Arduboy2 arduboy;
ArduboyTones sound(arduboy.audio.enabled); //enable sound (Arduboy Tones lib)

enum class GameState : uint8_t
{
  Title,
  Gameplay,
  Lose,
  Pause,
};
GameState gameState;

//create the 6 zones that make up the mine shaft
Point MIDDLE(65,32); // middle of the mine shaft
#include "ZonesSetup.h"
#include "Aliens.h"
//create an empty array of aliens
AlienList<15> aliens;
#include "Sprites.h"
#include "Sounds.h"

//Global Variables
int8_t current_zone = 0;
bool bullet_in_play = false;
int8_t bullet_zone = 0;
int8_t bullet_area = 2;
int score = 0;
int high_score = 0;
int8_t level = 1;
int alien_speed_level = 80; //level 1 speed
int8_t lives = 3;
int power_up = 0; //used for power up level for bomb
int counter=0; //just used for flashing text on title

void setup() {
  arduboy.begin(); // initialize Arduboy
  arduboy.setFrameRate(60);
  load_high_score(); //load the high score from EEPROM
  setUpZones(); //Setup the array of zone co-ordinates that make up the mine shaft
  current_zone = 0; //start at top zone (0 is top, then they go clockwise)
  bullet_in_play = false;
  bullet_zone = 0;
  bullet_area = 2;
  score = 0; 
  lives = 3;
  level = 1;
  power_up = 0;
  alien_speed_level = 80; //level 1 speed
  aliens.clear(); //clear any aliens in the alien array
  arduboy.clear();
  //show splash screen graphics for 2.5 secs
  arduboy.drawBitmap(0,0,Splash_Screen,128,48,WHITE);
  arduboy.setCursor(48, 42);
  arduboy.print(F("Alien"));
  arduboy.setCursor(48, 52);
  arduboy.print(F("Mine!"));
  arduboy.display();
  delay(2500);
  gameState=GameState::Title;
}

// MAIN LOOP //
void loop() {
  if (!arduboy.nextFrame())
    return;

  switch (gameState)
  {
    case GameState::Title:
      arduboy.clear();
      arduboy.print(F("Settings:\n"));
      arduboy.print(F("   B - Sound: "));
      arduboy.print((arduboy.audio.enabled()) ? F("on") : F("off"));
      arduboy.print(F("\nIn Play:"));
      arduboy.print(F("\n   L/R - Move"));
      arduboy.print(F("\n   A - Missile"));
      arduboy.print(F("\n   B - Bomb"));
      arduboy.print(F("\n   UP/DN - Pause"));
      counter++; // just counting number of frames that have passed to flash text
      if(counter < 25)
          arduboy.print(F("\nPress A to Start"));
      else
          arduboy.print(F("\n"));
      if(counter >= 35) 
          counter = 0;
      arduboy.display();
      arduboy.pollButtons();
      if (arduboy.justPressed(A_BUTTON)){
        gameState=GameState::Gameplay;
      }
      // B Button Toggle Sound On/Off
      if (arduboy.justPressed(B_BUTTON)) {
        arduboy.audio.toggle();
      }
    break;


    case GameState::Gameplay:
      arduboy.clear();
      arduboy.print(score);
      //arduboy.print(F("L "));
      Sprites::drawOverwrite(110,0,heart,0); //heart icon for lives
      arduboy.setCursor(120,0);
      arduboy.print(lives);
      arduboy.pollButtons();
      if (arduboy.justPressed(UP_BUTTON) || arduboy.justPressed(DOWN_BUTTON)) {
        gameState=GameState::Pause;
        break;
      }
      if (arduboy.justPressed(B_BUTTON) && power_up==20){
        //Player dropped nuke down mine!
        draw_mine_bomb();
        power_up=0;
        aliens.clear();
        break;
      }
      draw_cannon(); //draw players cannon 
      draw_zones(); //draw the mine
      draw_bomb_progress_bar();
      
      player_move(); //check if player is moving & move if is
      player_fire(); //check if player has fired a missile
      aliens_move();
      check_hit(); //added this twice now so bullets can't miss in a single frame
      add_alien(); //add random aliens from time to time
      draw_aliens(); 
      bullet_move();
      check_hit();
    break;


    case GameState::Lose:
      sound.tone(NOTE_C4,500, NOTE_C3,500, NOTE_C1,1000);
      high_score=(score>high_score) ? score : high_score;  // set the new hi-score if it is one
      save_high_score(); //write the high score to EEPROM
      arduboy.clear();
      arduboy.setCursor(8,0);
      arduboy.setTextSize(2);
      arduboy.print(F("Game Over!"));
      arduboy.setTextSize(1);
      arduboy.setCursor(0,24);
      arduboy.print(F("Score: ")); arduboy.print(score);
      arduboy.print(F("\nHi-Score: ")); arduboy.print(high_score);
      arduboy.print(F("\nPress B to replay"));
      arduboy.display();
      arduboy.waitNoButtons();
      do{
        //arduboy.pollButtons();
        }while (!arduboy.pressed(B_BUTTON));
      arduboy.waitNoButtons();
      setup();
    break;


    case GameState::Pause:
          // just keep looping until they press B button
          arduboy.setCursor((WIDTH/2)-16, HEIGHT/2);
          arduboy.print(F("PAUSED"));
          arduboy.setCursor((WIDTH/2)-16, (HEIGHT/2)+12);
          arduboy.print(F("Press B"));
          arduboy.display();
          arduboy.waitNoButtons();
          do{
            //arduboy.pollButtons();
            }while (!arduboy.pressed(B_BUTTON));
          arduboy.waitNoButtons();
          gameState=GameState::Gameplay;
      break;
  }
  arduboy.display();
} // end of main loop


// Methods //
void draw_zones(){
  //This draws the mine shaft
  //Co-ordinates are from the Zones array from ZonesSetup.h
  for (int i=0;i<6;i++){
    arduboy.drawLine (Zones[i].top_left.x,Zones[i].top_left.y,Zones[i].top_right.x,Zones[i].top_right.y,WHITE);
    arduboy.drawLine (Zones[i].bot_left.x,Zones[i].bot_left.y,Zones[i].bot_right.x,Zones[i].bot_right.y,WHITE);
    arduboy.drawLine (Zones[i].top_left.x,Zones[i].top_left.y,MIDDLE.x,MIDDLE.y);
    arduboy.drawLine (Zones[i].top_right.x,Zones[i].top_right.y,MIDDLE.x,MIDDLE.y);

  }
}

//draws the bomb progress bar at top of game screen
void draw_bomb_progress_bar(){
  arduboy.drawRect(55,0,20,4,WHITE);
  if (power_up != 0){
    arduboy.fillRect(55,0,power_up,4,WHITE);
  }
}

void draw_cannon(){
  //draws the players cannon
  //either uncomment the next line & use that instead of the Sprites in switch
  //arduboy.fillCircle(Zones[current_zone].cannon.x,Zones[current_zone].cannon.y,3,WHITE);
  switch (current_zone)
  {
  case 0:
    Sprites::drawOverwrite(62,7,cannon_0,0);
    break;
  case 1:
    Sprites::drawOverwrite(106,14,cannon_1,0);
    break;
  case 2:
    Sprites::drawOverwrite(105,41,cannon_2,0);
    break;
  case 3:
    Sprites::drawOverwrite(62,56,cannon_3,0);
    break;
  case 4:
    Sprites::drawOverwrite(17,42,cannon_4,0);
    break;
  case 5:
    Sprites::drawOverwrite(17,14,cannon_5,0);
    break;
  }
}

void draw_aliens(){
  uint8_t draw_zone, draw_area;
  //loop through all the aliens in our aliens array
  for(uint8_t i = 0; i < aliens.getCount(); ++i)
		{
			draw_zone = aliens[i].zone;
      draw_area = aliens[i].area;
      switch (draw_area)
      {
      case 0:
        Sprites::drawOverwrite(Zones[draw_zone].area[draw_area].x-2,Zones[draw_zone].area[draw_area].y-2,Alien_4,0);
        break;
      
      case 1:
        Sprites::drawOverwrite(Zones[draw_zone].area[draw_area].x-3,Zones[draw_zone].area[draw_area].y-3,Alien_6,0);
        break;

      case 2:
        Sprites::drawOverwrite(Zones[draw_zone].area[draw_area].x-4,Zones[draw_zone].area[draw_area].y-4,Alien_8,0);
        break;

      case 3:
        Sprites::drawOverwrite(Zones[draw_zone].area[draw_area].x-6,Zones[draw_zone].area[draw_area].y-6,Alien_12,0);
        break;

      case 4:
        Sprites::drawOverwrite(Zones[draw_zone].area[draw_area].x-6,Zones[draw_zone].area[draw_area].y-6,Alien_12,0);
        break;
      }
      //arduboy.fillCircle(Zones[draw_zone].area[draw_area].x,Zones[draw_zone].area[draw_area].y,draw_area+1,WHITE);
		}
}

void draw_mine_bomb(){
  //this draws a big explosion when they use B button to drop a bomb
  sound.tones(bomb_explosion_sound);
  //flash the screen for the Nuke!
  for (int i=0;i<3;i++){
    arduboy.setRGBled(255,0,0);
    arduboy.invert(true);
    delay(100);
    arduboy.setRGBled(0,0,0);
    arduboy.invert(false);
    delay(100);
  }
}

void aliens_move(){
  if (arduboy.everyXFrames(alien_speed_level)){
    for(uint8_t i = 0; i < aliens.getCount(); ++i)
      {
        aliens[i].area = aliens[i].area+1; //move the alien up the area of the zone
        //check if alien at top, if so lose a life & remove the alien from the array
        if (aliens[i].area > 3){
          sound.tone(NOTE_C1,1000);
          draw_alien_attack(aliens[i].zone, 3);
          aliens.removeAt(i);
          lives--;
          if (lives==0){
            gameState=GameState::Lose;
          }
        }
      }
  }
}

void check_hit(){
  //go through all aliens in play & check if player shot any
  for(uint8_t i = 0; i < aliens.getCount(); ++i)
      {
        //if the bullet zone & area is same as aliens, then we have a hit
        if (aliens[i].area == bullet_area && aliens[i].zone == bullet_zone && bullet_in_play){
          draw_explosion();
          aliens.removeAt(i); //remove the dead alien
          score+=10;
          //add to the bomb power up bar
          if (power_up < 20)
            power_up+=2;
          //levels
          if (score==300){
            level=2;
            alien_speed_level = 60;
            level_up();
          }
          if (score==500){
            level=3;
            alien_speed_level = 50;
            arduboy.setFrameRate(65);
            level_up();
          }
          if (score==800){
            level=4;
            alien_speed_level = 40;
            arduboy.setFrameRate(70);
            level_up();
          }
          if (score==1200){
            level=5;
            alien_speed_level = 30;
            arduboy.setFrameRate(80);
            level_up();
          }
          //now stop the current bullet, so ready to fire again
          bullet_in_play=false;
          bullet_area=2;
        }
      }
}

void level_up(){
  aliens.clear();
  arduboy.clear();
  arduboy.setCursor(20,10);
  arduboy.setTextSize(2);
  arduboy.print(F("Level "));
  arduboy.print(level);
  arduboy.setTextSize(1);
  arduboy.setCursor(45,40);
  arduboy.print("Press B");
  arduboy.display();
  arduboy.waitNoButtons();
  do{
    //arduboy.pollButtons();
  }while (!arduboy.pressed(B_BUTTON));
  arduboy.waitNoButtons();
}

void draw_explosion(){
  sound.tones(explosion_sound);
  const auto x = (Zones[bullet_zone].area[bullet_area].x - 4);
  const auto y = (Zones[bullet_zone].area[bullet_area].y - 4);
  for(uint8_t frame = 0; frame < 5; ++frame)
  {
    Sprites::drawOverwrite(x, y, explosions, 0);
    arduboy.display();
    delay(20);
  }
}

void draw_alien_attack(int8_t zone, int8_t area){
  //alien reached top of mine!
  arduboy.setRGBled(255, 0, 0);
  arduboy.setCursor(MIDDLE.x-18,MIDDLE.y-4);
  arduboy.print(F("BITTEN!"));
  arduboy.display();
  delay(1000);
  arduboy.setRGBled(0, 0, 0);
}

void add_alien(){
  //create aliens randomly, alien_speed_level is how many frames, level 1 is 80
  uint8_t alienZone;
  arduboy.initRandomSeed();
  if (arduboy.everyXFrames(alien_speed_level+random(1,alien_speed_level))){
    alienZone=random(0,6);
    aliens.add(Alien(0,alienZone));
  }
}

void player_move(){
  if (arduboy.justPressed(LEFT_BUTTON)){
        current_zone--;
        if (current_zone < 0) current_zone=5; //loop back round
      }
      if (arduboy.justPressed(RIGHT_BUTTON)){
        current_zone++;
        if (current_zone > 5) current_zone=0; //loop back round
      }
}

void player_fire(){
  //check player is able to fire as you can only have one bullet in play at a time
  if (arduboy.justPressed(A_BUTTON) && bullet_in_play==false){
        //player can fire
        sound.tone(NOTE_C5,10);
        bullet_in_play=true;
        bullet_zone = current_zone;
        bullet_area = 3; //bullet starts at the top of the mine (0 is bottom)
        arduboy.fillCircle(Zones[bullet_zone].area[bullet_area].x,Zones[bullet_zone].area[bullet_area].y,bullet_area+1,WHITE);
      }
}

//This method moves the missile down the mine shaft
void bullet_move(){
  if (bullet_in_play){
        //draw bullett
        arduboy.fillCircle(Zones[bullet_zone].area[bullet_area].x,Zones[bullet_zone].area[bullet_area].y,bullet_area+1,WHITE);
        //control speed of the bullet using frames
        if (arduboy.everyXFrames(10)){
          bullet_area--; //move it closer to the middle
          //check if bullet gone out of play without hitting anything:
          if (bullet_area<0){
            bullet_in_play=false;
            bullet_area=2;
          }
        }
    }
}

//High score EEPROM save Methods
void save_high_score(){
  uint16_t address = 256;

  uint32_t scoreData = high_score;
  EEPROM.update(address, scoreData);
  address += sizeof(uint32_t);
}
void load_high_score(){
  uint16_t address = 256;

  uint32_t scoreData = 0;
  EEPROM.get(address, scoreData);
  address += sizeof(uint32_t);
  high_score = scoreData;
}
