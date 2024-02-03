Program setup:
1. Ensure the usb has the test1.txt file and the singleGet.txt, then remove them from the project directory. They will
   reappear when GET is tested in the unitTests.sh script.
2. Ensure the project directory has the serverLocation.txt up to date, and singlePut.txt. The first so the program knows
   the location of your usb server. The second will be necessary for the sake of testing the single location PUT.
3. Run make to ensure the code is compiled.

Run the program:
1. Run ./server in a CLI tab.
2. Open a new ClI tab
3. To run unitTests.sh run ./unitTests.sh
4. If you want to manually test, you can start entering commands with ./fget.

