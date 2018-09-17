# HiFive1-2600

This project is an embedded cycle-accurate simulation of a MOS 6502 (technically, 6507)
processor. Also included is an abstraction of the Atari's RIOT (RAM I/O-Timer) chip 
and [Jay Miner's](https://en.wikipedia.org/wiki/Jay_Miner) Television Interface 
Adapter (TIA) IC. The end goal is to create an emulator capable of executing Atari 
2600 games on a modern micro-controller level device.

[Hold onto your butts ...](https://www.youtube.com/watch?v=HKK4KmDlj8U)

## Preview

![HiFive1-2600](./preview/HiFive1-2600-preview.png)

![HiFive1-2600](./preview/HiFive1-2600-Rainbow-test.jpg)

## Why?

[Just for fun.](https://www.amazon.com/Just-Fun-Story-Accidental-Revolutionary/dp/0066620732)

## No, really. Why?

Because I'm personally interested in learning about how the Atari 2600 worked, 
understanding the 6502 processor and wanted to build a project which would use 
a RISC-V micro-controller. Writing an emulator is a great way to practice programming 
and doing so in a resource constrained environment presents interesting problems.

## Hardware

This project targets the [SiFive HiFive1](https://www.sifive.com/products/hifive1/) 
development board paired with a 2.8" TFT [touchscreen from Adafruit](https://www.adafruit.com/product/1651).

## Usage

Checkout the [freedom-e-sdk](https://github.com/sifive/freedom-e-sdk) and clone 
this repository directly into the *software* directory. Proceed to compile and 
upload as with any other freedom-e-sdk program:

```
 $ make software PROGRAM=HiFive1-2600 BOARD=freedom-e300-hifive1
 $ make upload PROGRAM=HiFive1-2600 BOARD=freedom-e300-hifive1
```

## Compilation flags

Optionally, uncommment in the Makefile:

* -DEXEC_TESTS run the unit test suites which will load test programs into the 
emulator and verify the correct functioning of the simulation.

* -DMANUAL_STEP disables automatic clocking and executes a clock cycle with each 
press of the spacebar. Useful for step-by-step debugging.

* -DCOLOUR_TEST Executes a simple test where the TIA colour map is displayed on 
screen.
