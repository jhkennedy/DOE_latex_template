#NSF LATEX TEMPLATE

The initial version of this Sarah Gille at Scripps: 

	(http://www-pord.ucsd.edu/~sgille/how_to/proposal_prep.html)

and I modified it to fit my needs and the [12/2014 NSF GPG](http://www.nsf.gov/publications/pub_summ.jsp?ods_key=gpg).

Use `make all` to generate proposal. This creates 1 PDF file 
and attempts to subdivide it into sections that are to be loaded
to fastlane. The page numbers will need some changes.

The make file uses pdflatex ('cause I like my figures to be 
vector and/or bitmaps), check_repeats, pdftk.

check_repeats: (http://www.grapenthin.org/toolbox/check_repeats.html)
[pdftk](https://www.pdflabs.com/tools/pdftk-the-pdf-toolkit/): install w/ homebrew or other package manager.

Obviously it is your responsibility to make sure that everything
is, in fact, in agreement with the most current NSF Grant 
Proposal Guide and the respective Program's solicitation! 
This is all provided `as-is' and no blame or responsibility
for anything that went wrong will be taken.

Good luck!
