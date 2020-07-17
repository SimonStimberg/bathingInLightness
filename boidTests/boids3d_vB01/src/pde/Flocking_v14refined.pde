// Simon Stimberg - September 2019
//
// Code based on the teachings from 
// The Nature of Code by Daniel Shiffman
// http://natureofcode.com
//
// which implements Craig Reynolds' "Flocking" and "Steering" behavior
// See: http://www.red3d.com/cwr/



Flock flock;
Wall wall;
Ground ground;
Waves waves;
Food food;

float depth;
float initDepth;

boolean debug;



void setup() {

  size(1024, 768);

  // define the starting depth, resp. the position of the wave-line
  initDepth = 0.55;  
  depth = height * initDepth;

  // initialize all elements
  food = new Food();
  waves = new Waves();  
  newWall();
  flock = new Flock();

  // the ground consists of a grid of squares - comparable to "big pixels"
  // which make it much more performative, computing the color changing background
  int gridSize = 16;
  int cols = width / gridSize;
  int rows = height / gridSize;
  ground = new Ground(cols, rows, gridSize);

  debug = true;
}



void draw() {

  adjustDepth();
  ground.update();
  waves.render(depth);
  food.update(waves);
  flock.update();
  flock.render();
  drawGate();
  // wall.display();   // display the boundaries for debugging purposes
  

  if (debug) {
    fill(0);
    rect(0, 0, width, 20);
    fill(255);
    text("Framerate: " + int(frameRate), 10, 16);
    text("Boids: " + int(flock.boids.size()), 100, 16);
  }
}



public void keyPressed() {
  if (key == '1') {
    flock.addBoid(1);
  }
  if (key == '2') {
    flock.addBoid(2);
  }
  if (key == ' ') {
    food.spawn();
  }
  if (key == 'k') {
    flock.killFlock();
  }
}



// adjusts the depth according to the amount of boids
// while smoothly transitioning the water level using a lerp function
void adjustDepth() {
  
  ground.pollution = map(constrain(flock.boids.size(), 0, 300), 0, 300, 0.0, 1.0);

  float newDepth = height * constrain(map(flock.boids.size(), 0, 300, initDepth, 0.0), 0.0, initDepth);

  // only lerp, if depth differs from the newDepth
  // since lerp always approximates but never reaches the target value we enhance the threshhold by 0.5
  if (depth > newDepth+0.5 || depth < newDepth-0.5) {

    float oldDepth = depth;
    depth = lerp(depth, newDepth, 0.05);

    // also adjust the boundaries for boid behavior
    int mode = 0;
    if (depth < height*0.4-50) mode = 1;  
    if (depth < height*0.4-150) mode = 2;  
    wall.adjustWall(oldDepth - depth, mode);
  }
}



// draws the cavities of the gateways
// all positions are relative to the canvas size for easier adaption to various screen dimensions
void drawGate() {

  ellipseMode(CORNER);
  noStroke();
  fill(0);
  rect(width*0.146, height*0.675+width*0.062, width*0.124, height*0.245);
  ellipse(width*0.146, height*0.675, width*0.124, width*0.124);

  rect(width-width*0.27, height*0.675+width*0.062, width*0.124, height*0.245);
  ellipse(width-width*0.27, height*0.675, width*0.124, width*0.124);

  rect(width*0.377, height*0.4+width*0.123, width*0.246, height*0.44);
  ellipse(width*0.377, height*0.4, width*0.246, width*0.246);
}



// describes the boundaries of the space the boids are allowed to move in
//
// this is a bit of a hacky approach - since the boids don't behave well at corners with an angle of 90° or less,
// we ease out those corners to ensure the boids are moving smooth 
void newWall() {

  wall = new Wall();

  float offset = 30; // defines the amount of smoothness

  wall.addPoint(0, offset+depth+65);
  wall.addPoint(offset, 0+depth+65);

  wall.addPoint(width*0.377-offset*2, 0+depth+65);
  wall.addPoint(width*0.377, 0+depth+65+offset*2);
  wall.addPoint(width-width*0.377, 0+depth+65+offset*2);
  wall.addPoint(width-width*0.377+offset*2, 0+depth+65);


  wall.addPoint(width-offset, 0+depth+65);
  wall.addPoint(width, offset+depth+65); 
  wall.addPoint(width, height-offset);
  wall.addPoint(width-offset, height);

  wall.addPoint(width-width*0.146+offset, height);
  wall.addPoint(width-width*0.146, height-offset);
  wall.addPoint(width-width*0.146, height*0.675+offset*2);
  wall.addPoint(width-width*0.146-offset*2, height*0.675);
  wall.addPoint(width-width*0.27+offset*2, height*0.675);
  wall.addPoint(width-width*0.27, height*0.675+offset*2);
  wall.addPoint(width-width*0.27, height-offset);
  wall.addPoint(width-width*0.27-offset, height);

  wall.addPoint(width-width*0.377+offset, height);
  wall.addPoint(width-width*0.377, height-offset);
  wall.addPoint(width-width*0.377, height*0.4+offset*3);
  wall.addPoint(width-width*0.377-offset*3, height*0.4);
  wall.addPoint(width*0.377+offset*3, height*0.4);
  wall.addPoint(width*0.377, height*0.4+offset*3);
  wall.addPoint(width*0.377, height-offset);
  wall.addPoint(width*0.377-offset, height);

  wall.addPoint(width*0.27+offset, height);
  wall.addPoint(width*0.27, height-offset);
  wall.addPoint(width*0.27, height*0.675+offset*2);
  wall.addPoint(width*0.27-offset*2, height*0.675);
  wall.addPoint(width*0.146+offset*2, height*0.675);
  wall.addPoint(width*0.146, height*0.675+offset*2);
  wall.addPoint(width*0.146, height-offset);
  wall.addPoint(width*0.146-offset, height);

  wall.addPoint(offset, height);
  wall.addPoint(0, height-offset);
}
