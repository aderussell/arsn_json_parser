/*
//  json_parser_file_utilities.h
//
//  Created by Adrian Russell on 02/06/2015.
//  Copyright (c) 2015 Adrian Russell. All rights reserved.
*/

#ifndef __ARSN__JSON_PARSER_FILE_UTILITIES_H_
#define __ARSN__JSON_PARSER_FILE_UTILITIES_H_

#include <stdio.h>
#include <stdlib.h>
#include "json_parser.h"


/**
 *
 *
 *  @param parser
 *  @param filename
 *
 *  @return
 */
int parser_from_file(json_parser *parser, char *filename);

/**
 *  Frees the parser 
 *
 *  @param parser
 */
void file_parser_free(json_parser *parser);


#endif /* defined(__ARSN__JSON_PARSER_FILE_UTILITIES_H_) */
