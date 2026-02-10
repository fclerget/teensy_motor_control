# teensy_motor_control
This code is used for motor control via a Teensy 4.1 microcontroller
It is meant to be used with a ROS2 architecture to recieve the data from
This project was developed using the Teensyduino extension on the Arduino IDE

-THIS REPO CONTAINS 4 FILES-\n
serial_test.ino - this file is used to test and debug the serial transport and make sure the data is being sent correctly
                  includes debug print statements to make sure the process is working correctly

teensy_serial_recieve.ino - this file removes the debug print statements and just recieves the serial data from a Jetson Nano

teensy_serial_send.ino - this file sends the data to the motor drivers

teensy_serial.ino - this is the final file that combines the recieve and send code into one file that can be loaded onto the
                    Teensy 4.1
