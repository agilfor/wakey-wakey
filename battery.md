# Battery Wiring Explained

The idea of the battery wiring stems from a section of the [Seeed Studio XIAO ESP32C3 docs](https://wiki.seeedstudio.com/XIAO_ESP32C3_Getting_Started/#check-the-battery-voltage). The idea is that a battery can be wired to the battery pads, with the limitation that the ESP32C3 does not natively support monitoring the battery voltage. A [post by msfujino](https://forum.seeedstudio.com/t/battery-voltage-monitor-and-ad-conversion-for-xiao-esp32c/267535) details how this can be done using the A0 pin and a voltage divider. 

The solution presented by msfujino has been implemented in the design of wakey-wakey, but it requires soldering. Normally, it would be possible to wire directly from battery -> ESP32C3, but with the resistors for the voltage dividers it becomes more complicated and we end up with battery -> PCB -> ESP32C3. This is shown in the diagram below.

[!The diagram of the battery wiring](./assets/battery_plan.png)

The dotted lines indicated wiring/soldering that cannot be incorporated into the PCB. Though it would technically be possible to solder the resistors directly onto the ESP32C3 microcontroller, I find it to be a much cleaner solution to put it on the PCB as shown above. Electrically, the two circuits are equivalent so it does not make a difference for the functionality of the design.