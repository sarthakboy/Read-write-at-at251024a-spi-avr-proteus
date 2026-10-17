# 🔌 AT25F1024 SPI Flash Memory — Read & Write using AVR

<p align="center">
  <strong>Learning SPI communication by reading, writing, and erasing Flash memory!</strong>
</p>


## 📌 About the Project

This project demonstrates how to communicate with an **AT25F1024 SPI Flash Memory chip** using an AVR microcontroller and Embedded C.

I built functions to write data to Flash memory, read it back, erase a sector, and read the device identification bytes. Four push buttons control these operations, and a two-digit seven-segment display shows the data.

The most exciting part was understanding how a microcontroller sends commands and address bytes to a memory chip through SPI and receives data in return.

### Simulation Video
▶ **Watch the full demo:** https://drive.google.com/file/d/11O64dRb0t3HLXai_5yY_cMEQ0o1kfDES/view?usp=drive_link

## ✨ Features

- 📥 **Write data:** Write a byte of data to a selected Flash memory address.
- 📤 **Read data:** Read stored data from Flash memory.
- 🧹 **Erase sector:** Erase a sector containing the selected address.
- 🆔 **Read Flash ID:** Read three identification bytes from the memory chip.
- 🔘 **Button control:** Use four push buttons to select different operations.
- 🔢 **Data display:** Show read data on a two-digit, common-cathode seven-segment display.
- ⚙️ **Register-level SPI:** Configure and communicate through AVR SPI registers.
- ⏳ **Busy-status checking:** Check the Flash status register until the device finishes its operation.

## 🛠️ Hardware and Software

| Component | Purpose |
|---|---|
| AVR microcontroller | Runs the firmware |
| AT25F1024 SPI Flash memory | Stores data |
| Four push buttons | Control read, write, erase, and ID operations |
| Two-digit seven-segment display | Displays data |
| Proteus | Circuit simulation |
| AVR C/C++ toolchain | Builds the firmware |

**Clock frequency:** 16 MHz (`F_CPU = 16000000UL`)

## 🔌 How the Buttons Work

| Button | Operation |
|---|---|
| K1 | Write the value `70` to address `0x00000` |
| K2 | Read data from address `0x00000` and display it |
| K3 | Erase the sector containing address `0x00000` |
| K4 | Read and display the Flash identification bytes |

## 🧠 How SPI Flash Communication Works

The firmware uses the AVR's SPI peripheral in master mode. The microcontroller controls the Flash chip through its chip-select (CS) signal and sends commands, addresses, and data.

### Main operations

| Operation | Command | Purpose |
|---|---|---|
| Write Enable | `0x06` | Allows a write or erase operation |
| Read Status Register | `0x05` | Checks the device's busy status |
| Read Data | `0x03` | Reads data from a memory address |
| Page Program | `0x02` | Programs data into Flash memory |
| Sector Erase | `0x52` | Requests a sector erase |
| Read Identification | `0x15` | Reads identification bytes |

The firmware sends a **three-byte address** before reading, programming, or erasing data.

### Example: Writing a byte

1. Send the Write Enable command.
2. Pull CS low to start the program command.
3. Send the Page Program command.
4. Send the three address bytes.
5. Send the data byte (`70` in this project).
6. Pull CS high to end the command.
7. Poll the status register until the device is no longer busy.

### Example: Reading a byte

1. Pull CS low.
2. Send the Read Data command.
3. Send the three address bytes.
4. Send a dummy byte to generate SPI clock pulses and receive the stored data.
5. Pull CS high.
6. Display the received value.

## 💻 Embedded Concepts I Learned

This project helped me understand:

- **SPI communication:** How a microcontroller and memory chip exchange data.
- **Flash memory functionality:** Read, program, erase, status checking, and device identification.
- **Command and address transfer:** Sending command bytes followed by a three-byte memory address.
- **AVR SPI registers:** Configuring `SPCR`, `SPSR`, and `SPDR`.
- **Chip-select control:** Using macros to select and deselect the Flash chip.
- **Reusable C macros:** Creating simple macros such as `SPI_EN()`, `SPI_DI()`, and `K1_DOWN()` to make hardware control code easier to read.
- **Bitwise operations:** Setting, clearing, and checking individual register bits.
- **Seven-segment display:** Converting a numeric value into segment patterns.
- **Busy polling:** Waiting for the Flash memory operation to complete before continuing.

One of my favourite parts was learning to use macros for hardware actions, including button-state checks. It helped me see how small reusable pieces of code can make embedded programs easier to understand.

## 🚀 How to Run

1. Clone or download this repository.
2. Open the source file in an AVR-compatible development environment.
3. Select the same AVR microcontroller used in your Proteus circuit.
4. Set the clock frequency to 16 MHz.
5. Compile the firmware and generate the HEX file.
6. Load the HEX file into the microcontroller in Proteus.
7. Run the simulation and use the buttons to test the memory operations.

## 🔭 Future Improvements

- Display all three Flash identification bytes clearly.
- Add proper button debouncing.
- Add support for writing different data values and addresses.
- Verify written data automatically by reading it back.
- Add error handling for unexpected device IDs or failed operations.
- Improve the display to show larger values and hexadecimal data.

## 🎯 Project Takeaway

This project strengthened my understanding of **Embedded C, AVR microcontrollers, SPI communication, Flash memory, and register-level programming**.

It was a great learning experience to move beyond basic peripheral control and communicate with an external memory device using its commands and datasheet.

I look forward to applying these concepts in more embedded firmware projects!

---

<p align="center">
  <strong>Learn by building. Improve by debugging. Grow through every project. 🚀</strong>
</p>
