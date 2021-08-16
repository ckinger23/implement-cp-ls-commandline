/* Implementing ls
 * Carter King
 * Dr. Larkins
 * CS 330 Operating Systems
 * January 22, 2019
 *
 *
 * This program allows the user to display the content within the 
 * current working directory. If given the "-l" parameter, the 
 * contents are printed out in long format where the permissions,
 * user, group, and file size are also displayed.
 *
 *
 * Sites used:
 *
 * https://linux.die.net/man/3/getcwd
 * https://stackoverflow.com/questions/2603039/warning-comparison-with-string-literals-results-in-unspecified-behaviour
 * https://stackoverflow.com/questions/17751826/print-equal-width-columns-in-c-using-printf-formatting
 * https://www.eecs.wsu.edu/~cs150/reading/printf.htm
 * https://stackoverflow.com/questions/845556/how-to-ignore-hidden-files-with-opendir-and-readdir-in-c-library
 */


#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/param.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <limits.h>
#include <pwd.h>
#include <grp.h>
#include <errno.h>

int main(int argc, char ** argv)
{
  /*Stat struct to hold info on each file for "-l"*/
  struct stat info;
  /*Buffer to hold path of curr. working directory*/
  char cwd[MAXPATHLEN]; 


  /*Check parameter count*/
  if(argc > 2){
    printf("Too many arguments");
    exit(1);
  }


  /*pathname held in cwd is pointed to by directory"*/
  char* directory = getcwd(cwd, sizeof(cwd)); 
  if(directory == NULL){
    perror("get cwd ");
    exit(1);
  }


  /*open the directory and dir points to first item in directory*/
  DIR* dir = opendir(directory); 
  if(dir == NULL){
    perror("open directory: ");
    exit(1);
  }


  /* If "-l" a parameter or not"*/
  if(argc == 2){
    /* "-l process */ 
    if(strcmp(argv[1],"-l") == 0){ 
      /* read first file from directory */
      struct dirent* file = readdir(dir);
      if(file == NULL){
        perror("file read: ");
        exit(1);
      }


      /* While files still left unread */
      while(file != NULL){
        /* Don't print current dir and parent dir */
        if(file->d_name[0] != '.'){
          /* Gather info about the read file */
          if(stat(file->d_name, &info)== -1){
            perror("getting stats: ");
            exit(1);
          }

          /*print out file permissions*/
          if(S_ISDIR(info.st_mode)){
            printf("d");
          }
          else{
            printf("-");
          }
          if(S_IRUSR & info.st_mode){
            printf("r");
          }
          else{
            printf("-");
          }
          if(S_IWUSR & info.st_mode){
            printf("w");
          }
          else{
            printf("-");
          }
          if(S_IXUSR & info.st_mode){
            printf("x");
          }
          else{
            printf("-");
          }
          if(S_IRGRP & info.st_mode){
            printf("r");
          }
          else{
            printf("-");
          }
          if(S_IWGRP & info.st_mode){
            printf("w");
          }
          else{
            printf("-");
          }
          if(S_IXGRP & info.st_mode){
            printf("x");
          }
          else{
            printf("-");
          }
          if(S_IROTH & info.st_mode){
            printf("r");
          }
          else{
            printf("-");
          }
          if(S_IWOTH & info.st_mode){
            printf("w");
          }
          else{
            printf("-");
          }
          if(S_IXOTH & info.st_mode){
            printf("x");
          }
          else{
            printf("-");
          }


          /* Get username from user id */
          struct passwd* pw = getpwuid(info.st_uid);
          char* userName = pw->pw_name;

          /* Get group name from group id*/
          struct group* grp = getgrgid(info.st_gid);
          char* groupName = grp->gr_name;


          /* Print out file statistics */
          printf("%10s %5s %5li %5s\n", userName, groupName, info.st_size, file->d_name);

          /* Read out next file in directory */
          file = readdir(dir);
        }
        else{
          file = readdir(dir);
        }
      }
    }
    /* Make sure parameter is valid */ 
    else{
      printf("wrong parameter, use -l");
      exit(1); 
    }
  }

  /* No "-l" to the call */
  else{
    int counter = 0;
    struct dirent* file;
    while((file = readdir(dir))){
      if(file->d_name[0] != '.'){
        printf("%20s", file->d_name);
        counter ++;
        if(counter % 3 == 0){
          printf("\n");
        }
      }
    }
    printf("\n");
  }    
  closedir(dir);
  return 0;
}
