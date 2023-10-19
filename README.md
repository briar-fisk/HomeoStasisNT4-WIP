# Homeostasis Module with Distributed Symbol Networks

![GitHub](https://img.shields.io/github/license/your/repository)

This development repository hosts a homeostasis module designed to mimic biological systems, utilizing the Distributed Symbol Networks neural architecture. Please note that this is a work in progress and intended for sharing with your development team. The complete implementation will follow once the module is thoroughly designed and coded from scratch to ensure it meets the desired quality and functionality standards.

## Overview

The homeostasis module operates as a black box, functioning above the data to regulate and maintain system stability. It utilizes the Distributed Symbol Networks neural architecture, providing a framework for maintaining homeostasis in a variety of systems, with a focus on goal states and feedback mechanisms.

## Getting Started

Here's how you can set up and use the homeostasis module:

1. **Declaration**: Declare the homeostasis module object:

    ```cpp
    c_Homeostasis testerman;
    ```

2. **Register Goal States**: Register the goal states you want the module to maintain. You can register multiple goal states, but for demonstration purposes, we'll register two:

    ```cpp
    testerman.register_Goal_States(2);
    ```

3. **Name Goal States**: Give meaningful names to the goal states:

    ```cpp
    testerman.rename_Goal_State(0, "O2");
    testerman.rename_Goal_State(1, "Temperature");
    ```

4. **Set Goal State Data Types**: Define the data type for each goal state (0 for int, 1 for float):

    ```cpp
    testerman.set_Goal_State_Type(0, 1); // O2 as float
    testerman.set_Goal_State_Type(1, 1); // Temperature as float
    ```

5. **Register Feedback States**: Register the feedback states relevant to your dataset:

    ```cpp
    testerman.register_Feedback_States(2);
    ```

6. **Name Feedback States**: Assign meaningful names to the feedback states:

    ```cpp
    testerman.rename_Feedback_State(0, "Heater");
    testerman.rename_Feedback_State(1, "Oxygen_Pump");
    ```

7. **Set Feedback State Data Types**: Define the data type for each feedback state (0 for int, 1 for float):

    ```cpp
    testerman.set_Feedback_State_Type(0, 1); // Heater as float
    testerman.set_Feedback_State_Type(1, 1); // Oxygen_Pump as float
    ```

## Usage Example

Here's an example of using the module, which currently reads data into the afferent interface and calculates deltas for goal states:

```cpp
// Read data into the afferent interface and calculate deltas for goal states
testerman.Afferent_Goal_State.set_float(0, 0, float(Simboi.O2));
testerman.Afferent_Goal_State.set_float(1, 0, float(Simboi.Temp));
testerman.Afferent_Feedback_State.set_float(0, 0, float(Simboi.O2_Pump));
testerman.Afferent_Feedback_State.set_float(1, 0, float(Simboi.Heater));
testerman.update(); // Update the homeostasis module
```

## `update` Function

The `update` function within the homeostasis module performs several essential steps:

- **Calculate Delta**: Calculate delta values for goal states.
- **Prepare Networks**: Clear existing networks and prepare for new input.
- **Read Data Into Networks**: Read data into the afferent and delta interfaces.
- **Build Raw Networks**: Construct raw networks at the lowest level.
- **Read Raw Treetops Into MSC**: Read raw treetops into the MSC (Multi-Sensory-Construct) input.
- **Build MSC**: Encode the MSC trace.
- **Increment Chrono Array**: Increment the Chrono array for time tracking.
- **Read Chrono Array Into Chrono**: Reset input, read in the current Chrono, and encode the Chrono trace.
- Future development, including predictions and trace selection, will be added here.

## License

This project is licensed under public domain.

## Contributing

Briar Fisk

## Contact

For questions or suggestions, please contact Briar Fisk at briarfisk@gmail.com

---
