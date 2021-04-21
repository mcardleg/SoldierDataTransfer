# Computer Networks Project 2
## Data transmission project of vital signs of soldiers
Computer networking college project. Transferring soldiers data over TCP connections.<br>
<br>
### Implemtation
***The folowing outlines how the project should be run and tested.*** <br>
**NOTE:** The code was developed in C on an Ubunto Linux virtal machine! <br>
1) Clone the Github repository using the Github http key.
2) Navigate to the directory in which the project was cloned to.
3) The Github should include executable files already but just incase, re-compile them as follows:
cc base.c -o base
cc router.c -o router
cc solider.c -o soldier
4) The code is easy to run, although it should be run in the following order:
./router
./base
./soldier
5) When the soldier file is run it will prompt you for a soldier ID number, this must be a 5 digit integer.
6) It is possible and recommended to run multiple soldier files to simulate different soldiers on a mission.
7) The data will be sent to the base module and automatically put into CSV files with names corresponding with their soldier ID. Maximum of six soldier CSV files can be created in the current code.
8) In order to end the stop the function from running simply type 'Q' into the base terminal (press enter) and the code will stop.
9) To graph the data type gcc pbPlots.c supportLib.c plot.c -lm
10) Then type into terminal ./a.out (soldierId).csv for example for a soldier with ID number 12345 you would type ./a.out 12345.csv
11) Check folder to see if the graphs are created

12) In order to test the timeout, while a soldier is transmitting data press ‘ctrl+C’ on the terminal that the soldier is running. After 5 seconds, the soldier will timeout and the timeout message along with the soldier ID number will be displayed on the router.c terminal.

