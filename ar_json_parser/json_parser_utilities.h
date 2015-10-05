/*
//  json_parser_utilities.h
//
//  Created by Adrian Russell on 26/05/2015.
//  Copyright (c) 2015 Adrian Russell. All rights reserved.
*/

#ifndef __ARSN__JSON_PARSER_UTILITIES_H_
#define __ARSN__JSON_PARSER_UTILITIES_H_

#include <stdlib.h> /* needed for strtod(...) in the value_from_numerical_token(...) function and calloc(...) in various methods. */
#include "json_parser.h"


/**
 *
 *
 *  @param ...
 *  @param ...
 *  @param ...
 *  @param ...
 *
 *  @return ...
 */
int children_for_token(json_parser *parser, int tokenIndex, int *children, int childLength);


int getValueForIndexInArray(json_parser *parser, int tokenIndex, int index);


/**
 *
 *
 *  @param parser     ...
 *  @param tokenIndex ...
 *  @param key        ...
 *
 *  @return
 */
int value_for_key_in_object(json_parser *parser, int tokenIndex, char *key);


int object_contains_key(json_parser *parser, int tokenIndex, char *key);



/**
 *
 *
 * @param parser ...
 * @param token  ...
 *
 * @return ...
 */
double value_from_numerical_token(json_parser *parser, json_token *token);


#endif /* defined(__ARSN__JSON_PARSER_UTILITIES_H_) */
