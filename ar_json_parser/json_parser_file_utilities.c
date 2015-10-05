/*
//  json_parser_file_utilities.c
//
//  Created by Adrian Russell on 02/06/2015.
//  Copyright (c) 2015 Adrian Russell. All rights reserved.
*/

#include "json_parser_file_utilities.h"

int parser_from_file(json_parser *parser, char *filename)
{
    FILE *f = NULL;
    char *file_contents;
    size_t input_file_size;
    
    f = fopen(filename, "r");
    
    if (f == NULL) {
        return EXIT_FAILURE;
    }
    
    fseek(f, 0, SEEK_END);
    
    input_file_size = ftell(f);
    
    rewind(f);
    
    file_contents = malloc(input_file_size * (sizeof(char)));
    
    if (file_contents == NULL) {
        return EXIT_FAILURE;
    }
    
    
    fread(file_contents, sizeof(char), input_file_size, f);
    
    
    
    
    parser->jsonString = file_contents;
    parser->stringLength = input_file_size;
    
    
    return fclose(f);
}


void file_parser_free(json_parser *parser)
{
    free(parser->jsonString);
    parser->jsonString = NULL;
    parser->stringLength = 0;
}