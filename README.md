# Computer Networks Project 2
## Data transmission project of vital signs of soldiers
Computer networking college project. Transferring soldiers data over TCP connections.<br>
<br>
### Implemtation
***The folowing outlines how the project should be run and tested.*** <br>
**NOTE:** The code was developed in C on an Ubunto Linux virtal machine! <br>
1). Clone the github repository using the github http key. <br>
2). Navigate to the directory in which the project was cloned to. <br>
3). The github should include executable files alread but just encase, re-compile them as follows: <br>
>cc base.c -o base<br>
cc router.c -o router<br>
cc solider.c -o soldier<br>

4). The code is easy to run, although it should be run in the following order: <br>
>./rouer<br>
>./base<br>
>./soldier<br>

5). When the soldier file is run it will prompt you for a soldier ID number, this must be a 5 digitl integer.<br>
6). It is possible and recommended to run multiple solider files to simulate different soliders on a battlefield. <br>
7). The data will be sent to the base module and automatically put into CSV files respectively named in order to their soldier numbers. <br>
8). In order to end the stop the function from running simply type 'Q' into the base termainal (press enter) and the code will be stop. <br> 
