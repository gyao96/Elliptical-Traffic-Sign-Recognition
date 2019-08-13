> Achieved as a team of three @ Zhejiang University
> Final Course project of *Photoelectric Information Processing Comprehensive Experiment*

------------------------------------------

## Project demo
<center><iframe width="728" height="410" src="https://www.youtube.com/embed/x5zLtLUOzrE" frameborder="0" allow="autoplay; encrypted-media" allowfullscreen></iframe></center>

[View project report](Resources/TSR_ZJU.pdf)

------------------------------------------
## Introduction
Traffic Sign Recognition (TSR) has been an important topic of autonomous driving and assisted driving systems. Here we implemented *fast ellipse detection algorithm* proposed by [Fornaciari M. et al.](Ref/TSR.pdf) And then constructed and trained a 4-layer Convolutional Neural Network to train the classification model.

- Segment out the region of interest containing the traffic signs
- Classify the segmented image.

Then we tested our model on real data streams and deploy it onto a raspi 3b+ driven vehicle. The camera on the vehicle continuously streams video data to our computer via Ethernet or WiFi. After running the algorithm, an instruction code is sent back to the vehicle to perform corresponding actions accordingly.

![System Preview](Resources/cloud.png)

### Fast Ellipse Detection Algorithm
Please <a href="/Resources/TSR.pdf">Download our report</a> and read through Chapter 2.1 for the details of how we modify and implement our version of the algorithm. Code is written in c++-11 under directory `/server/EllipseDetectorYaed.cpp`.

In short, the algorithm can be decomposed to the following steps:
- Arch extraction
- Arch clusterring
- Center estimation
- Parameter evaluation

The overall running speed of segmenting one frame is between 5 to 15 ms, which is much more efficient than traditional Hough-transform method.

### Training
Please refer to Chapter 2.2 in <a href="/Resources/TSR_ZJU.pdf">our report</a> for more details.
##### Obtaining the data set
We ran the fast ellipse detection algorithm on 12 traffic sign pictures, namely "Left-Turn", "Stop", etc. This way, all the data we capture are all tagged and containing only the ROI. For the all 5730 datas we have, 4500 were used as the training set, and the rest verification set.
###### Demension Reduction
We use first normalize our raw image data to 40-by-40 grayscale matrices. Then run _Principle Component Analysis_ to further reduce the feature into 388 pixel nodes.
###### Building the Back-propagation Model
There are 4 layers in our model, one input, two hidden and one output. Input layer has 388 nodes, 300100 nodes for each hidden layer and 13 nodes for output layer (one negative class).
###### Precision
![Precision](Resources/precision.png)

### Vehicle Control Finite-State-Machine
![System FSM](/Resources/fsm.png)
*Note: The code for controlling the vehicle is not included in this repository*

## Compiling the Project
#### getDataSet
For building dataSet
#### getVideoSet
You maybe need some videos before "getDataSet"
#### server
main function
#### usage
* cd ./server <br>
* cmake . <br>
* make <br>
* ./test <br>
* Server will start while waiting for client. Enjoy it!
#### Environment
* Ubuntu 16.04
* opencv2.4.9

## Ref
1. [The German Traffic Sign Recognition Benchmark: A multi-class classification competition. Proceedings of International Joint Conference on Neural Networks, San Jose, California, USA, July 31 – August 5, 2011](Ref/06033395.pdf)
1. [Michele Fornaciari, Andrea Prati, Rita Cucchiara, A fast and effective ellipse detector for embedded vision applications Pattern Recognition, Volume 47, Issue 11, November 2014, Pages 3693-3708, ISSN 0031-3203](Ref/HSI.pdf)
1. [Road Sign Detection and Recognition System for Real-Time Embedded Applications. International Conference on Electrical, Control and Computer Engineering Pahang, Malaysia, June 21-22, 2011](Ref/hsi2.pdf)
1. [Fornaciari M, Prati A, Cucchiara R. A fast and effective ellipse detector for embedded vision applications[J\]. Pattern Recognition, 2014, 47(11): 3693-3708.](Ref/TSR.pdf)
