How to make it run on CLION:

1. open project folder / file using CLION
2. Go to edit configurations and set program arguments to 'power_quality_log', 
3. set the working directory to the project root folder (should be  (I believe) Programming_For_Engineers)
4. hit Run / OK then RUN

How to make it run on gcc (for  command line builds)

open a terminal and type
```bash
gcc main.c waveform.c io.c -o analyser -lm
./analyser power_quality_log.csv
```
for me my analyser is attempt2 so id have to type
```bash
gcc main.c waveform.c io.c -o attempt2 -lm
./attempt2 power_quality_log.csv
```
Link to the GitHub repository
https://github.com/Underordhi/Programming_For_Engineers