// the new Y positions measured in cm from the base plate of the installation
// measure the positions and write them in a new sketch with the row-numbers as bulb number indicator
// then paste them here:
float[] newYs = {
  69.5, 
  179.5, 
  126.0, 
  139.0, 
  87.0, 
  169.5, 
  104.5, 
  172.5, 
  106.5, 
  149.5, 
  90.5, 
  78.5, 
  112.0, 
  157.5, 
  158.5, 
  163.0, 
  165.0, 
  163.0, 
  123.5, 
  170.5, 
  145.5, 
  145.5, 
  113.5, 
  146.0, 
  148.0, 
  155.5, 
  88.5, 
  87.5, 
  83.5, 
  130.0, 
  142.0, 
  111.5, 
  117.0, 
  134.0, 
  131.0, 
  133.0, 
  100.0, 
  105.5, 
  99.0, 
  143.0, 
  142.0, 
  112.0, 
  93.5, 
  95.5, 
  127.5, 
  133.5, 
  118.0, 
  113.0, 
  121.5, 
  136.5, 
  117.0, 
  127.0
};


int[] positions = {
  281, 324, 0, // Bulb No. 1
  304, 262, 960, // Bulb No. 2
  340, 347, 445, // Bulb No. 3
  229, 287, 590, // Bulb No. 4
  365, 262, 285, // Bulb No. 5
  277, 385, 730, // Bulb No. 6
  256, 221, 140, // Bulb No. 7
  400, 332, 70, // Bulb No. 8
  205, 346, 140, // Bulb No. 9
  343, 197, 680, // Bulb No. 10
  338, 409, 300, // Bulb No. 11
  192, 237, 315, // Bulb No. 12
  427, 272, 535, // Bulb No. 13
  223, 410, 850, // Bulb No. 14
  285, 167, 880, // Bulb No. 15
  410, 393, 775, // Bulb No. 16
  152, 306, 820, // Bulb No. 17
  408, 192, 925, // Bulb No. 18
  293, 457, 515, // Bulb No. 19
  197, 176, 910, // Bulb No. 20
  463, 322, 880, // Bulb No. 21
  161, 396, 705, // Bulb No. 22
  343, 132, 490, // Bulb No. 23
  388, 453, 865, // Bulb No. 24
  129, 245, 760, // Bulb No. 25
  467, 223, 795, // Bulb No. 26
  228, 473, 60, // Bulb No. 27
  238, 121, 265, // Bulb No. 28
  472, 390, 245, // Bulb No. 29
  109, 350, 600, // Bulb No. 30
  408, 131, 795, // Bulb No. 31
  334, 501, 325, // Bulb No. 32
  137, 173, 435, // Bulb No. 33
  509, 283, 635, // Bulb No. 34
  155, 456, 530, // Bulb No. 35
  301, 84, 640, // Bulb No. 36
  447, 462, 150, // Bulb No. 37
  79, 279, 175, // Bulb No. 38
  479, 164, 270, // Bulb No. 39
  269, 527, 630, // Bulb No. 40
  177, 105, 740, // Bulb No. 41
  525, 362, 380, // Bulb No. 42
  90, 408, 295, // Bulb No. 43
  383, 76, 210, // Bulb No. 44
  401, 518, 520, // Bulb No. 45
  77, 203, 560, // Bulb No. 46
  533, 220, 475, // Bulb No. 47
  174, 515, 350, // Bulb No. 48
  242, 57, 420, // Bulb No. 49
  507, 447, 605, // Bulb No. 50
  48, 342, 425, // Bulb No. 51
  468, 102, 580   // Bulb No. 52
};


void setup() {
  size(1280, 720);

  String[] bulbPositions = new String[newYs.length];

  int j = 0;
  for (int i = 0; i < positions.length; i+=3) {

    float newY = ( newYs[j] - newYs[0] ) * 10;  // convert from cm to mm and substract the lenght of the highest bulb (No. 1) which will be 0  
    j++;

    // data suitable for ofApp
    String data = "" + positions[i] + ", " + positions[i+1] + ", " + (int)newY + ",   // Bulb No. " + j;

    bulbPositions[j-1] = data;
  }


  saveStrings("newPositions.txt", bulbPositions);
}
