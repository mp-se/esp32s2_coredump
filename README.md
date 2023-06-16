This is an example on how to get exception (core dump) data from a crashed ESP32S2. The easy way is to use a debugger but this might happend when you dont have a debugger attached so this is a good way to capture that error in a production environment. 

Core dump is enabled by default but there needs to be a core dump partition for it to work as in the below example. 

```
# Name,Type,SubType,Offset,Size,Flags
nvs,data,nvs,0x9000,0x5000
otadata,data,ota,0xe000,0x2000
app0,app,ota_0,0x10000,0x1c0000
app1,app,ota_1,0x1d0000,0x1c0000
spiffs,data,spiffs,0x390000,0x60000
coredump,data,coredump,0x3F0000,0x10000
```

Use the PC value to find out the adress where it crashed. 

Create a dump of the elf file to see what line of code caused the crash. This path might vary depending on what version you have installed.

```
<platformio install dir>\packages\toolchain-xtensa-esp32s2\bin\xtensa-esp32s2-elf-objdump.exe -S .\firmware.elf > dump.txt
```

On my device the crash was on adress; 0x40081746 and this points to the line of code that casused it to crash.

```
    Serial.println("Crashing....");
40081734:	fa4ab1        	l32r	a11, 4008005c <_stext+0x3c>
40081737:	fa3ba1        	l32r	a10, 40080024 <_stext+0x4>
4008173a:	000f25        	call8	4008182c <_ZN5Print7printlnEPKc>
    delay(500);
4008173d:	f4a1a2        	movi	a10, 0x1f4
40081740:	0099a5        	call8	400820dc <delay>
    *ptr = 0x10; // Cause crash
40081743:	00a082        	movi	a8, 0
40081746:	10a092        	movi	a9, 16
40081749:	004892        	s8i	a9, a8, 0
```

Happy debugging.