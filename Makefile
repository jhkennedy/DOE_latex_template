# A latex make file by @jhkennedy
# Addapted from:
#     https://drewsilcock.co.uk/using-make-and-latexmk

MAIN=main
SOURCES=$(MAIN).tex Makefile $(shell find ./ -iname "*.tex")

LATEX=pdflatex
LATEXOPT=--shell-escape
NONSTOP=--interaction=nonstopmode

LATEXMK=latexmk
LATEXMKOPT=-pdf
CONTINUOUS=-pvc

#TODO: automatically update figure pdfs from eps files

.refresh:
	touch .refresh

$(MAIN).pdf: $(MAIN).tex .refresh $(SOURCES) 
	$(LATEXMK) $(LATEXMKOPT) \
		-pdflatex="$(LATEX) $(LATEXOPT) $(NONSTOP) %O %S" $(MAIN)

$(MAIN).flt: $(MAIN).tex .refresh $(SOURCES) 
	./Scripts/flatex $(MAIN).tex
	

.PHONY: all once force clean distclean continuous

all:$(MAIN).flt  $(MAIN).pdf 

once: $(MAIN).flt
	$(LATEXMK) $(LATEXMKOPT) -pdflatex="$(LATEX) $(LATEXOPT) %O %S" $(MAIN)

force: $(MAIN).flt
	touch .refresh
	rm $(MAIN).pdf
	$(LATEXMK) $(LATEXMKOPT) \
		-pdflatex="$(LATEX) $(LATEXOPT) %O %S" $(MAIN)

clean:
	$(LATEXMK) -c $(MAIN)
	rm -f $(MAIN).pdfsync
	rm -rf *~ *.tmp
	rm -f *.bbl *.blg *.aux *.end *.fls *.log *.out *.fdb_latexmk

distclean:
	$(LATEXMK) -C $(MAIN)
	rm -f $(MAIN).pdfsync
	rm -rf *~ *.tmp
	rm -f *.bbl *.blg *.aux *.end *.fls *.log *.out *.fdb_latexmk

# using this will continuously look for changes and update the pdf automatically. 
# It's not quite a live preview, but it's pretty dang close.
#
#NOTE: This command will work best when you setup a `.latexmkrc` file in your home 
#      directory that, at least, sets the previewer you want to use (e.g., evince, 
#      xpdf, acroread). To do that, place this line in that file:
#          $pdf_previewer="start evince %O %S"
#
#NOTE: This will capture your terminal so it works best to run make continuous 
#      in its own terminal.
#
#NOTE: This will *not* update the flattened file. Make sure to run make on its own 
#      before commiting changes.
continuous: $(MAIN).tex $(MAIN).flt .refresh $(SOURCES)
	$(LATEXMK) $(LATEXMKOPT) $(CONTINUOUS) \
		-pdflatex="$(LATEX) $(LATEXOPT) $(NONSTOP) %O %S" $(MAIN)

debug:
	$(LATEX) $(LATEXOPT) $(MAIN)

