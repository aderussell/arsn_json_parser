/*
 *  json_parser.c
 *
 *  Created by Adrian Russell on 10/05/2015.
 *  Copyright (c) 2015 Adrian Russell. All rights reserved.
 */

#include "json_parser.h"

/* define null, if not already defined elsewhere, possibly by inclusion of stddef.h */
#ifndef NULL
#define NULL ((void*)0)
#endif


/* -------------------- private function prototypes -------------------- */

int parseValue(json_parser *parser, int parentToken);
int parseObject(json_parser *parser, int parentToken);
int parseArray(json_parser *parser, int parentToken);
int parseNull(json_parser *parser, int parentToken);
int parseFalse(json_parser *parser, int parentToken);
int parseTrue(json_parser *parser, int parentToken);
int parseNumber(json_parser *parser, int parentToken);
int parseString(json_parser *parser, int parentToken);

json_error_code parseEscapedCharacter(json_parser *parser);

int isValidHexidecimalCharacter(char c);


int parseJsonRFC4627Base(json_parser *parser);




/* --------------------------------------------------------------------- */


void json_parser_init(json_parser *parser)
{
    parser->jsonString   = NULL;
    parser->stringLength = 0;
    parser->tokens       = NULL;
    parser->maxTokens    = 0;
    parser->pos          = 0;
    parser->nextToken    = 0;
}


json_token* json_token_alloc(json_parser *parser, json_type type, unsigned int parent)
{
    json_token *token = NULL;
    
    if (parser->nextToken >= parser->maxTokens) {
        return NULL;
    }
    
    token = &parser->tokens[parser->nextToken++];
    token->start  = parser->pos;
    token->end    = -1;
    token->children   = 0;
    token->type   = type;
    token->parent = parent;

    return token;
}

/* --------------------------------------------------------------------- */
#pragma mark -

/*
 *
 *  @param c ...
 *
 *  @return ...
 */
int isValidHexidecimalCharacter(char c)
{
    return ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f'));
}


int areSame(json_parser *parser, json_token *tokenA, json_token *tokenB)
{
    unsigned int i; /* loop counter */
    unsigned int tokenALength = tokenA->end - tokenA->start;
    unsigned int tokenBLength = tokenB->end - tokenB->start;
    
    /* check if the token are the same length. return 0 if they are not. */
    if (tokenALength != tokenBLength) {
        return 0;
    }
    
    
    for (i = 0; i < tokenALength; i++) {
        if (parser->jsonString[tokenA->start + i] != parser->jsonString[tokenB->start + i]) {
            return 0;
        }
    }
    
    return 1;
}


/* --------------------------------------------------------------------- */
#pragma mark -


json_error_code parse_json(json_parser *parser, json_standard standard)
{
    if (standard == RFC4627) {
        return parseJsonRFC4627Base(parser);
    } else {
        return parseValue(parser, NO_PARENT);
    }
}


/**
 *
 *
 *  @param parser ...
 *
 *  @return ...
 */
int parseJsonRFC4627Base(json_parser *parser)
{
    while (parser->pos < parser->stringLength || parser->jsonString[parser->pos] != '\0') {
        
        switch (parser->jsonString[parser->pos]) {
                
            case '{':
            {
                return parseObject(parser, -1);
                
                break;
            }
            case '[':
                return parseArray(parser, -1);
                
                break;
                
            case ' ':
            case '\n':
            case '\r':
            case '\t':
                /* do nothing */
                break;
                
                
            default:
                /* error - unacceptable character */
                
                return invalidCharacter;
                
                break;
        }
        
        
        parser->pos++;
    }
    
    
    return unexpectedEnd;
}







#pragma mark -

int parseValue(json_parser *parser, int parentToken)
{
    
    while (parser->pos < parser->stringLength || parser->jsonString[parser->pos] != '\0') {
        
        switch (parser->jsonString[parser->pos]) {
            case '{':
                return parseObject(parser, parentToken);
                break;
                
            case '[':
                return parseArray(parser, parentToken);
                break;
                
                
            case '\"':
                return parseString(parser, parentToken);
                break;
                
            case ' ':
            case '\n':
            case '\r':
            case '\t':
                /* do nothing */
                break;
                
            case 't':
                return parseTrue(parser, parentToken);
                break;
                
            case 'f':
                return parseFalse(parser, parentToken);
                break;
                
            case 'n':
                return parseNull(parser, parentToken);
                break;
                
                
            case '-':
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                return parseNumber(parser, parentToken);
                break;
                
                
                
            default:
                // error!!
                return invalidCharacter;
                break;
        }
        
        parser->pos++;
    }
    
    return unexpectedEnd;
}


typedef enum {
    expectStringOrEnd,
    expectString,
    expectColon,
    expectValue,
    expectEnd
} jsonObjectSection;


int parseObject(json_parser *parser, int parentToken)
{
    if (parser->jsonString[parser->pos] == '{') {
        // is object
        //printf("object started at %d", parser->pos);
        // make the token
    } else {
        // got an error by being here
    }
    
    
    unsigned int tokenIndex = parser->nextToken;
    unsigned int lastKeyIndex = -1;
    
    /* create new token for the object */
    json_token *token = json_token_alloc(parser, json_object, parentToken);
    if (token == NULL) {
        return ranOutOfMemory;
    }
    token->start = parser->pos;
    
    /* advance to character after start of object */
    parser->pos++;
    
    
    jsonObjectSection expectedSection = expectStringOrEnd;
    
    
    while (parser->pos < parser->stringLength || parser->jsonString[parser->pos] != '\0') {
        
        switch (parser->jsonString[parser->pos]) {
            case '}':
                if (expectedSection == expectEnd || expectedSection == expectStringOrEnd) {
                    
                    token->end = parser->pos;
                    
                    // object end
                    // return success
                    return success;
                    
                } else {
                    // error
                }
                break;
            
            case ':':
                if (expectedSection == expectColon) {
                    expectedSection = expectValue;
                } else {
                    // error
                }
                break;
                
            case ',':
                // onject thing here
                if (expectedSection == expectEnd) {
                    expectedSection = expectString;
                } else {
                    // error
                }
                break;
                
            case '\"':
                // parse string
                if (expectedSection == expectString || expectedSection == expectStringOrEnd) {
                    
                    // key string
                    parseString(parser, tokenIndex);
                    lastKeyIndex = parser->nextToken - 1;
                    token->children++;
                    
                    expectedSection = expectColon;
                    
                } else if (expectedSection == expectValue) {
                    // value string
                    parseString(parser, lastKeyIndex);
                    parser->tokens[lastKeyIndex].children++;
                    
                    expectedSection = expectEnd;
                    
                } else {
                    // error
                }
                
                
                break;
                
            case ' ':
            case '\n':
            case '\r':
            case '\t':
                // do nothing
                break;
                
                
            default:
                if (expectedSection == expectValue) {
                    parseValue(parser, lastKeyIndex);
                    parser->tokens[lastKeyIndex].children++;
                    expectedSection = expectEnd;
                } else {
                    // error
                }
                break;
        }
        
        parser->pos++;
    }
    
    // return error
    return unexpectedEnd;
}



typedef enum {
    jsonArrayExpectValueOrEnd,
    jsonArrayExpectEnd,
    jsonArrayExpectValue
} jsonArraySection;


int parseArray(json_parser *parser, int parentToken)
{
    unsigned int tokenIndex = parser->nextToken;
    
    if (parser->jsonString[parser->pos] != '[') {
        return invalidCharacter;
    }
    
    
    // create new token for the object
    json_token *token = json_token_alloc(parser, json_array, parentToken);
    if (token == NULL) {
        return ranOutOfMemory;
    }
    token->type = json_array;
    
    // advance to character after start of object
    parser->pos++;
    
    
    jsonArraySection expectedSection = jsonArrayExpectValueOrEnd;
    
    
    while (parser->pos < parser->stringLength || parser->jsonString[parser->pos] != '\0') {
        
        switch (parser->jsonString[parser->pos]) {
            case ']':
                if (expectedSection == jsonArrayExpectValueOrEnd || expectedSection == jsonArrayExpectEnd) {
                    //printf("array end at %d", parser->pos);
                    
                    token->end = parser->pos;
                    // array end
                    // return success
                    
                    return success;
                    
                } else {
                    // error
                    return invalidCharacter;
                }
                break;
                
            case ',':
                // onject thing here
                if (expectedSection == jsonArrayExpectEnd) {
                    expectedSection = jsonArrayExpectValue;
                } else {
                    // error
                    return invalidCharacter;
                }
                break;
                
            case ' ':
            case '\n':
            case '\r':
            case '\t':
                // do nothing
                break;
                
                
            default:
                if (expectedSection == jsonArrayExpectValue || expectedSection ==  jsonArrayExpectValueOrEnd) {
                    parseValue(parser, tokenIndex);
                    
                    token->children++;
                    
                    expectedSection = jsonArrayExpectEnd;
                    
                } else {
                    // error
                    return invalidCharacter;
                }
                break;
        }
        
        parser->pos++;
    }
    
    
    return unexpectedEnd;
}




int parseNull(json_parser *parser, int parentToken)
{
    unsigned int i; /* loop counter */
    char expectedCharacters[4] = "null";
    
    // create the token
    json_token *token = json_token_alloc(parser, json_null, parentToken);
    if (token == NULL) {
        return ranOutOfMemory;
    }
    
    token->start = parser->pos;
    
    
    for (i = 0; i < 4; i++) {
        char currentCharacter = parser->jsonString[parser->pos];
        if (currentCharacter != expectedCharacters[i]) {
            // error
            return invalidCharacter;
        }
        parser->pos++;
    }
    
    parser->pos--;
    
    token->end   = parser->pos;
    
    
    
    return success;
}

int parseTrue(json_parser *parser, int parentToken)
{
    unsigned int i; /* loop counter */
    char expectedCharacters[4] = "true";
    
    // create the token
    json_token *token = json_token_alloc(parser, json_true, parentToken);
    token->start = parser->pos;
    
    
    for (i = 0; i < 4; i++) {
        char currentCharacter = parser->jsonString[parser->pos];
        if (currentCharacter != expectedCharacters[i]) {
            // error
            return invalidCharacter;
        }
        parser->pos++;
    }
    
    parser->pos--;
    
    token->end = parser->pos;
    
    return success;
}


int parseFalse(json_parser *parser, int parentToken)
{
    unsigned int i; /* loop counter */
    char expectedCharacters[5] = "false";
    
    // create the token
    json_token *token = json_token_alloc(parser, json_false, parentToken);
    token->start = parser->pos;
    
    for (i = 0; i < 5; i++) {
        char currentCharacter = parser->jsonString[parser->pos];
        if (currentCharacter != expectedCharacters[i]) {
            // error
            return invalidCharacter;
        }
        parser->pos++;
    }
    
    parser->pos--;
    
    token->end = parser->pos;
    
    return success;
}





typedef enum {
    jsonNumberExpectNumberOrMinus,                  /* beginning of number */
    jsonNumberExpectNumber,                         /* needs number for int */
    jsonNumberExpectNumberOrDecimalOrExponentOrEnd, /* has atleast one digit */
    jsonNumberExpectDecimalPoint,                   /* expect a decimal point */
    jsonNumberExpectDecimalNumber,                  /* after decimal point */
    jsonNumberExpectDecimalNumberOrExponentOrEnd,   /* has atleast one decimal digit */
    jsonNumberExpectExponentNumberOrPlusMinus,      /* after exponent symbol */
    jsonNumberExpectExponentNumber,                 /* after exponent symbol plus/minus */
    jsonNumberExpectExponentNumberOrEnd             /* has atleast one decimal digit */
} jsonNumberSection;

/*
 number = [ minus ] int [ frac ] [ exp ]
 
 decimal-point = %x2E       ; .
 
 digit1-9 = %x31-39         ; 1-9
 
 e = %x65 / %x45            ; e E
 
 exp = e [ minus / plus ] 1*DIGIT
 
 frac = decimal-point 1*DIGIT
 
 int = zero / ( digit1-9 *DIGIT )
 
 minus = %x2D               ; -
 
 plus = %x2B                ; +
 
 zero = %x30                ; 0
 */
int parseNumber(json_parser *parser, int parentToken)
{
    // while not end of string OR a terminating character
    
    /* create the token */
    json_token *token = json_token_alloc(parser, json_number, parentToken);
    token->start = parser->pos;
    
    
    // note the index of the first character of the number being parsed
    //int firstCharacterIndex = parser->pos;
    
    jsonNumberSection expectedSection = jsonNumberExpectNumberOrMinus;
    
    
    /* while not '\"' OR '\0' OR end of length specified */
    while (parser->pos < parser->stringLength || parser->jsonString[parser->pos] != '\0') {
     
        
        
        
        switch (parser->jsonString[parser->pos]) {
            case 'e': case 'E': /* exponent */
                if (expectedSection == jsonNumberExpectExponentNumberOrPlusMinus ||
                    expectedSection == jsonNumberExpectDecimalNumberOrExponentOrEnd) {
                    expectedSection = jsonNumberExpectExponentNumberOrPlusMinus;
                } else {
                    return invalidCharacter;
                }
                break;
            case '.':       /* decimal point */
                if (expectedSection == jsonNumberExpectDecimalNumberOrExponentOrEnd ||
                    expectedSection == jsonNumberExpectDecimalPoint) {
                    expectedSection = jsonNumberExpectDecimalNumber;
                } else {
                    return invalidCharacter;
                }
                break;
            case '-':       /* must be first char OR after 'e' */
                if (expectedSection == jsonNumberExpectNumberOrMinus) {
                    expectedSection = jsonNumberExpectNumber;
                } else if (expectedSection == jsonNumberExpectExponentNumberOrPlusMinus) {
                    expectedSection = jsonNumberExpectExponentNumber;
                } else {
                    return invalidCharacter;
                }
                break;
            case '+':       /* must be after 'e' */
                if (expectedSection == jsonNumberExpectExponentNumberOrPlusMinus) {
                    expectedSection = jsonNumberExpectExponentNumber;
                } else {
                    return invalidCharacter;
                }
                break;
            case '0':
                // if first char
                    // next must be decimal
                if (expectedSection == jsonNumberExpectNumberOrMinus) {
                    expectedSection = jsonNumberExpectDecimalPoint;
                }
                
                break;
                
            case '1': case '2': case '3':
            case '4': case '5': case '6':
            case '7': case '8': case '9':
                break;
                
                
            case '\t': case '\r': case '\n': case ' ':
            case ',' : case ']' : case '}':
                // end of number
                
                // step back a character as we have moved past the number of find a non-number character.
                parser->pos--;
                
                // make object
                //json_token *token = json_token_alloc(parser, json_number, parentToken);
                token->end   = parser->pos;

                
                return success;
                
                break;
                
            default:
                // error
                return invalidCharacter;
                break;
        }
        
        parser->pos++;
        
    }
    
    return unexpectedEnd;
}


int parseString(json_parser *parser, int parentToken)
{
    json_token *token; /* the token the string will go into */
    
    
    
    // check that the first character is a double quote so the start of a string, return invalid character error if not.
    if (parser->jsonString[parser->pos] != '\"') {
        return invalidCharacter;
    }
    
    /* allocate the next token in the parser as the token to use for the string being parsed. */
    token = json_token_alloc(parser, json_string, parentToken);

    
    /* advance to character after start of string rather than the quotation mark */
    parser->pos++;
    
    
    /* while not '\"' OR '\0' OR end of length specified */
    while (parser->pos < parser->stringLength || parser->jsonString[parser->pos] != '\0') {
        
        
        if (parser->jsonString[parser->pos] == '\"') {
            
            token->end = parser->pos;
            
            return success;
            
        }
        
        // valid the escaped character
        if (parser->jsonString[parser->pos] == '\\') {
            
            json_error_code error = parseEscapedCharacter(parser);
            if (error) {
                return error;
            }
        }
        
        /* advance to the next character in the json string */
        parser->pos++;
    }
    
    /* return that the json string has ended before the string was closed */
    return unexpectedEnd;
}


json_error_code parseEscapedCharacter(json_parser *parser)
{
    unsigned int i; /* loop counter */
    
    /* check that the first character is an escaping slash. return an 'invalid character' error code if it isn't. */
    if (parser->jsonString[parser->pos] != '\\') {
        return invalidCharacter;
    }
    
    /* advance the parser position to the character after the excaping slash */
    parser->pos++;
    
    switch (parser->jsonString[parser->pos]) {
            
        // check if acceptable escapable characters are valid
        case '\"': case '\\' : case '/' : case 'b' :
        case 'f' : case 'n' : case 'r'  : case 't' :
            return success;
            break;
            
        // check if unicode is valid
        case 'u':
            // check next four characters to be hex (upper & lower)
            for (i = 0; i < 4; i++) {
                parser->pos++;
                if (!isValidHexidecimalCharacter(parser->jsonString[parser->pos])) {
                    return invalidCharacter;
                }
            }
            
            return success;
            
            break;
            
        default:
            // error
            return invalidCharacter;
            break;
    }
    
    return invalidCharacter;
}


