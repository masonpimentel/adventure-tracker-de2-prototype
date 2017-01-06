# AdventureTrackerDE2

Android application can be found here: https://github.com/snxfz947/AdventureTrackerAndroid

Adventure Tracker is a wearable device, used to log the position and velocity of a user over a set interval, and at the end of a run, present the user with interesting data such as total distance traveled, change in altitude, average velocity, maximum velocity, time elapsed, etc. With this information, users will be able to track their progress and record personal achievements across a variety of outdoor sports, including hiking, skiing, and biking.

# Features

* Real time location information
* Real time path drawing
* 3 user modes that correspond to sampling rate:
  * Walking
  * Biking
  * Skiing
* Past trips cumulative data
* Total time
* Total distance
* Total altitude change
* Past trips path drawing

## Past Trips

In the past trips page, the user is shown:
* The log file they are currently reading from
* Total cumulative stats including total time, distance and change in altitude
* The full drawn path starting at the S and ending at E

When first entering this page after boot up the most recent log file is shown.

<img src="https://github.com/snxfz947/AdventureTrackerDE2/blob/master/Images/20160301_122751.jpg" width="500"><br>_Past trips_

The user is able to press on “Prev” and “Next” to cycle between log files on the SD card. Pressing Next when displaying the last log file wraps around back to log 0. Pressing Prev when displaying log 0 wraps forward to the most recent log file. The user is able to return to the Main Menu by pressing the button on the bottom right.

## New Trips

In the new trips page the user is shown:
* The current log file being written to in the SD card
* Current location information including the current time, latitude, longitude and altitude
* The path taken since the start of the trip

The user is able to press on “Walking”, “Biking” and “Skiing” to cycle between the three different location sampling rates. Walking corresponds to a rate of every 5 seconds, biking every 3 seconds and skiing every 2 seconds. By pressing “End Trip” on the bottom right, the trip is ended, and upon re-entering this page from the main menu, a new log file is created.


<img src="https://github.com/snxfz947/AdventureTrackerDE2/blob/master/Images/20160301_133041.jpg" width="500"><br>_New trip_<br>Note that it is not shown here, but the feature to also log points of interest was added to this page.

<img src="https://github.com/snxfz947/AdventureTrackerDE2/blob/master/Images/20160301_133221.jpg" width="500"><br>_Biking mode_

<img src="https://github.com/snxfz947/AdventureTrackerDE2/blob/master/Images/20160301_133240.jpg" width="500"><br>_Skiing mode_

<img src="https://github.com/snxfz947/AdventureTrackerDE2/blob/master/Images/20160303_134653.jpg" width="500"><br>_Attempting to get a fix_

## Bluetooth Settings

In the new trips page the user is shown:
* Name of device
* PIN settings

The user is able to change the name of the device and the PIN here.

<img src="https://github.com/snxfz947/AdventureTrackerDE2/blob/master/Images/Bluetooth.png" width="250"><br>_Bluetooth settings_

<img src="https://github.com/snxfz947/AdventureTrackerDE2/blob/master/Images/Bluetooth2.png" width="250"><br>_Changing PIN_

## Hardware Block Diagram

<img src="https://github.com/snxfz947/AdventureTrackerDE2/blob/master/Images/Block.png" width="500">

# Source Structure

* QuartusProject 
  * Contains all the HDL files
* SoftwareFiles 
  * Contains all the C code for programming the NIOS processor
