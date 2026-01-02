# Hardware Instructions

Want to build this for yourself! You're in the right place

## Step 1: Gather the materials (BOM)

| part                         | quantity | 
|------------------------------|----------|
| Seeed XIAO RP2040            | 1        |
| MX-Style Switches            | 9        | 
| DSA Blank White Keycaps      | 9        |
| EC11 rotary encoder          | 1        |
| 1N4148 signal diodes         | 10       |
| 0.91 in 128 × 32 OLED Pannel | 1        |

## Step 2: Print the PCB

In this repo, there's a hardware directory that includes KiCAD files and gerber files.

KiCAD is there if you want to make any edits to the PCB, but if you want the same board I have, you don't need to worry about those. 

When you're ready to print, go to a PCB printer (such as JLCPCB or PCBWAY) and upload the gerbers zip. You can make the PCB as fancy as you want, but I just had the default settings. One thing that might be impactful is if you use leaded vs non-leaded solder--make sure you pick the appropriate HASL. 

### Step 3: Soldering

Now the fun part! If you're soldering on LEDs (I didn't), do that first because those are surface mounted. Everything else is through-hole though! 

If this is your first time soldering, there are plenty of online resoruces that walk through how to solder. I recommend doomscrolling youtube--those shorts are really helpful. 

I would personally solder small -> big, so that means soldering the diodes first, then the oled, then the mcu, then the encoder, and lastly the switches. At the end, it should look like this!

** Special Precaution **

Make sure you solder the diodes in teh correct direction! In the PCB, there's a silkscreen "line" on every spot you solder a diode onto -- make sure the line on teh actual diode corresponds with the silkscreen line! If it doesn't, thatll cause current to flow in the wrong direction. 



