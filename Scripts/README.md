Included scripts
================


__bibdigest.sh__: This is a `bash` script for extracting all the entries from a
BibTeX file that appear in a TeX document. 

Usage:

    ./bibdigest.sh bibfile.bib file.aux > newbib.bib
   
    file.tex can be used in place of file.aux as well.
    
    This will not take cross references into account. To get them,
    run
    
    bibtex -min-crossrefs=1 file
   
    and then run latex (twice?). This should get all used crossrefs
    into your aux file. You may want to delete the aux file after 
    that.



This tool was created by Markus Kroetzsch and is provided under the terms of the
GNU General Public License by the Free Software Foundation, version 3, and can
be found [here][bib].


__flatex__: This is a C program to flatten a LaTeX file into a single file, by
explicitly including the files included by `\include` and `\input` commands.
Also, if BibTeX is being used, then it includes the bbl file into the resulting
file. The result is therefore a stand-alone LaTeX file.

Usage:

       flatex [-v] [-x FileName] [files]
               -v      Verbose, display file structure.
               -x      Unflatex: extract files from archive
               -q      Quiet mode. Cleaner output but -x can not be used.
               -b      Do not insert bibiliography file(.bbl)


flatex was created by Sariel Har-Peled, and no license information is available
for this script, but it is available on CTAN, the comprehensive TeX archive
network, [here][flatten].

__fmtlatex.pl__: This tool formats a LaTeX document, putting it into a canonical
format suitable for finding genuine differences rather than formatting
differences [when using tools like diff]. fmtlatex formats a LaTeX document into
a standardised layout according to the following rules:

1. Each sentence is on its own line.
2. Change-of-state commands are on their own line (e.g. \begin or \[)
3. Long pieces of inline maths are on their own line
4. Long array entries are on their own line, otherwise array rows are on their own line.


fmtlatex works by going through a LaTeX document, supplied either on the command
line or via standard input, line by line and removing or adding end-of-line
characters according to the above rules.  The purpose is to produce a document
in a standardised form that can be compared with a previous version using, say,
diff so that the differences shown are due to actual differences of content and
not accidental differences of formatting.

For usage, run`perldoc fmtlatex` on the command line.

This tool was created by Andrew Stacey and is provided under the terms of the
GNU General Public License by the Free Software Foundation, version 2 or later. 


__ltxclean.pl__: This is a tool used to clean up a latex file before submission.
It will (1) remove latex comments, (2) remove `\todo{}` commands, (3) merge
files that were `\input{}`/`\included{}` into the main file, and (4) merge the
bbl file into the main file. _WARNING: (3) and (4) don't work very well in this
script and flatex should be used instead._

Usage:

```sh
ltxclean.pl main.tex > cleaned.tex
```

Where `main.tex` is the main LaTeX source file you want cleaned, and
`cleaned.tex` will be a clean copy. (If you use flatex first, as recommended,
then the command will be `ltxclean.pl main.flt > cleaned.tex`.)

This script was made freely available by Csaba Szepesvari and described [on his
blog][clean].


[bib]: http://korrekt.org/page/Note:How_to_Extract_Used_Entries_From_BibTeX_Files
[flatten]: http://www.ctan.org/pkg/flatex 
[clean]: http://readingsml.blogspot.com/2011/04/useful-latexsvn-tools-merge-clean-svn.html

