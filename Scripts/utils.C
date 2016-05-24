/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
 * utils.C -
 *     
\*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

#include  <stdlib.h>
#include  <stdio.h>
#include  <assert.h>
#include  <string.h>

#include  "utils.h"

/*--- Constants ---*/


/*--- Start of Code ---*/

bool      isFileExists( const char     * file )
{
    FILE     * fl;

    fl = fopen( file, "rt" );
    if  ( fl != NULL ) {
      fclose( fl );
      return  true;
    }

    return  false;
}


bool    isFileExtExists( const char      * oldName,
                         const char      * ext )
{
    char   * p_Name;
    bool  val;

    p_Name = (char *)malloc( LINE_SIZE );
    assert( p_Name != NULL );

    strcpy( p_Name, oldName );
    replaceExt( p_Name, ext );
    
    val = isFileExists( p_Name );
    
    //free( p_Name );

    return val;
}



void        replaceExt( char     * str, const char    * ext )
{
    int        len, ind;

    len = strlen( str );
    ind = len - 1;
    while  ( ind >= 0  &&  str[ ind ] != '.'  &&  str[ ind ] != '\\' &&
             str[ ind ] != '/' )
        ind--;

    if  ( ind >= 0  &&  str[ ind ] == '.' ) {
        str[ ind ] = 0;
    }

    strcat( str, ext );
}

/* utils.C - End of File ------------------------------------------*/
