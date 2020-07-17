// Simon Stimberg - September 2019

class Ground {

  int cols, rows, scl;
  color c1, c2;
  float zOff;
  
  float pollution;


  Ground(int c, int r, int s) {
    
    cols = c;
    rows = r;
    scl = s;    
    
    colorMode(HSB, 360, 100, 100);
    c1 = color(189, 100, 66);
    c2 = color(11, 76, 75);
    
    pollution = 0;

    //c1 = #008ea8;
    //c2 = color(191, 71, 45);    

    zOff = 0.0;
  }



  void update() {
    
    colorMode(HSB, 360, 100, 100);
    int hue = int(map(pollution, 0, 1.0, 189, 168));
    c1 = color(hue, 100, 66);
    
    
    // iterate over the grid and assign color shift values using Perlin Noise
    // resulting in a smooth gradient background
    for (int i = 0; i < cols; i++) {
      for (int j = 0; j < rows; j++) {

        float xOff = i*0.01;
        float yOff = j*0.01;
        float intensity = noise(xOff, yOff, zOff) * 1.0;
        
        colorMode(RGB);
        color inter = lerpColor(c1, c2, intensity);
        noStroke();
        fill(inter);
        rect(i*scl, j*scl, scl, scl);
        
      }
    }
    zOff += 0.005;

  }
}





class Waves {

  float yOff;
  int wavesAmount;
  float resolution;
  float[] wavesY;


  Waves() {
    yOff = 0;
    wavesAmount = 50;
    resolution = width / (wavesAmount-2);

    wavesY = new float[wavesAmount];
  }


  // returns the water surface depth (y) at a given x position
  // is needed to know when a food particle exactly enters the water
  float checkSurface(float x) {

    int index = floor(x/resolution);
    float interPos = (x - resolution * index) / resolution;
    float interY = lerp(wavesY[index], wavesY[index+1], interPos);
    return interY;
  }


  void render(float depth) {
    fill(0);
    noStroke();
    float xOff = 0.0;

    beginShape(); 

    // Iterate over horizontal wave points
    for (int i = 0; i < wavesAmount; i++) {
      
      // Calculate a y value according to noise
      wavesY[i] = map(noise(xOff, yOff), 0, 1, depth, depth+100);

      float x = i * resolution;
      // Set the vertex
      vertex(x, wavesY[i]); 
      // Increment x dimension for noise
      xOff += 0.05;
    }
    // increment y dimension for noise
    yOff += 0.005;
    
    // close the shape
    vertex(width, 0);
    vertex(0, 0);
    endShape(CLOSE);
  }
}
