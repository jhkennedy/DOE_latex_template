Compiling the proposal
======================

As long as you have a full TexLive distribution, you can compile this document
by typing:

```sh
make
```

At the command line. The make command will generate a `main.pdf` file from the
`main.tex` file, and a flattened (stand-alone) version of `main.tex` includes
all the imported tex (like through `\input{}` or `\include{}`) and all the
bibliography info from the bibtex generated `main.bbl` (from the `main.bib`
file). This is to allow easy submission to journals, and makes using `latexdiff`
much, much easier. 


Makefile options
----------------

__make once__: Compile the document once and stop if any errors are detected. 

__make force__: Compile the document once even if the PDF file is up to date. 

__make clean__: Remove all regenerateable files except postscript, dvi, and PDF. 

__make distclean__: Remove all regenerateable files including postscript, dvi,
and PDF. 

__make continuous__: Continuously look for changes and update the PDF
automatically. This command will work best when you setup a `.latexmkrc` file in
your home directory that, at the least, sets the previewer you want to use
(e.g., evince, xpdf, acroread). To do that, place `$pdf_previewer="start evince
%O %S"` in that file. This will also capture your terminal so it works best to
run this in its own terminal. WARNING: this will _not_ update the flattened
file -- make sure to run make on its own again before committing changes. 

Note: Makefile, and this template assumes your proposal is named `main.tex`. You
can change the make target by running

```sh
make MAIN=ALT_TARGET OPTIONS
```

where `ALT_TARGET` is the name of the alternate LaTeX file without the `.tex`
extension (e.g., to compile `alternate.tex`, you'd run `make MAIN=alternate`),
and `OPTIONS` is any of the above make options.


The DOE proposal template
=========================

This template was adapted from an NSF proposal template with a distinguished
history. 

The initial version came from [Sarah Gille at Scripps](http://www-pord.ucsd.edu/~sgille/how_to/proposal_prep.html) 

It was then [modified by Rn Grapenthin](https://github.com/rgrapenthin/nsf_latex_template) 
to fit the needs of [12/2014 NSF GPG](http://www.nsf.gov/publications/pub_summ.jsp?ods_key=gpg).

It has since been [modified by J. H. Kennedy](https://github.com/jhkennedy/DOE_latex_template) 
to create a DOE style proposal in line with XXX.


Fair warning
------------

Obviously it is your responsibility to make sure that everything
is, in fact, in agreement with the most current DOE Grant 
Proposal Guide and the respective program's solicitation! 
This is all provided __as-is__ and no blame or responsibility
for _anything_ will be taken.

Good luck!
