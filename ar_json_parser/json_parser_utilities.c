/*
//  json_parser_utilities.c
//
//  Created by Adrian Russell on 26/05/2015.
//  Copyright (c) 2015 Adrian Russell. All rights reserved.
*/

#include "json_parser_utilities.h"




int children_for_token(json_parser *parser, int tokenIndex, int *children, int childLength)
{
    json_token *token = &parser->tokens[tokenIndex];
    
    int foundChildren = 0;
    
    int currentIndex = tokenIndex + 1;
    
    while (foundChildren < token->children) {
        
        json_token *t = &parser->tokens[currentIndex];
        
        if (t->parent == tokenIndex) {
            children[foundChildren] = currentIndex;
            foundChildren++;
        }
        
        currentIndex++;
    }
    
    
    return foundChildren;
}


int getValueForIndexInArray(json_parser *parser, int tokenIndex, int index)
{
    json_token *token = &parser->tokens[tokenIndex];
    
    int *children = calloc(token->children, sizeof(int));
    
    children_for_token(parser, tokenIndex, children, token->children);
    
    return children[tokenIndex];
    
    
    return 0;
}


int value_for_key_in_object(json_parser *parser, int tokenIndex, char *key)
{
    return 0;
}


int object_contains_key(json_parser *parser, int tokenIndex, char *key)
{
    unsigned int i; /* loop counter */
    
    json_token *token = &parser->tokens[tokenIndex];
    
    int *children = calloc(token->children, sizeof(int));
    
    children_for_token(parser, tokenIndex, children, token->children);
    
    for (i = 0; i < token->children; i++) {
        
        json_token *token = &parser->tokens[children[tokenIndex]];
        
        if (token->type == json_string && token->parent == tokenIndex) {
            
            // check if equals key
            
            //strncmp(<#const char *#>, <#const char *#>, <#size_t#>);
            
        }
        
    }
    
    
    return 0;
}


double value_from_numerical_token(json_parser *parser, json_token *token)
{
    return strtod(&(parser->jsonString[token->start]), NULL);
}