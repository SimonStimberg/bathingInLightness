// based on The Nature of Code - Flocking Example
// by Daniel Shiffman
// https://natureofcode.com/book/chapter-6-autonomous-agents/
// https://github.com/nature-of-code/noc-examples-processing/tree/master/chp06_agents


class Boid {

  PVector position;
  PVector velocity;
  PVector acceleration;
  float r;
  float maxforce;    // Maximum steering force
  float maxspeed;    // Maximum speed
  float wandertheta;
  
  boolean die;


  Boid(int gate) {

    float y = height * 0.9;
    float x = 0;
    float velY = random(-1, 1);
    float velX = 0;
    
    die = false;


    // which gate to spawn
    if (gate == 1) {
      x = width*0.146;
    } else 
    if (gate == 2) {
      x = width*0.73;
    }

    // distribute randomly on the left and right side of the gateway
    if (int(random(2)) < 1) {
      velX = random(-1, -0.2);
    } else {
      x += width*0.124;
      velX = random(0.2, 1);
    }


    acceleration = new PVector(0, 0);
    velocity = new PVector(velX, velY);
    position = new PVector(x, y);
    r = random(3.0, 6.0);  // size of the boid
    maxspeed = random(2.0, 4.0);  // max speed a boid can move
    maxforce = 0.05;
    wandertheta = 0;
  }
  
  void die() {
    die = true;
  }


  void run(ArrayList<Boid> boids) {
    behavior(boids);
    update();
  }

  void applyForce(PVector force) {
    // We could add mass here if we want A = F / M
    acceleration.add(force);
  }


  // applies different rules of behavior and weights them
  void behavior(ArrayList<Boid> boids) {
    
    // the three classic rules for flock behavior
    PVector sep = separate(boids);   // Separation
    PVector ali = align(boids);      // Alignment
    PVector coh = cohesion(boids);   // Cohesion

    // weight these forces
    sep.mult(1.5);
    ali.mult(1.0);
    coh.mult(1.0);

    // Add the force vectors to acceleration
    applyForce(sep);
    applyForce(ali);
    applyForce(coh);
    
    if(!die) {
      // check for world boundaries and steer away from them
      PVector bnd = boundaries(wall);
      bnd.mult(4.0);  // weight the force pretty strong to avoid running into walls
      applyForce(bnd);
    }
 
    // if there is food, get the food
    if (food.stuff.size() > 0) {
      PVector urgeToEat = getFood();
      urgeToEat.mult(3.0);
      applyForce(urgeToEat);
    }

    // if no other boids are close - start wandering around
    if (sep.mag() == 0 && ali.mag() == 0 && coh.mag() == 0) applyForce(wander());
    
  }

  // Method to update position
  void update() {
    // Update velocity
    velocity.add(acceleration);
    // Limit speed
    velocity.limit(maxspeed);
    position.add(velocity);
    // Reset accelertion to 0 each cycle
    acceleration.mult(0);
  }


  // A method that calculates and applies a steering force towards a target
  // STEER = DESIRED MINUS VELOCITY
  PVector seek(PVector target) {
    PVector desired = PVector.sub(target, position);  // A vector pointing from the position to the target
    // Normalize desired and scale to maximum speed
    desired.normalize();
    desired.mult(maxspeed);
    // Steering = Desired minus Velocity
    PVector steer = PVector.sub(desired, velocity);
    steer.limit(maxforce);  // Limit to maximum steering force
    return steer;
  }


  void render() {
    // Draw the silhouette rotated in the direction of velocity
    float theta = velocity.heading() + radians(90);
    fill(0);
    noStroke();
    pushMatrix();
    translate(position.x, position.y);
    rotate(theta);
    beginShape();
    vertex(0, -r*3);
    bezierVertex(r*2.25, -r*2.25, r, r, 0, r*3);
    bezierVertex(-r, r, -r*2.25, -r*2.25, 0, -r*3);
    endShape();
    popMatrix();
  }



  // search for and steer towards food
  PVector getFood() {

    float sensationRadius = 300; // radius in which the boid is attracted towards the food
    PVector closestFood = null;
    float closestDist = 1000000;  // an arbitrary but very high starting value that will be underbid
    int foodIndex = -1;
    
    // iterate over the food array and choose the closest one that already has entered the water
    for (int i = 0; i < food.stuff.size(); i++) {
      
      Stuff s = food.stuff.get(i);
      PVector distance = PVector.sub(s.pos, position);
      
      if (s.inLiquid && distance.mag() < closestDist ) {
        closestDist = distance.mag();
        closestFood = s.pos.copy();
        foodIndex = i;
      }
    }

    // if the closest food found is inside the sensation radius, steer towards it
    if (closestDist < sensationRadius) {   

      if (closestDist < 5) {
        food.eat(foodIndex);
      }
      return seek(closestFood);
    } else {
      return new PVector(0, 0);
    }
  }



  // if approaching the world boundaries (defined by the wall class)
  // steer away from it
  PVector boundaries(Wall p) {

    // Predict position 15 frames ahead
    PVector predict = velocity.get();
    predict.mult(15);
    PVector predictpos = PVector.add(position, predict);
   
    PVector target = predictpos.copy();
    boolean wallDetected = false;


    // iterate over all segments of the boundary and steer away if any is too close 
    // for this we calculate the reflection vector depending on the boids' current velocity
    for (int i = 0; i < p.points.size(); i++) {

      // Look at a line segment
      PVector a = p.points.get(i);
      PVector b = p.points.get((i+1)%p.points.size()); // Note Path has to wraparound

      // Get the normal point to that line (meaning the closest point on the segment to our predicted position)
      PVector normalPoint = getNormalPoint(predictpos, a, b);
      PVector dir = PVector.sub(b, a); // the direction vector of the wall segment
      
      // Check if normal is on the line segment
      if (normalPoint.x < min(a.x, b.x) || normalPoint.x > max(a.x, b.x) || normalPoint.y < min(a.y, b.y) || normalPoint.y > max(a.y, b.y)) {
        
        // if it is outside the line segment: do nothing

      } else {

        // How far away are we from the wall?
        float d = PVector.dist(predictpos, normalPoint);


        // the extreme case: if we are very close, meaning short to collision then reverse velocity
        // this ensures the boid doesn't crosses the boundary by all means
        if (d < 1) {
          
          velocity.mult(-1);

        // the normal case: if we are approaching a wall, add force to steer away from it
        } else if (d <= p.radius) {
          
          wallDetected = true;

          // calculate the angle between my velocity and the wall
          float theta = PVector.angleBetween(dir, velocity);

          // calculate the reflection vector used as desired target to steer away from the wall
          // for this use trigonomy
          float gk = sin(theta) * predict.mag(); // "Gegenkathete"

          PVector pointer = PVector.sub(predictpos, normalPoint); // get direction between predicted position and the wall
          pointer.normalize();
          pointer.mult(5+gk*2); // move away from the wall by double its distance

          target.add(pointer);
        }
      }
    }

    if (wallDetected) {
      return seek(target);  // Steer towards the position
    } else {
      return new PVector(0, 0);
    }
  }    



  // A function to get the normal point from a point (p) to a line segment (a-b)
  // This function could be optimized to make fewer new Vector objects
  PVector getNormalPoint(PVector p, PVector a, PVector b) {
    // Vector from a to p
    PVector ap = PVector.sub(p, a);
    // Vector from a to b
    PVector ab = PVector.sub(b, a);
    ab.normalize(); // Normalize the line
    // Project vector "diff" onto line by using the dot product
    ab.mult(ap.dot(ab));
    PVector normalPoint = PVector.add(a, ab);
    return normalPoint;
  }



  // let the boid wander around randomly if alone 
  PVector wander() {
    
    float wanderR = 65;         // Radius for our "wander circle"
    float wanderD = 80;         // Distance for our "wander circle"
    float change = 0.3;
    wandertheta += random(-change, change);     // Randomly change wander theta

    // Now we have to calculate the new position to steer towards on the wander circle
    PVector circlepos = velocity.get();    // Start with velocity
    circlepos.normalize();            // Normalize to get heading
    circlepos.mult(wanderD);          // Multiply by distance
    circlepos.add(position);               // Make it relative to boid's position

    float h = velocity.heading();        // We need to know the heading to offset wandertheta

    PVector circleOffSet = new PVector(wanderR*cos(wandertheta+h), wanderR*sin(wandertheta+h));
    PVector target = PVector.add(circlepos, circleOffSet);

    return seek(target);
  }



  // Separation
  // Method checks for nearby boids and steers away
  PVector separate (ArrayList<Boid> boids) {
    float desiredseparation = 25.0f;
    PVector steer = new PVector(0, 0, 0);
    int count = 0;
    // For every boid in the system, check if it's too close
    for (Boid other : boids) {
      float d = PVector.dist(position, other.position);
      // If the distance is greater than 0 and less than an arbitrary amount (0 when you are yourself)
      if ((d > 0) && (d < desiredseparation)) {
        // Calculate vector pointing away from neighbor
        PVector diff = PVector.sub(position, other.position);
        diff.normalize();
        diff.div(d);        // Weight by distance
        steer.add(diff);
        count++;            // Keep track of how many
      }
    }
    // Average -- divide by how many
    if (count > 0) {
      steer.div((float)count);
    }

    // As long as the vector is greater than 0
    if (steer.mag() > 0) {
      // Implement Reynolds: Steering = Desired - Velocity
      steer.normalize();
      steer.mult(maxspeed);
      steer.sub(velocity);
      steer.limit(maxforce);
    }
    return steer;
  }

  // Alignment
  // For every nearby boid in the system, calculate the average velocity
  PVector align (ArrayList<Boid> boids) {
    float neighbordist = 50;
    PVector sum = new PVector(0, 0);
    int count = 0;
    for (Boid other : boids) {
      float d = PVector.dist(position, other.position);
      if ((d > 0) && (d < neighbordist)) {
        sum.add(other.velocity);
        count++;
      }
    }
    if (count > 0) {
      sum.div((float)count);
      sum.normalize();
      sum.mult(maxspeed);
      PVector steer = PVector.sub(sum, velocity);
      steer.limit(maxforce);
      return steer;
    } else {
      return new PVector(0, 0);
    }
  }

  // Cohesion
  // For the average position (i.e. center) of all nearby boids, calculate steering vector towards that position
  PVector cohesion (ArrayList<Boid> boids) {
    float neighbordist = 50;
    PVector sum = new PVector(0, 0);   // Start with empty vector to accumulate all positions
    int count = 0;
    for (Boid other : boids) {
      float d = PVector.dist(position, other.position);
      if ((d > 0) && (d < neighbordist)) {
        sum.add(other.position); // Add position
        count++;
      }
    }
    if (count > 0) {
      sum.div(count);
      return seek(sum);  // Steer towards the position
    } else {
      return new PVector(0, 0);
    }
  }
}
