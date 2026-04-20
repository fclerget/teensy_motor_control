# teensy_motor_control
This code is used for motor control via a Teensy 4.1 microcontroller
It is meant to be used with a ROS2 architecture to receive the data from
This project was developed using the Teensyduino extension on the Arduino IDE

Author(s) : Fiya Clerget, Gianni Dragos

Contact : sophiaclerget@gmail.com

==THIS REPO CONTAINS 4 FILES==

serial_test.ino - this file is used to test and debug the serial transport and make sure the data is being sent correctly
                  includes debug print statements to make sure the process is working correctly

teensy_serial.ino - this is the final file that combines the receive and send code into one file that can be loaded onto the
                    Teensy 4.1

motor_test.ino - this file was written by Gianni Dragos and was used to test sending PWM and DIR to the motor driver this project uses

gamepad_motor - this file was also written by Gianni Dragos and allows for a gamepad to be used to send the commands to the driver
