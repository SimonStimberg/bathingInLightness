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