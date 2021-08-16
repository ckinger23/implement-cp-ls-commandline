/* Implementing cp
 * Carter King
 * Dr. Larkins
 * CS330 Operating Systems
 * January 22nd, 2019
 *
 * This program allows a user to call cp on two files, recognized
 * by their file paths. The data from the first file is copied 
 * over to the newly created second file. If the user passes a 
 * second file that already exists, the program deletes that file
 * and creates a new to paste the data into. 
 *
 *
 * Sites used:
 * https://stackoverflow.com/questions/230062/whats-the-best-way-to-check-if-a-file-exists-in-c-cross-platform
 * https://www.geeksforgeeks.org/c-program-delete-file/
 * http://codewiki.wikidot.com/c:system-calls:write
 * http://codewiki.wikidot.com/c:system-calls:read
 *
*/


#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

int main(int argc, char ** argv)
{
  /*Test if correct amount of arguments are passed*/
  if(argc != 3){
    perror("Argument error: ");
    exit(1);
  }

  /*string of path to both files*/
  char* copyPath = argv[1];  
  char* pastePath = argv[2];

  /*Open a read only copy file*/
  int copyFile = open(copyPath, O_RDONLY);
  if(copyFile == -1){
    perror("open copy: ");
    exit(1);	
  }

  /*See if path to pasteFile exists, and delete if so */
  if(access(pastePath, F_OK) == 0){ 
    if(remove(pastePath) == -1){ 
      printf("Could not delete second file");
      exit(1);
    }
  }

  /*Create and open pasteFile for reading and writing*/
  int pasteFile = open(pastePath, O_CREAT|O_RDWR, S_IRWXU);
  if(pasteFile == -1 || pasteFile < 0){
    perror("open paste: ");
    exit(1);	
  }	

  /*initialize a buffer of 1024 characters to move file content*/
  char content[1024]; 
  ssize_t dataRead = 1;
  ssize_t toBeWritten;

  /*While there is content unread from copyFile*/
  while(dataRead != 0){ 

    dataRead = read(copyFile, content, sizeof(content));
    if(dataRead == -1){
      perror("Read error: ");
    }

    /*write the content buffer to the pasteFile*/
    toBeWritten = write(pasteFile, content, dataRead);
    if(toBeWritten == -1){
      perror("write error: ");
    }
  }
  close(pasteFile);
  close(copyFile);
  return 1;
}
