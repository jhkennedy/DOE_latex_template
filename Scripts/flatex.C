/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
 * flatex.c -
 *    Flatten a latex file into a single file, by explicitly including 
 *  the files inclued by \include and \input commands. Also, if bibtex is
 *  beeing used, then includes the .bbl file into the resulting file. Thus,
 *  creating a stand alone latex file that can be emailed to someone else. 
 *
 * Compile    : make
 * Tested on  : Linux + gcc   
 * By         : Sariel Har-Peled 
 * Email      : sariel@uiuc.edu
 * WEB Page   : http://uiuc.edu/~sariel/progs/flatex.html
 * Status     : You can do whatever you like with this program. please 
 *              email me bugs & suggestions. Its is liscenced under the 
 *              GPL.
 * Warranty   : None. 
 *
 * To do      : Add support to the includeonly command.
 *-----------------------------------------------------------------------
 * FLATEX  1.23, 1994, 1996, 1997 by Sariel Har-Peled.
 *
 * flatex - create a single latex file with no include/inputs
 *
 *       flatex [-v] [-x FileName] [files]
 *               -v      Verbose, display file structure.
 *               -x      Unflatex: extract files from archive
 *               -q      Quiet mode. Cleaner output but -x can not be used.
 *               -b      Do not insert bibiliography file(.bbl)
 *               -s      Try to handle \usepackge, but only for packags
 *                       in local directories...
 *               -i      Ignore include/input commands that the program
 *                       is unable to handle (useful if you have:
 *               -o      Overwrite existing files 
 * 
 * Flatex page: http://www.math.tau.ac.il/~sariel/flatex.html
 *-----------------------------------------------------------------------
 * History:
 *    05/04/15, 1.26
 *         Fixed a bug with fake input like \inputfun.
 *    31/12/13, 1.25
 *         Updated code so that it compiles, fixed some minor bugs.
 *    10/02/07, 1.24 
 *         Added support so that it works well with latex_ins_pckgs
 *    24/11/97, 1.23
 *         Added -i, -o flags.
 *         Fixed the bug with the extra space at end of included files.
 *         Improved (?) help message. 
 *    25/7/97, 1.22
 *         Handle input/include commands correctly when they are used
 *         without braces. Improved help message. 
 *     9/6/97, 1.21d
 *         Fixed a bug with remarks handling.
 *    25/9/96, 1.21c
 *         Added advertisement.
 *     9/9/96, 1.21b
 *         Fixed a bug that cause a loop when the .bbl file did not exist
 *    26/8/96, 1.21a
 *         Fixed bug with includegraphics command.
\*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

#include  <stdlib.h>
#include  <stdio.h>
#include  <string.h>
#include  <math.h>
#include  <ctype.h>
#include  <assert.h>

#include  "utils.h"

/*======================================================================
 * Static constants.
\*======================================================================*/
#define  DIR_LIMIT  10
#define  USE_ARGUMENT( X )   ((void)X)

static bool f_debug = false;

class  structFlags;

/*======================================================================
 * Headers of funcs
\*======================================================================*/

static void   * myMalloc( unsigned int      size );
//static void     replaceExt( char     * str, char    * ext );
static void     spacesByLevel( int     level );
static char     is_str_prefix( const char    * str, const char * prefix );
static bool     isBeforeInMem( char    * p_A, char    * p_B );
static void     addTexExt( char    * file );
static FILE      * fopenTex( const char         * _file, 
                             const char         * mode,
                             structFlags  * pFlags,
                             char         * out_name );






/*======================================================================
 * Types
\*======================================================================*/
class  structFlags  
{
public:
    char    verbose;
    bool    fBibInsert, fQuiet, fIgnoreFailInclude, fOverwrite;
    bool    fHandleStyle;

    char  * tex_inputs;
    char  * tex_dirs[ DIR_LIMIT ];
    int     tex_dirs_count;

    int     cSpecialInputLevel;
    int     countEndDocs;
    char    szFullName[ LINE_SIZE ];

    void  push_dir( char  * dir );
    void  init_tex_inputs();
    bool  tex_dirs_find_file( char  * filename, char  * out );
};


static void        flushall( void )
{
    fflush( stdout );
    fflush( stderr );
}


class flat
{
private:
    FILE         * flOut;
    char         * p_InName;
    int            level;
    structFlags  * pFlags;
    char         * command;


    //FILE      * flIn;
    char      * str, * p_Input, * p_Include, * line, * p_Rem, *inc;
    char      * p_Line, * p_Remark, * p_Bib, * p_BibStyle;
    char      * p_NewCommand, * p_Name;
    char        cont;
    char        repFlag;
    char        szInName[ 100 ];
    //    char        fInclude;
    char      * last_line;

    void        handleIncludeCommand();
    void        handle_open_failure();
    void        flatIt_inner();
    int         handle_line( FILE  * flIn );

    void        handle_single_package( char  * package );
    void        handle_packages( char  * packages );


    bool        is_usepackage_and_can_handle();
    void        handle_usepackage();

    bool        find_package( char  * package, char  * out );

public:
    void        flatIt( FILE         * _flOut,
                        char         * _p_InName,
                        int            _level,
                        structFlags  * _pFlags,
                        char         * _command );
};


void  structFlags::push_dir( char  * dir )
{
    assert( tex_dirs_count < DIR_LIMIT );
    
    tex_dirs[ tex_dirs_count ] = strdup( dir );

    assert( tex_dirs[ tex_dirs_count ] != NULL );
    tex_dirs_count++;    

    //char  * pos;

    //pos = tex_dirs[ tex_dirs_count ];
}


bool  structFlags::tex_dirs_find_file( char  * filename, char  * _out )
{
    char  test[ LINE_SIZE ];
    int  len;

    for  ( int  ind = 0; ind < tex_dirs_count; ind++ ) {
        strcpy( test, tex_dirs[ ind ] );
        len = strlen( test );
        while  ( ( len > 0 )  && (  test[ len - 1 ] == '/' ) ) {
            test[ len - 1 ] = 0;
            len--;
        }
        strcat( test, "/" );
        strcat( test, filename );
        //printf( "test: [%s]\n", test );
        if   ( isFileExists( test ) ) {
            strcpy( _out, test );
            return true;
        }
    }
    
    return  false;    
}


void  structFlags::init_tex_inputs()
{
    tex_inputs = getenv( "TEXINPUTS" );
    tex_dirs_count = 0;
    if  ( tex_inputs == NULL ) 
        return;

    if  ( verbose ) 
        printf( "TEXINPUTSS: [%s]\n", tex_inputs );

    char   * curr, * next, * pos;
    char  tmp[ LINE_SIZE ];

    curr = tex_inputs;
    next = strchr( curr, ':' );
    while  ( next != NULL ) {
        strcpy( tmp, curr );
        pos = strchr( tmp, ':' );
        assert( pos != NULL );        
        *pos = 0;
        push_dir( tmp );

        curr = next + 1;        
        next = strchr( curr, ':' );
    }
    
    push_dir( curr );    
}

    


void  flat::handle_open_failure()
{
    if  ( pFlags->fIgnoreFailInclude  &&  command != NULL ) { 
        flushall();
        fprintf( stderr,
                 "\tUnable to open file: `%s`. Ignored.\n", szInName );
        if  ( pFlags->verbose ) 
            fprintf( stderr, "\tIgnoring command: [%s]\n", command );
        
        fprintf( flOut, "%s\n", command );
        
        return;
    }

    flushall();
    fprintf( stderr, "Unable to open file: `%s`\n", szInName );
    fprintf( stderr,
             "This might be a system wide include file. If you want\n" 
             "flatex to ignore such include commands, use -i\n" );
    exit( -1 );
}


void        flat::flatIt( FILE         * _flOut,
                          char         * _p_InName,
                          int            _level,
                          structFlags  * _pFlags,
                          char         * _command )
{
    flOut = _flOut;
    p_InName = _p_InName;
    level = _level;
    pFlags = _pFlags;
    command = _command;

    flatIt_inner();
}


bool      flat::is_usepackage_and_can_handle() 
{
    char   * p_use_package, * pos;

    if  ( ! pFlags->fHandleStyle ) 
        return  false;
    
    p_use_package = strstr( line, "\\usepackage" );
    if  ( p_use_package == NULL )
        return  false;
    if  ( strchr( line, '[' ) != NULL ) {
        if ( pFlags->verbose ) {
            printf( "-- Can not handle optional params"
                     " in usepackage\n"  );
            printf( "--   [%s]\n", line );
        }
        
        return  false;
    }
    pos = strchr( line, '\\' );
    assert( pos != NULL );
    pos = strchr( pos+1 , '\\' );
    if  ( pos != NULL ) {
        if ( pFlags->verbose )
            printf( "-- usepacakge command must be on its own line\n" );
        return  false;
    }

    pos = strchr( line, '}' );
    if  ( pos == NULL ) {
        if ( pFlags->verbose )
            printf( "-- usepacakge command must be on its own line\n" );
        return  false;
    }

    return  true;     
}

bool      flat::find_package( char  * package, char   * out )
{
    char  tmp[ LINE_SIZE ];
    //char  name[ LINE_SIZE ];
    
    strcpy( tmp, package );
    strcat( tmp, ".sty" );
    *out = 0;
    if  ( pFlags->tex_dirs_find_file( tmp, out ) ) {
        flat  next_level;

        printf( "Found usepacakge: [%s]. Inserting it.\n", out );
        next_level.flatIt( flOut, out, level + 1, pFlags, NULL ); //"usepackage" );
        return  true;
    }

    return  false;
}


void      flat::handle_single_package( char  * package )
{
    char  out[ LINE_SIZE ];

    //printf( "PACKAGE: [%s]\n", package );
    if  ( ! find_package( package, out ) ) {
        fprintf( flOut, "\\usepackage{%s}\n", package );
    }
    strcpy( p_Line, p_Remark );
}


void      flat::handle_packages( char  * packages )
{
    char  tmp[ LINE_SIZE ];
    char  * pos;

    do  {
        strcpy( tmp, packages );
        pos = strchr( tmp, ',' );
        if  ( pos == NULL ) 
            break;
        *pos = 0;
        handle_single_package( tmp );
        packages += strlen( tmp ) + 1;
    }  while  ( true );

    handle_single_package( tmp );    
}

void      flat::handle_usepackage()
{
    char   in_line[ LINE_SIZE ];
    char  * start, * end;
    char   packages[ LINE_SIZE ];

    strcpy( in_line, line );
    
    start = strchr( in_line, '{' );
    if  ( start == NULL )
        return;

    end = strchr( start + 1, '}' );
    if  ( end == NULL )
        return;
    
    *end = 0;
    strcpy( packages, start + 1 );
    
    handle_packages( packages );
}


int       flat::handle_line( FILE  * flIn )
{
    bool   fInclude;
    //    char  * p_use_package;

    *p_Line = 0;
    line[ 0 ] = 0;

    if  ( feof( flIn ) )
        return -1;

    str = fgets( line, LINE_SIZE, flIn );
    if  ( str == NULL ) {
        return  -1;
    }

    last_line = NULL;
    fInclude = false;
    
    strcpy( p_Line, line );
    
    *p_Remark = 0;
    p_Rem = strchr( line, '%' );
    if  ( p_Rem != NULL ) {
        strcpy( p_Remark, p_Rem );
        *p_Rem = 0;
    }
    
    do  {
        cont = 0;
        p_Input = strstr( line,   "\\input\{" );
        //        p_use_package = strstr( line, "\\usepackage" );
        p_Bib = strstr( line, "\\bibliography" );
        p_BibStyle = strstr( line, "\\bibliographystyle" );
        
        if  ( is_usepackage_and_can_handle() ) {
            handle_usepackage();
        }
        if  ( pFlags->fBibInsert 
              &&  ( p_Bib != NULL  ||  p_BibStyle != NULL ) 
              &&  ( isFileExtExists( pFlags->szFullName, ".bbl" ) ) ) {
            p_Name = (char *)myMalloc( LINE_SIZE );
            
            strcpy( p_Name, p_Line );
            strcpy( p_Line, pFlags->fQuiet? "%" : "%FLATEX-REM:" );
            strcat( p_Line, p_Name );
            
            if  ( p_BibStyle != NULL ) {
                strcpy( p_Name, pFlags->szFullName );
                replaceExt( p_Name, ".bbl" );
                
                pFlags->cSpecialInputLevel++;
                flatIt( flOut, p_Name, level + 1, pFlags, NULL );       
                pFlags->cSpecialInputLevel--;
                
                if  ( pFlags->verbose ) {
                    printf( "\t" );
                    spacesByLevel( level + 1 );
                    printf( "(Bibiliography)\n" );
                }
            }
            break;
        }
        
        inc = line;
        do { 
            repFlag = 0;
            p_Include = strstr( inc, "\\include" );
            
            if  ( is_str_prefix( p_Include, "\\includeversion" )  
                  ||  is_str_prefix( p_Include, 
                                     "\\includegraphics"  ) ) {
                repFlag = 1;
                inc = p_Include + 1;
                continue;
            }
            
            if  ( is_str_prefix( p_Include, "\\includeonly" ) ) {
                flushall();
                fprintf( stderr, "WARNING: \"\\includeonly\" command "
                         "ignored\n" );
                inc = p_Include + 1;
                repFlag = 1;
                continue;
            }
            if  ( ( p_Include != NULL )  && ( isalpha( p_Include[ 8 ] ) ) ) {
                flushall();
                fprintf( stderr,
                         "\nWarning: include-like(?) command ignored"
                         " at line:\n\t%s", p_Line );
                inc = p_Include + 1;
                repFlag = 1;
                continue;
            }
        }  while  ( repFlag );
        
        if  ( isBeforeInMem( p_Input, p_Include ) )
            p_Include = p_Input;
        
        if  ( p_Include != NULL ) {
            p_NewCommand = strstr( line, "\\newcommand" );
            if (  p_NewCommand == NULL )  {
                handleIncludeCommand( );
                cont = 1;
                fInclude = true;
            }
        }
    } while  ( cont );
    if  ( fInclude ) {
        if  ( *p_Remark != 0 ) {
            strcat( line, p_Remark );            
        }
        last_line = line;
        fputs( line, flOut );
    } else {
        last_line = p_Line;
        fputs( p_Line, flOut );
    }   

    return  0;
}

void        flat::flatIt_inner()
{
    char  tmp_name[ LINE_SIZE ];
    FILE  * flIn;

    last_line = NULL;
    strcpy( szInName, p_InName );

    if  ( f_debug )
        printf( "Open: %s\n", p_InName );

    flIn = fopenTex( szInName, "rt", pFlags, tmp_name );
    if  ( flIn == NULL ) {
        addTexExt( szInName );
        flIn = fopenTex( szInName, "rt", pFlags, tmp_name );
    }

    if  ( f_debug )
        printf( "y\n" );

    if   ( flIn == NULL ) {
        handle_open_failure();
        return;
    }
    strcpy( szInName, tmp_name );

    if  ( ! pFlags->fQuiet ) 
      fprintf( flOut, "%%%cflatex input: [%s]\n", 
               pFlags->cSpecialInputLevel > 0? '*' : ' ', 
               szInName ); 
    if  ( pFlags->verbose ) {
        printf( "\t" );
        spacesByLevel( level );
        printf( "%s\n", szInName );
    }

    line = (char *)myMalloc( LINE_SIZE );
    p_Line = (char *)myMalloc( LINE_SIZE );
    p_Remark = (char *)myMalloc( LINE_SIZE );

    if  ( f_debug ) printf( "z\n" );

    while  ( handle_line( flIn ) >= 0 );
    //! feof( flIn ) ) {
    //        handle_line();
    //    }
    if  ( f_debug ) 
        printf( "zz\n" );
    assert( flIn != NULL );

    fclose( flIn );

    if  ( f_debug )
        printf( "zz - close\n" );

    /* we want to print \n only if we have to,.. */
    if  ( last_line != NULL ) {
        while  ( *last_line != 0  &&  last_line[ 1 ] != 0 ) {
            last_line++;
        }
    }
    if  ( f_debug )
        printf( "zzz\n" );
    if  ( last_line == NULL 
          ||  ( *last_line != '\n'  &&  *last_line != '\r' ) )
        fputs( "\n", flOut );

    if  ( ! pFlags->fQuiet ) 
      fprintf( flOut, "%% flatex input end: [%s]\n", szInName );

    if  ( f_debug )
        printf( "zzzz\n" );
    //free( line );
    //free( p_Line );
    //free( p_Remark );
}


/*======================================================================
 * Static prototypes.
\*======================================================================*/


/*======================================================================
 * Start of Code
\*======================================================================*/


static void        spacesByLevel( int     level )
{
  while  ( level > 0 ) {
    printf( "    " );
    level--;
  }
}


static void          printHelp( void )
{
    printf( "flatex - create a single latex file with no \\include/\\input"
            " commands\n" );
    printf( "\n\tflatex [-q] [-b] [-v] [-x FileName] [files]\n" );
    printf( "\t\t-h\tHelp.\n" );
    printf( "\t\t-v\tVerbose, display file structure.\n" );
    printf( "\t\t-x\tUnflatex: extract files from archive\n" );
    printf( "\t\t-q\tQuiet mode. Cleaner output but -x can not be used.\n" );
    printf( "\t\t-b\tDo not insert bibiliography file(.bbl)\n" );
    printf( "\t\t-i\tIgnore include/input commands that the program\n" );
    printf( "\t\t\tis unable to find source files\n" );
    printf( "\t\t-s\tTry to handle \\usepackge, but only for packags\n"
            "\t\t\tin local directories, as defined in TEXINPUTS\n" );
    printf( "\t\t-o\tOverwrite existing files\n" );
    printf( "\n" );
    printf( "Note A: flatex can not cause any damage to your files "
            "because it creates\n\ta new file with an .flt extension as its"
            " output. When\n\tusing the -x option, your older files"
            " are overwriten\n\tonly if you use the -o option.\n" );
    printf( "Note B: Adding superfluous \\include commands after the\n\t"
          "\\end{document} command, enables you to carry additional files\n\t"
            "in the archive. For example, \\input{bogi.bib} will add the\n\t"
            "bibtex file of the paper to the archive. Notice that the archive is\n\t"
            "still a valid latex file.\n" );
}


static void      * myMalloc( unsigned int      size )
{
    void         * ptr;

    ptr = malloc( size );
    if  ( ptr == NULL ) {
        flushall();
        fprintf( stderr, "Not enough memory" );
        exit( -1 );
    }

    return    ptr;
}


static  bool   isLatexCommand( const char  * str, const char  * cmd )
{
    int  len = strlen( cmd );
    
    if  ( strncmp( str, cmd, len ) != 0 )
        return  false;
    if  ( isalpha( str[ len ] ) )
        return  false;

    return  true;                    
}


void     flat::handleIncludeCommand()
{
    char       * p_Brace, * p_Name, * p_EndBrace;
    char         ch, fInput = 0;

    p_Brace = NULL;
    p_Name = NULL;

    if  ( isLatexCommand( p_Include, "\\input" ) ) {
        ///if  ( strncmp( p_Include, "\\input", 6 ) == 0 ) {
        p_Brace = p_Include + 6;
	fInput = 1;
    } else
        if  ( isLatexCommand( p_Include, "\\include" ) ) {
            p_Brace = p_Include + 8;
        }

    ch = *p_Include;
    *p_Include = 0;
    fputs( line, flOut );
    *p_Include = ch;

    if  ( p_Brace == NULL ) {
        fprintf( stderr,"What? Please report bug to the author."
                 "\n\nline: [%s]\n", line );
        exit( -1 );
    }
    while  ( *p_Brace  && ( p_Brace[ 1 ] == ' ' 
                              ||  p_Brace[ 1 ] == '\t' 
                              ||  p_Brace[ 1 ] == '\n' 
                              ||  p_Brace[ 1 ] == '\r' ) ) {
        ++p_Brace;
    }

    p_EndBrace = strchr( p_Brace + 1, '}' );
    if  ( *p_Brace == '{'  &&  p_EndBrace == NULL ) {
        fprintf( stderr, "ERROR: Expected brace not found.\n\n\tline:%s\n",
                 line );
        exit( -1 );
    }

    if  ( *p_Brace != '{' ) {
        p_EndBrace = strchr( p_Brace + 1, ' ' );
        if  ( p_EndBrace == NULL )
            p_EndBrace = strchr( p_Brace + 1, '\n' );
        if  ( p_EndBrace == NULL )
            p_EndBrace = strchr( p_Brace + 1, '\r' );
    }

    if  ( p_EndBrace == NULL ) {
        fprintf( stderr, "Sorry, I am very very confused. Bailing out."
                 "\n\n\tline:%s\n", line );
        exit( -1 );
    }

    ch = *p_EndBrace;
    *p_EndBrace = 0;

    if  ( strchr( p_Brace, '#' ) != NULL ) {
        flushall();
        fprintf( stderr, "\tInclude command encontered inside macro. Ignored\n" );
        if  ( pFlags->verbose ) 
            fprintf( stderr, "\tLine: [%s]\n", line );
        fputs( p_Include, flOut );
        *p_EndBrace = ch;
    } else {
        p_Name = (char *)myMalloc( LINE_SIZE );
        strcpy( p_Name, p_Brace + 1 );
        if  ( ! fInput )  
            replaceExt( p_Name, ".tex" );

        flat  next_level;

        next_level.flatIt( flOut, p_Name, level + 1, pFlags, p_Include );
        //free( p_Name );

        p_EndBrace++;
    }
    while  ( *p_EndBrace ) {
        *line++ = *p_EndBrace++;
    }
    *line = 0;
}


static bool        isBeforeInMem( char    * p_A, char    * p_B )
{
    if  ( p_B == NULL )
        return  true;
    if  ( p_A == NULL )
        return  false;

    if  ( (int)( p_A -p_B ) < 0 ) {
        return   true;
     }
    return   false;
}


static FILE      * fopenTex( const char         * _file, 
                             const char         * mode,
                             structFlags  * pFlags,
                             char         * out_name )
{
    FILE     * fl;
    char  file[ LINE_SIZE ];

    fl = NULL;

    strcpy( file, _file );

    fl = fopen( file, mode );
    if  ( ( fl != NULL )  &&  ( out_name != NULL ) )
        strcpy( out_name, file );
    if  ( fl != NULL ) 
      return fl;
    
    if ( pFlags != NULL ) {        
        for  ( int  ind = 0; ind < pFlags->tex_dirs_count; ind++ ) {
            char  new_name[ LINE_SIZE ];
            
            strcpy( new_name, pFlags->tex_dirs[ ind ] );
            strcat( new_name, file );
            if  ( f_debug )
                printf( "Trying: [%s]\n", new_name );

            fl = fopen( new_name, mode );
            if  ( fl != NULL ) {
                if  ( out_name != NULL )
                    strcpy( out_name, new_name );
                return fl;
            }
        }
    }
   
    replaceExt( file, ".tex" );
    fl = fopen( file, mode );    
    if  ( ( fl != NULL )  &&  ( out_name != NULL ) )
        strcpy( out_name, file );

    if  ( f_debug )
        printf( "x\n" );
    if  ( fl == NULL ) 
        printf( "fl == NULL\n" );
    else {
        if  ( f_debug )
            printf( "FILENAME: %s\n", file );
    }
    return  fl;
}


static char      isTexFileExists( char        * file )
{
    FILE     * fl;

    fl = fopenTex( file, "rt", NULL, NULL );
    if  ( fl != NULL ) {
      fclose( fl );
      return  1;
    }
    if  ( f_debug )
        printf( "z\n" );

    return  0;
}


static void        addTexExt( char    * file )
{
    FILE      * fl;

    fl = fopenTex( file, "rt", NULL, NULL );
    if  ( fl != NULL )
      fclose( fl );
}


static char     is_str_prefix( const char    * str, const char * prefix )
{
    int len;

    if  ( str == NULL  ||  prefix == NULL )
        return  0;

    len = strlen( prefix );
    
    return  (strncmp( str, prefix, len ) == 0);
}



static char         strCmpPrefixAndCopy( char      * line, 
                                         const char      * str,
                                         char      * outName )
{
    char       * pos;
    const char * pPreLine;
    
    pPreLine = line;
    
    pos = strstr( line, str );
    if  ( pos == NULL )
        return  0;
    
    line = pos + strlen( str );
    strcpy( outName, line );
    pos = strchr( outName, ']' );
    
    if  ( pos == NULL ) {
        fprintf( stderr, "Error encountered in line: [%s]", pPreLine );
        exit( -1 );
    }
    *pos = 0;
    
    return  1;
}


static void        writeFile( FILE     * flIn, 
                              char     * pOutName, 
                              int        level,
                              structFlags  * pFlags )
{
    FILE       * flOut;
    char       * p_Line, * pos, * start;
    char         line[ LINE_SIZE ], outName[ LINE_SIZE ];
    char         flag;
    
    outName[ 0 ] = 0;
    
    if  ( pOutName == NULL ) {
        flOut = NULL;
        printf( "Scanning for flatex archive start...\n" );
    } else {
        if  ( !pFlags->fOverwrite  &&  isFileExists( pOutName ) ) {
            fprintf( stderr, "\tFile [%s] exists. If you want to overwrite it"
                     "\n\tuse the -o flag. It is RECOMMENDED to open flatex"
                     "\n\tarchive"
                     " in a clean directory, and copy the relevant\n\tfiles"
                     " afterwards.\n", pOutName );
            fprintf( stderr, "bailing out...\n" );
            exit( -1 );
        }

        flOut = fopen( pOutName, "wt" );
        if  ( flOut == NULL ) {
            fprintf( stderr, "Unable to open file: %s\n", pOutName );
            exit( -1 );
        }
        spacesByLevel( level );
        printf( "[%s]\n", pOutName );
    }
    
    do {
        p_Line = fgets( line, LINE_SIZE, flIn );
        if  ( p_Line == NULL ) 
            break;
     
        pos = strstr( line, "\\end{document}" );
        if  ( pos != NULL ) {
            start = line;
            while  ( *start != '%'  &&  start != pos )
                start++;
            if  ( *start != '%' ) 
                pFlags->countEndDocs++;
        }
        flag = strCmpPrefixAndCopy( line, "% flatex input end: [", outName ); 
        if  ( flag ) {
            if  ( flOut == NULL ) {
                fprintf( stderr, "Something is wrong!!!!\n" );
                exit( -1 );
            }
            /*spacesByLevel( level );
              printf( "/\n" );
              printf( "Writing [%s] done\n", outName );*/
            break;
        }
        
        flag = strCmpPrefixAndCopy( line, "% flatex input: [", outName ); 
        if  ( flag ) {
            writeFile( flIn, outName, level + 1, pFlags );
            if  ( flOut != NULL ) 
                fprintf( flOut, "\\input{%s}\n", outName );
        } else {
            flag = strCmpPrefixAndCopy( line, "%*flatex input: [", outName ); 
            if  ( flag ) {
                writeFile( flIn, outName, level + 1, pFlags );
            } else {
                if  ( flOut != NULL ) {
                    if  ( strncmp( line, "%FLATEX-REM:", 12 ) == 0 ) 
                        fputs( line + 12, flOut );
                    else
                        fputs( line, flOut );
                }
            }
        }
    }  while  ( ! feof( flIn ) );
    
    if  ( flOut != NULL ) 
        fclose( flOut );  
}


static void        flatOutFile( char         * fileName, 
                                structFlags  * pFlags )
{
  FILE       * flIn;

  pFlags->countEndDocs = 0;
  
  USE_ARGUMENT( pFlags );

  flIn = fopen( fileName, "rt" );
  if  ( flIn == NULL ) {
    fprintf( stderr, "Unable to open file: %s\n", fileName );
    exit( -1 );
  }
  
  writeFile( flIn, NULL, 0, pFlags );

  fclose( flIn );

  if  ( pFlags->countEndDocs > 1 ) {
      flushall();
      fprintf( stderr, "\n\t%d \\end{document} encountered in archive\n",
               pFlags->countEndDocs );
  }
}


static void        flatFile( char         * fileName, 
                             structFlags  * pFlags )
{
    char             * szInName, * szOutName;
    int                inLen;
    FILE             * flOut;

    szInName = (char *)myMalloc( LINE_SIZE );
    szOutName = (char *)myMalloc( LINE_SIZE );

    strcpy( szInName, fileName );
    if  ( ! isTexFileExists( szInName ) ) {
      fprintf( stderr, "--Unable to open file: [%s]\n", fileName );
      exit( -1 );
    }

    inLen = strlen( szInName );
    if  ( inLen < 4  ||  ( szInName[ inLen ] != '.'  &&
            strcmp( szInName + inLen - 4, ".tex" ) != 0 ) ) {
        strcat( szInName, ".tex" );
    }

    printf( "input file: [%s]\n", szInName );

    strcpy( pFlags->szFullName, szInName );

    strcpy( szOutName, szInName );
    replaceExt( szOutName, ".flt" );

    flOut = fopen( szOutName, "wt" );
    if   ( flOut == NULL ) {
        fprintf( stderr, "Unable to open file: %s\n", szOutName );
        exit( -1 );
    }

    flat  fl;

    fl.flatIt( flOut, szInName, 0, pFlags, NULL ); 

    fclose( flOut );
    
    printf( "\n\tFile: \"%s\" generated\n", szOutName );
}


static char    isFlag( char     * str, char       ch )
{
    if  ( ( str[ 0 ] == '-'  ||  str[ 1 ] == '+' )  &&
          ( str[ 1 ] == ch  || str[ 1 ] == toupper( ch ) )
          &&  ( str[ 2 ] == 0 ) )
      return  true;

    return  false;
}


int       main( int    argc, char   * argv[] )
{
    int                   ind;
    structFlags           sFlags;

    printf( "FLATEX  1.26, 2015 by Sariel Har-Peled.\n\n" );
    if   ( argc == 1 )
        printHelp();

    memset( &sFlags, 0, sizeof( sFlags ) );
    sFlags.verbose = false;
    sFlags.fBibInsert = true;
    sFlags.fIgnoreFailInclude = false;
    sFlags.cSpecialInputLevel = 0;
    sFlags.fOverwrite = false;
    *sFlags.szFullName = 0;
    sFlags.tex_inputs = getenv( "TEXINPUTS" );
    sFlags.init_tex_inputs();

    sFlags.fQuiet = false;

    for   ( ind = 1; ind < argc; ind++ ) {
        if   ( isFlag( argv[ ind ], 'v' ) ) {
            sFlags.verbose = true;
            continue;
        }

        if  ( isFlag( argv[ ind ], 'h' ) ) {
            printHelp();
            printf( "\tFlatex page:\n"
                    "\t\thttp://www.math.tau.ac.il/~sariel/flatex.html\n\n" );
            printf( "\tPlease email bugs & suggestions to: " 
                    "sariel@math.tau.ac.il\n\n" );
            
            continue;
        }

        if   ( isFlag( argv[ ind ], 'b' ) ) {
            sFlags.fBibInsert = false;
            continue;
        }
        if   ( isFlag( argv[ ind ], 's' ) ) {
            //printf( "BOGI\n\n\n" );
            sFlags.fHandleStyle = true;
            continue;
        }
        if   ( isFlag( argv[ ind ], 'i' ) ) {
            sFlags.fIgnoreFailInclude = true;
            continue;
        }
        if   ( isFlag( argv[ ind ], 'q' ) ) {
            sFlags.fQuiet = true;
            continue;
        }
        if   ( isFlag( argv[ ind ], 'o' ) ) {
            sFlags.fOverwrite = true;
            continue;
        }

        if   ( isFlag( argv[ ind ], 'x' ) ) {
            flatOutFile( argv[ ind + 1 ], &sFlags );
            ind++;
            continue;
        }

        flatFile( argv[ ind ], &sFlags );
    }
    return   0;
}


/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
 *
 * flatex.c - End of File
\*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/
