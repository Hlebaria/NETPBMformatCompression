#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int red;
  int green;
  int blue;
}Pixel;

int factorOf(int value, int factor){
  int result = 1;

  while(factor > 0){
    result = result*value;
    factor--;
  }
  return result;
}

int convertTextValueToNumber(char* text){
  int number = 0;
  int textSize = 0;
  //get the size of the text
  while(text[textSize] != '\0'){
    //while at it, do a value check
    if(text[textSize] > '9' || text[textSize] < '0'){
      printf("Invalid number value!\n");
      return 0;
    }
    textSize++;
  }
  //prepare the textSize value to be used as a factor of 10
  textSize--;
  for(int i = 0; textSize >= 0; textSize--, i++){
    //add up the numbers one by one, using the facotr of 10 to know the position of the digits in the number
      number += (text[i]-'0')*factorOf(10, textSize);
  }

  return number;
}

char* convertNumberToTextValue(int number){
  //corner case
  if(number == 0)return "0";
  
  char* result;
  char text[10];
  int size = 0;
  result = calloc(10, sizeof(char));
  //convert the digits to symbols
  for(; number > 0; size++){
    text[size] = '0' + number%10;
    number-=number%10;
    number/=10;
  }

  //get the size to fit the indexes
  size--;
  //reverse them to be in the correct order
  for(int i = 0; size >= 0; size--, i++){
    result[i] = text[size];
  }
  return result;
}

int main()
{
  FILE *inputImage;
  FILE *newFile;
  
	inputImage = fopen("inputImages/sample.ppm", "r");
  newFile = fopen("Compressed/compFile.ppm", "w");

  char info[200];

  Pixel current;
  Pixel next;
  int repetitions = 1;

  printf("\nCompressing . . . ");
  
	if (inputImage != NULL)
  {
    //check for file type
    fscanf(inputImage, "%s", info);
    if(strcmp(info, "P3") != 0){
      printf("Not compatable format!\n");
      return 0;
    }
    fprintf(newFile, "%s ", info);

    //get the width
    fscanf(inputImage, "%s", info);
    if(convertTextValueToNumber(info) < 1){
      printf("The image has 0 width!\n");
      return 0;
    }
    fprintf(newFile, "%s ", info);
    
    //get the height
    fscanf(inputImage, "%s", info);
    if(convertTextValueToNumber(info) < 1){
      printf("The image has 0 height!\n");
      return 0;
    }
    fprintf(newFile, "%s ", info);
    
    //get max pixel value
    fscanf(inputImage, "%s", info);
    if(convertTextValueToNumber(info) < 1){
      printf("Invalid max pixel value!\n");
      return 0;
    }
    fprintf(newFile, "%s ", info);

    //begin the actual compression--------------------------------------------------
    
    //get the values of the first pixel to begin the process
    fscanf(inputImage, "%s", info);
    current.red = convertTextValueToNumber(info);
    fscanf(inputImage, "%s", info);
    current.green = convertTextValueToNumber(info);
    fscanf(inputImage, "%s", info);
    current.blue = convertTextValueToNumber(info);
    
    while(1){
    
      //get the values of the next pixel, if there is one
      if(fscanf(inputImage, "%s", info) == EOF){
        //if we've reached the final pixel, print it and exit
        if(repetitions > 1)fprintf(newFile, "/%s ", convertNumberToTextValue(repetitions));
        fprintf(newFile, "%s ", convertNumberToTextValue(current.red));
        fprintf(newFile, "%s ", convertNumberToTextValue(current.green));
        fprintf(newFile, "%s ", convertNumberToTextValue(current.blue));
        break;
      }
      next.red = convertTextValueToNumber(info);
      fscanf(inputImage, "%s", info);
      next.green = convertTextValueToNumber(info);
      fscanf(inputImage, "%s", info);
      next.blue = convertTextValueToNumber(info);

      //compare the two adjacent pixel
      if(current.red == next.red && current.green == next.green && current.blue == next.blue){
        repetitions++;
      }
      else{
        if(repetitions == 1){
          //if we can't compress anything, just lay out the pixel values
          fprintf(newFile, "%s ", convertNumberToTextValue(current.red));
          fprintf(newFile, "%s ", convertNumberToTextValue(current.green));
          fprintf(newFile, "%s ", convertNumberToTextValue(current.blue));
          current.red = next.red;
          current.green = next.green;
          current.blue = next.blue;
        }
        else{
          //if we can compress the pixels, plase the special symbol, the number of repetitions and the pixel value
          fprintf(newFile, "/%s ", convertNumberToTextValue(repetitions));
          fprintf(newFile, "%s ", convertNumberToTextValue(current.red));
          fprintf(newFile, "%s ", convertNumberToTextValue(current.green));
          fprintf(newFile, "%s ", convertNumberToTextValue(current.blue));
          current.red = next.red;
          current.green = next.green;
          current.blue = next.blue;
          repetitions = 1;
        }
      }
    }
    fclose(inputImage);
    fclose(newFile);
  }

  printf("\nCompression complete!");

  //get input on wether to continue or not
  do{
    printf("\nDo you wish to decompress? (y/n): ");
    scanf("%s", info);

    if(strcmp(info, "y") != 0 && strcmp(info, "n") != 0){
      printf("Invalid input!\n");
    }
    else{
      break;
    }
  }while(1);

  if(strcmp(info, "y") == 0){
    
    //begin decompression -----------------------------------------------------

    repetitions = 1;

    inputImage = fopen("Compressed/compFile.ppm", "r");
    newFile = fopen("Decompressed/decompFile.ppm", "w");
    
    if (inputImage != NULL){
      
      //get the file type
      fscanf(inputImage, "%s", info);
      if(strcmp(info, "P3") != 0){
        printf("Not compatable format!\n");
        return 0;
      }
      fprintf(newFile, "%s ", info);
  
      //get the width
      fscanf(inputImage, "%s", info);
      if(convertTextValueToNumber(info) < 1){
        printf("The image has 0 width!\n");
        return 0;
      }
      fprintf(newFile, "%s ", info);
      
      //get the height
      fscanf(inputImage, "%s", info);
      if(convertTextValueToNumber(info) < 1){
        printf("The image has 0 height!\n");
        return 0;
      }
      fprintf(newFile, "%s ", info);
      
      //get max pixel value
      fscanf(inputImage, "%s", info);
      if(convertTextValueToNumber(info) < 1){
        printf("Invalid max pixel value!\n");
        return 0;
      }
      fprintf(newFile, "%s ", info);
  
      //begin the actual decompression--------------------------------------------------
      while(1){
        
        //get the first part of info about the pixlel, and when the file is done decompressing, stop
        if(fscanf(inputImage, "%s", info) == EOF)break;
    
        //if we encounter the special symbol, get the info of how much repetitions of the same pixel there are
        if(info[0] == '/'){
    
          //remove the '/' in the string to leave the number only
          for(int i = 0; info[i] != '\0'; i++){
            info[i] = info[i+1];
          }
    
          repetitions = convertTextValueToNumber(info);
    
          //get the "red" value of the pixel and continue normally
          fscanf(inputImage, "%s", info);
        }
  
        current.red = convertTextValueToNumber(info);
        fscanf(inputImage, "%s", info);
        current.green = convertTextValueToNumber(info);
        fscanf(inputImage, "%s", info);
        current.blue = convertTextValueToNumber(info);
        
        do{
          
          //R
          fprintf(newFile, "%s ", convertNumberToTextValue(current.red));
          //G
          fprintf(newFile, "%s ", convertNumberToTextValue(current.green));
          //B
          fprintf(newFile, "%s ", convertNumberToTextValue(current.blue));
  
          repetitions--;
        }while(repetitions > 0);
        
        repetitions = 1;
      }
      fclose(inputImage);
      fclose(newFile);
    }
  }
}