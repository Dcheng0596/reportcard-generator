# Report card Generator
 Creates a report card of all students, their courses, and each courses weighted average 
 based on the given csv files

 ## Setup 
 Simply place the csv files in the same directory as the compiled program
 Then freely change, delete, or add more entries 

 ## Command line arguments
  If no arguments are supplied then it will create the file "report-card" in the same directory
  ```
  $ reportcard
  ```
  If a file name is supplied than it will create the named file in the same directory
  ```
  $ reportcard filename
  ```
  If filepath is supplied than it will create the named file in the given directory
  ```
  $ reportcard /dir1/dir2/filename
