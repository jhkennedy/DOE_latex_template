#Obviously it is your responsibility to make sure that everything
#is, in fact, in agreement with the most current NSF Grant 
#Proposal Guide and the respective Program's solicitation! 
#This is all provided `as-is' and no blame or responsibility
#for anything that went wrong will be taken.
#
#Good luck!

.PHONY: all proposal diff

all: proposal_template 

proposal_template:
	pdflatex $@
	bibtex $@ 
	pdflatex $@
	pdflatex $@ 
	@echo; echo
	@check_repeats $@.tex
	pdftk $@.pdf cat r1 output $@_facilities.pdf
	pdftk $@.pdf cat r2 output $@_data_management.pdf
	pdftk $@.pdf cat r3 output $@_budget_justification.pdf	
	pdftk $@.pdf cat r4 output $@_current_pending.pdf
	pdftk $@.pdf cat r5 output $@_references.pdf
	pdftk $@.pdf cat 1 output $@_project_summary.pdf
	pdftk $@.pdf cat 2-r6 output $@_project_description.pdf	

diff:
	latexdiff v0.tex v1.tex > diff.tex
	pdflatex diff
	bibtex diff
	pdflatex diff
	pdflatex diff

clean: 
	rm -f *.spl *.dvi *.aux *.log *.bbl *.blg *.out
