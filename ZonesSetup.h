//This creates an Array (Zones) of 6 zones which make up the mine shaft
//each zone has a top line, bottom line, position for cannon, 
//and also area (array) where the bullets & aliens will appear in that zone
class Zone
{
  public:
    Point top_left,top_right,bot_left,bot_right;
    Point cannon;
    Point area[4];
};

Zone Zones[6]; // create an array to hold the 6 zones in the game

void setUpZones(){
    //This builds the wire frame of the mine (6 zones)
    Zones[0].top_left.x = 40;
    Zones[0].top_left.y = 10;
    Zones[0].top_right.x = 90;
    Zones[0].top_right.y = 10;
    Zones[0].bot_left.x = 56;
    Zones[0].bot_left.y = 25;
    Zones[0].bot_right.x = 74;
    Zones[0].bot_right.y = 25;
    //Now the areas in the zone where bullets/aliens are drawn
    Zones[0].area[0].x = 65; 
    Zones[0].area[0].y = 26;
    Zones[0].area[1].x = 65; 
    Zones[0].area[1].y = 20;
    Zones[0].area[2].x = 65; 
    Zones[0].area[2].y = 14;
    Zones[0].area[3].x = 65; 
    Zones[0].area[3].y = 10;
    // finally where cannon will appear at top to shoot from
    Zones[0].cannon.x = 65;
    Zones[0].cannon.y = 10;


    Zones[1].top_left.x = 90;
    Zones[1].top_left.y = 10;
    Zones[1].top_right.x = 120;
    Zones[1].top_right.y = 32;
    Zones[1].bot_left.x = 74;
    Zones[1].bot_left.y = 25;
    Zones[1].bot_right.x = 80;
    Zones[1].bot_right.y = 32;
    //Now the areas in the zone where bullets/aliens are drawn
    Zones[1].area[0].x = 74; 
    Zones[1].area[0].y = 29;
    Zones[1].area[1].x = 90; 
    Zones[1].area[1].y = 25;
    Zones[1].area[2].x = 100; 
    Zones[1].area[2].y = 23;
    Zones[1].area[3].x = 106; 
    Zones[1].area[3].y = 23;
    // finally where cannon will appear at top to shoot from
    Zones[1].cannon.x = 106;
    Zones[1].cannon.y = 23;

    Zones[2].top_left.x = 120;
    Zones[2].top_left.y = 32;
    Zones[2].top_right.x = 90;
    Zones[2].top_right.y = 54;
    Zones[2].bot_left.x = 80;
    Zones[2].bot_left.y = 32;
    Zones[2].bot_right.x = 74;
    Zones[2].bot_right.y = 39;
    //Now the areas in the zone where bullets/aliens are drawn
    Zones[2].area[0].x = 74; 
    Zones[2].area[0].y = 35;
    Zones[2].area[1].x = 87; 
    Zones[2].area[1].y = 40; //
    Zones[2].area[2].x = 100; 
    Zones[2].area[2].y = 41;
    Zones[2].area[3].x = 106; 
    Zones[2].area[3].y = 42;
    // finally where cannon will appear at top to shoot from
    Zones[2].cannon.x = 106;
    Zones[2].cannon.y = 42;

    Zones[3].top_left.x = 90;
    Zones[3].top_left.y = 54;
    Zones[3].top_right.x = 40;
    Zones[3].top_right.y = 54;
    Zones[3].bot_left.x = 74;
    Zones[3].bot_left.y = 39;
    Zones[3].bot_right.x = 56;
    Zones[3].bot_right.y = 39;
    //Now the areas in the zone where bullets/aliens are drawn
    Zones[3].area[0].x = 65; 
    Zones[3].area[0].y = 38;
    Zones[3].area[1].x = 65; 
    Zones[3].area[1].y = 44;
    Zones[3].area[2].x = 65; 
    Zones[3].area[2].y = 50;
    Zones[3].area[3].x = 65; 
    Zones[3].area[3].y = 54;
    // finally where cannon will appear at top to shoot from
    Zones[3].cannon.x = 65;
    Zones[3].cannon.y = 54;

    Zones[4].top_left.x = 40;
    Zones[4].top_left.y = 54;
    Zones[4].top_right.x = 10;
    Zones[4].top_right.y = 32;
    Zones[4].bot_left.x = 56;
    Zones[4].bot_left.y = 39;
    Zones[4].bot_right.x = 50;
    Zones[4].bot_right.y = 32;
    //Now he areas in the zone where bullets/aliens are drawn
    Zones[4].area[0].x = 55; 
    Zones[4].area[0].y = 36;
    Zones[4].area[1].x = 40; 
    Zones[4].area[1].y = 39;
    Zones[4].area[2].x = 30; 
    Zones[4].area[2].y = 40;
    Zones[4].area[3].x = 24; 
    Zones[4].area[3].y = 42;
    // finally where cannon will appear at top to shoot from
    Zones[4].cannon.x = 24;
    Zones[4].cannon.y = 42;

    Zones[5].top_left.x = 10;
    Zones[5].top_left.y = 32;
    Zones[5].top_right.x = 40;
    Zones[5].top_right.y = 10;
    Zones[5].bot_left.x = 50;
    Zones[5].bot_left.y = 32;
    Zones[5].bot_right.x = 56;
    Zones[5].bot_right.y = 25;
    //Now the areas in the zone where bullets/aliens are drawn
    Zones[5].area[0].x = 55; 
    Zones[5].area[0].y = 29;
    Zones[5].area[1].x = 42; 
    Zones[5].area[1].y = 26;
    Zones[5].area[2].x = 30; 
    Zones[5].area[2].y = 23;
    Zones[5].area[3].x = 24; 
    Zones[5].area[3].y = 22;
    // finally where cannon will appear at top to shoot from
    Zones[5].cannon.x = 24;
    Zones[5].cannon.y = 22;
}

