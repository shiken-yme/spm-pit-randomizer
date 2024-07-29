#pragma once

#ifdef __cplusplus
extern "C" {
#endif

void abort(void);
double strtod(char const* str, char** endptr);
unsigned long strtoul(const char *nptr, char **endptr, int base);

#ifdef __cplusplus
}
#endif