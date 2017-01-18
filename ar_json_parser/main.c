/*
 *  main.c
 *
 *  Created by Adrian Russell on 27/05/2015.
 *  Copyright (c) 2015 Adrian Russell. All rights reserved.
 */

#include <stdio.h>
#include <string.h>
#include <time.h>
#include "json_parser_utilities.h"
#include "json_parser_file_utilities.h"




void printResults(json_parser *parser, char *filename, double time)
{
    printf("%s: size: %ld, time: %f\n", filename, parser->stringLength, time);
}




void benchmarkFile(char *filename)
{
    clock_t begin, end;
    double time_spent;
    
    json_error_code error_code;
    
    json_token *tokens = calloc(sizeof(json_token), 3000000);
    
    json_parser parser;
    json_parser_init(&parser);
    
    
    parser.tokens = tokens;
    parser.maxTokens = 3000000;
    
    
    parser_from_file(&parser, filename);
    
    
    
    
    begin = clock();
    
    /* parse the json document */
    error_code = parse_json(&parser, RFC4627);
    
    
    end = clock();
    time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    
    printResults(&parser, filename, time_spent);
    
    file_parser_free(&parser);
    
    free(tokens);
}

void benchmarkFile2(char *folder, char *filename)
{
    char fullPath[512];
    strcpy(fullPath, folder);
    strcat(fullPath, filename);
    
    benchmarkFile(fullPath);
}



int main(int argc, const char * argv[]) {
    
    char *inputDir;
    char *endSlash;
    
    inputDir = (char *)argv[0];
    
    /* find and remove last part of the directory */
    endSlash = strrchr(inputDir, '/');
    *endSlash = '\0';
    
    
    benchmarkFile2(inputDir, "/Test Data/valid_1.json");
    benchmarkFile2(inputDir, "/Test Data/valid_2.json");
    benchmarkFile2(inputDir, "/Test Data/valid_3.json");
    benchmarkFile2(inputDir, "/Test Data/valid_4.json");
    
    
    return 0;
}
