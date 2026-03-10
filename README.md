# Multi-Pattern LED Controller

This project implements a multi-threaded control system on the nRF52832, designed to manage LED blink patterns and timing indicators through user-defined hardware inputs. The system leverages Zephyr RTOS primitives-specifically Event Objects, Mutexes, and Thread Management to ensure responsive control over two distinct LED patterns while maintaining a secondary background time indicator task.

---

## 🎯 Top Summary
This application demonstrates an event-driven architecture that replaces traditional polling loops with Zephyr's k_event API. By decoupling physical hardware interrupts from pattern execution, the system achieves high responsiveness and low CPU overhead. It utilizes atomic bit-masking for state transitions and thread-safe mutexes to manage shared blink-rate configurations across multiple concurrent tasks.

---

## ✨ Technologies
* **Zephyr RTOS**: Powering multithreaded execution and hardware abstraction.
* **C (Embedded)**: Core logic for event handling and GPIO peripheral interfacing.
* **nRF52832 (ARM Cortex-M4)**: The target microcontroller hardware.
* **DeviceTree (DTS)**: Used for hardware-agnostic peripheral configuration of buttons and LEDs.
* **Event-Driven Primitives**: Utilizes k_event for non-blocking task synchronization.

---

## 🚀 Features
* **Multi-Threaded Architecture**: Separates concerns into distinct threads for pattern execution, event handling, and background timing.
* **Event-Driven Logic**: Uses k_event objects to sync button interrupts (ISRs) with state-handling threads, allowing for snappy pattern switching without blocking the system. 
* **State Management**: Implements custom state-based logic to cycle through three distinct lighting patterns and three speed profiles.
* **Thread Safety**: Uses a k_mutex to protect shared variables, ensuring concurrent access to the blink rate multiplier remains stable.
* **Atomic Consumption**: Leverages k_event_wait with the clear flag for safe, atomic consumption of button events, preventing race conditions. 

---

## 📍 The Process
The goal of this project was to build a responsive, event-driven system that manages complex LED behaviors across multiple threads. By utilizing Zephyr's k_event API, I ensured the CPU remains idle until a user interaction (button press) actually occurs, significantly reducing power consumption. 

The system uses GPIO_INT_EDGE_TO_INACTIVE to detect button releases, ensuring the logic is polarity-independent. Each button press triggers an ISR that posts a bit to a k_event object, which wakes the corresponding event-handler thread to update the system state. 

---

## 🚦 Running the Project
1. **Set up the Environment**: Ensure you have the [nRF Connect SDK](https://www.nordicsemi.com/Products/Development-software/nrf-connect-sdk) and west tool installed.
2. **Clone the repository**:
   git clone https://github.com/ugonnatrunks/Single_RGB_LED.git

---

## 📸 Preview
The system provides real-time feedback through the serial console. It uses ANSI escape codes to maintain a clean, stationary display in your terminal, showing which pattern or rate is currently active.

**Live Serial Output:**
```text
Pattern 1 Executing!
Button 2 Pressed, Pattern Changed!
Button 4 Pressed, Rate Changed!
```

## 📺 Video Demo
