// Simon Stimberg - September 2019


// a higher order food class
class Food {

  ArrayList<Stuff> stuff;
  float liquidDrag = 0.9;


  Food() {
    stuff = new ArrayList<Stuff>(); // An arraylist to store the single food objects
  }


  void spawn() {
    // spawn the food either on the left or on the right side from the central gateway
    // and assign a random mass property
    float foodPos = (random(2) < 1) ? random(30, width*0.32) : random(width*0.68, width - 30);
    stuff.add(new Stuff(foodPos, random(1, 3)));
  }


  void update(Waves w) {
    vanish(); // remove food from the list that has been eaten up

    for (Stuff s : stuff) {

      // check if the food has hit the water
      if (s.pos.y > w.checkSurface(s.pos.x)) s.inLiquid = true;

      // if so add a drag force, simulating a liquid
      if (s.inLiquid) {
        s.drag(liquidDrag);
      }

      // apply gravity to the food particle according to its mass
      float m = 0.1*s.mass;
      PVector gravity = new PVector(0, m);
      s.applyForce(gravity);

      s.update();
      s.render();
    }
  }


  // mark food as eaten and thereby initialize decay process
  // is being called from the boid class
  void eat(int index) {
    Stuff s = stuff.get(index);
    if (!s.inDecay) {
      s.decay();
    }
  }


  // remove expired food
  void vanish() {
    for (int i = stuff.size() - 1; i >= 0; i--) {
      Stuff s = stuff.get(i);
      if (s.inDecay && millis() > s.expiration) {
        stuff.remove(i);
      }
    }
  }
}




// a class for the single food particles
class Stuff {

  PVector pos;
  PVector velocity;
  PVector acceleration;
  float mass;
  boolean inLiquid;

  int expiration;
  boolean inDecay;
  float noiseOffset;
  float size;
  int decayTime;


  Stuff(float posX, float m) {
    
    pos = new PVector(posX, 0);
    velocity = new PVector(0, 0);
    acceleration = new PVector(0, 0);
    mass = m;
    inLiquid = false;

    expiration = 0;
    inDecay = false;
    decayTime = 1200; // Time in milliseconds the food needs to be eaten up

    noiseOffset = 0;
  }


  void update() {
    velocity.add(acceleration);
    pos.add(velocity);

    // Now add clearing the acceleration each time!
    acceleration.mult(0);
  }


  void applyForce(PVector force) 
  {
    // Receive a force, divide by mass, and add to acceleration.
    PVector f = PVector.div(force, mass);
    acceleration.add(f);
  }


  // if in the water, add a contrary drag (simulating the liquid)
  void drag(float c) 
  {

    float speed = velocity.mag();
    // The force’s magnitude: Cd * v~2~
    float dragMagnitude = c * speed * speed;

    PVector drag = velocity.copy();
    drag.mult(-1);   // The force's direction: -1 * velocity

    // clamp the magnitude to not exceed the velocity
    dragMagnitude = constrain(dragMagnitude, 0, speed);

    // Finalize the force: magnitude and direction together.
    drag.normalize();      
    drag.mult(dragMagnitude);

    // Apply the force.
    applyForce(drag);
  }


  // set timestamp for expiration when the food is going to be removed 
  void decay() {
    expiration = millis() + decayTime;
    inDecay = true;
  }


  void render() {

    ellipseMode(CENTER);
    noStroke();
    float alpha = 200;
    if (inDecay) {
      alpha = map(expiration-millis(), decayTime, 0, 255, 0);
    }    
    fill(200, alpha);
    
    // add a bit movement to the horizontal dimension to create the appearance of floating in the water
    float floating = map(noise(noiseOffset), 0, 1, -10, 10);
    ellipse(pos.x + floating, pos.y, mass*3, mass*3); // 

    noiseOffset += 0.005;
  }
}
