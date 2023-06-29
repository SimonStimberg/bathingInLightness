# Version History (Development Stage)

## Cluster01
- Integrated Kinect Image to Point Cloud into the Cluster System
- Map bulb intensites towards point cloud

## Cluster02
- added Synth into Cluster (Synth05-Version from separate develpment project)
- bulb intensites control oscillator levels

## Cluster03
- reintegrated Particle Swarm into Cluster

## Cluster04
- particles follow point cloud as attraction points and embrace the whole silhouette
- adjustments to the graphical interface for Demo Video (Mensch und Computer 2020)

## Cluster05
- separated the synth initialization into an own function, made it generative, meaning depending on the bulb cluster
- made the random cluster generative: an abitrary amount of bulbs can be added / optimized the random distribution (applied the paper's approach)
- cleaned positioning confusion: now everything is oriented around the center (-halfBox, halfBox)
- modified particle behaviour (more drag, more force -> stick closer to the silhouette)

## Cluster05b
- modifications for Mensch und Computer Live Demo presentation
  - switch for particle behaviour (physics) depending on if a camera input is given or the swarm is alone (used CurrentMode for this)
  - slight interface adjustments 
    - kinect point cloud and depth image can be switched seperately
  - depth image output clipping according to the clipping of the point cloud

## Cluster06
- tested and made adjustments to the settings for a bigger setup
- for a cloud with a diameter of 1,5m (= 500px) and 200 lamps
- adjustments to the cluster generation algorithm (equalisation of the distribution (too much density in the center)) 

## Cluster07
- adjustments for Zönoteka exhibition
- iteration of version Cluster05b
- adjusted bulb position values
- dismiss klinect points if they are outside the bounding box / don't allocate space for point vector if kinect not connected

## Cluster07b
- Implementation of Boid Swarm system
- customized the boid algorithm
  - introduced a scale factor which scales the dimensions of the system  
    so that in small worldsizes the flock can become smaller too, to avoid the "shark in the bottle"
  - added a getPosition function and a draw/drawScaled function
- introduced a check radius variable for the bulbs -> important to adjust the sensitivity
- changed baud rate to 38400 and refresh rate to 20ms
- added attraction according to the kinect points (tweaked the boid class for each boid being attracted by a different point)

## Cluster07c
- Iteration of 7b
- direct mapping of the kinect points and boids at the same time

## Cluster07d
- tilted camera implementation for installation setup
- light intensity as well controls filter cutoff frequency of the oscillator (changes made only to the synth files)

## Cluster07e
- using position vector in flock class to stor positions and distribute them via a reference
- tydied up files in creating subfolders for the different class files
- created a bulb cluster class to handle all of the bulb operations

## Cluster07f
- rectangular world size implemented
  - adjusted parameters correspondingly - also the creation for a random bulb cluster
- added a attraction point in the center to award the swarm for staying more in the center

## Cluster07g
- Zoenoteka exhibition: on site changes/implementations

## Cluster08
- Implementation of Skeleton Tracker
- changes/replacements in kinect class to implement the openNI stuff for tracking

## Cluster09
- flow calculation in the kinect class
  - the overall flow amount is being computed which determines the intensity impact of the kinect point cloud
  - with more movement the intensities getting higher and the sound gets louder (and cutoff filter opens)
  - the swarm is not affected by this

## Cluster10
- Multiple Kinect feature
  - the point cloud space is divided for 2 Kinects
  - the calculated flow is an average of the movement of all kinects together

## Cluster11
- 4 Channel Surround feature
  - self-made surround panner from combining 3 stereo panner
  - fall back for Stereo setup: overwrite the Y-panning with -1.0

## Cluster11b
- lab 30 exhibition on location changes
  - dim sound on mouse click
  - deactivated flow calculation - did not work well with 2 Kinects
  - open Kinect by Serial ID to make sure the right one is chosen first (to keep the same order)
  - adjusted the Bulb Positions to the new real world length corresponding to the exhibition setup
  - adjusted the 2 Kinect mapping/mirroring to real world conditions

## Cluster12a
- added demo points function to aid/check mapping of the bulbs
  - uses the Kinect Point Cloud array to create a grid of topdown moving points
  - the check radius of the bulbs has to be decreased to achieve a meaningful pattern 
- based on version Cluster11 - so before the lab 30 changes (!)

## Cluster12b
- merging the lab30 changes (v11b) with the new mapping function (from v12a)