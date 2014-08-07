#ifndef _LIBEXAMPLECLIENT_H_
#define _LIBEXAMPLECLIENT_H_

typedef enum {
	EXAMPLE_NO_ERROR = 0,
	EXAMPLE_NEED_INIT,
	EXAMPLE_UNKNOWN_ERROR		
}example_status_code_t;

example_status_code_t example_init();
example_status_code_t example_term();
uint32_t add100(uint32_t num);
uint32_t multiply100(uint32_t num);
#endif