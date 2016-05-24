/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
 * utils.h -
 *     
\*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

#ifndef  __UTILS__H
#define  __UTILS__H


#define  LINE_SIZE 1000


void        replaceExt( char     * str, const char    * ext );
bool        isFileExists( const char     * file );
bool        isFileExtExists( const char  * oldName, const char  * ext );


#else   /* __UTILS__H */
#error  Header file utils.h included twice
#endif  /* __UTILS__H */

/* utils.h - End of File ------------------------------------------*/
