/*
 *  json_parser.h
 *
 *  Created by Adrian Russell on 10/05/2015.
 *  Copyright (c) 2015 Adrian Russell. All rights reserved.
 */

#ifndef __ARSN_JSON_PARSER_H_
#define __ARSN_JSON_PARSER_H_


#ifdef __cplusplus
    extern "C"
    {
#endif


/**
 *  The acceptable json standards that can be used.
 */
typedef enum  {
    RFC4627 = 0,        /* RFC 4627 - only allow array or object as base value type. */
    RFC7158 = 1,        /* RFC 7158 - any type as base value. */
    EMCA404 = RFC7158   /* EMCA 404 - currently treated as RFC 7158. */
} json_standard;


/**
 *  The possible types for a token. 
 *  It also includes an unknown type which is not a valid token but is used until the token type is assigned.
 */
typedef enum {
    json_type_unknown,  /* The type of the token is currently undefined. Note that 'unknown' is used to prevent confusion with the javascript type undefined which is not represented in json. */
    json_null,          /* The token is of type null. */
    json_true,          /* The token is a boolean type and has the value true. */
    json_false,         /* The token is a boolean type and has the value false. */
    json_number,        /* The token contains a number. */
    json_string,        /* The token contains a string. */
    json_object,        /* The token represents an object. */
    json_array          /* The token represents an an array. */
} json_type;


/**
 *  The possible error codes that can be returned by the json parser.
 */
typedef enum {
    success          = 0,   /* no error was encountered */
    invalidCharacter = -1,  /* an unexpected character was encountered */
    unexpectedEnd    = -2,  /* the json string ended without the json structure being closed */
    ranOutOfMemory   = -3   /* ran out of json_tokens in the parser */
} json_error_code;

#define NO_PARENT -1    /* The parent index for a token that has no parent */


/**
 *  The token; a single element in the json stucture
 */
typedef struct {
    json_type type; /* The type of the token. */
    int index;      /* The index of the token in the parser's array of tokens.*/
    int start;      /* start pos of token */
    int end;        /* end point of token */
    int children;   /* number of child elements  */
    int parent;     /* index of the parent token */
} json_token;


/**
 *  The parser object; the structure that contains all the json string and tokens that is passed to the parser.
 */
typedef struct {
    char *jsonString;       /* The json string being parsed. */
    long  stringLength;     /* The length of the string. */
    json_token *tokens;     /* An array of the tokens that have been assigned to the parser. These will be used for the results tokens. */
    unsigned int maxTokens; /* The number of tokens available. */
    unsigned int pos;       /* The current offset in the json string. This is used while parsing. */
    unsigned int nextToken; /* The index of the next token to allocate. */
} json_parser;





/**
 *  Initializes a specified parser strucutre ready for use.
 *
 *  @param parser The parser structure to initialize.
 */
void json_parser_init(json_parser *parser);


/**
 *
 *
 * @param parser   The parser structure to parse.
 * @param standard The json standard to use for determining if the json being parsed is valid.
 *
 * @return
 */
json_error_code parse_json(json_parser *parser, json_standard standard);

        
#ifdef __cplusplus
    } /* extern "C" */
#endif

#endif /* defined(__ARSN_JSON_PARSER_H_) */
