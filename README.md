# Arduino Drone Flight Controller

This repository contains the code for an Arduino-based drone flight controller.

## Overview

This flight controller uses an Arduino Uno to control the speed of four brushless DC motors through Electronic Speed Controllers (ESCs). The inputs are received from a FlySky I6 Radio Controller through a FlySky FS-Ia6B Radio Receiver.

## Hardware Connections

- **Motor ESC Connections:**
  - Front Left Motor (FL): Pin 7
  - Front Right Motor (FR): Pin 4
  - Rear Left Motor (RL): Pin 6
  - Rear Right Motor (RR): Pin 5

- **Receiver Channels:**
  - Yaw (Channel 1): Pin 8
  - Pitch (Channel 2): Pin 9
  - Throttle (Channel 3): Pin 10
  - Roll (Channel 4): Pin 11

## Code

- The code initializes the ESCs and receiver channels.
- It reads the PWM signals from the receiver and calculates the motor speeds based on the throttle, roll, pitch, and yaw values.
- The calculated motor speeds are sent to the ESCs to control the drone's movement.

## How to Use

1. Connect your motors, ESCs, and receiver to the Arduino as per the pin configuration.
2. Upload the `drone_flight_controller.ino` code to your Arduino.
3. Power on your drone and controller.
4. Adjust the minimum throttle value if necessary to keep the motors spinning.

## License

This project is licensed under the MIT License. See the `LICENSE` file for more details.

## Contributing

Contributions are welcome! Please open an issue or submit a pull request for any changes.

## Contact

For any questions or inquiries, please contact [adityakumar2128@gmail.com].
