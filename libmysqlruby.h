/* 
 * mapaBRASIL.net
 *
 *
 * Interpretador do Ruby dentro do mySQL
 * File:   libmysqlruby.h
 * Author: arthur
 *
 * Created on April 25, 2008, 10:56 AM
 */

#ifndef _LIBMYSQLRUBY_H
#define	_LIBMYSQLRUBY_H

#ifdef	__cplusplus
extern "C" {
#endif

my_bool irb_init(UDF_INIT *initid, UDF_ARGS *args, char *message);

void irb_deinit(UDF_INIT *initid);

char *irb(UDF_INIT * initid, UDF_ARGS * args, char *result, unsigned long *length, char *is_null, char *error);

#ifdef	__cplusplus
}
#endif

#endif	/* _LIBMYSQLRUBY_H */

