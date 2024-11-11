# Description
This repo holds the code for solving a persistant problem of mine: Forgetting to close the garage door when I leave for work in the morning. 

When wired up to the proper components, this code uses an ultra-sonic sonar to determine if the car is in the garage, a limit-switch to
determine if the garage door is open, and a relay + wires with alagator clips to terminals on the garage door opener to trigger the door
to close.

Specifically, the core logic in main.ino amounts to:
* If the garage door is open and the car is absent, start a 20 sec timer
* If the garage door is closed or the car is present, reset the timer
* If the timer expires, trigger the door to close
* If someone waves their hand directly in front of the sonar, wait for 10 seconds, then trigger the door to close.

This last rule is useful if you don't have the fob on you and wish to close the door with the car still inside with yourself ending up outside 
after the door is closed. Previously this required, pushing the wall button at the back of the garage, then running to the front of the garage, 
and simultaiously jumpingover the safety beam while ducking under the closing door. Now I can simply waive my hand in front of the sonar and 
walk leasurly out of the garage.

# TODO
- [ ] Include links to components
- [ ] Include a wiring diagram
- [ ] Include a picture link to a video of the working device
- [ ] Port to Rust
