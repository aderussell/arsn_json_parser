

An ANSI C json parser.

Strict validation checking in accordance to standards. RFC4627 & RFC7158.

Performs no memory allocation or copying; everything is done in place and referenced in relation to input string.
No function pointers.

Note: The parser is implemented in a recursive manner that makes it unsuitable for embedded systems. This may or may not change in the future.


#TODO
* Create python wrapper
* Additional unit tests
* extend makefile to produce static and dynamic libraries
* add underscores to private functions
