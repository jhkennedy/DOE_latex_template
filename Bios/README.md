Biographical sketches
=====================

DOE proposals require a short 2-page biographical sketch (read CV) for each
contributing member of the proposal. 

This directory contains:

* `biosketches.tex` -- the tex file imported into the main proposal document
which sets up how all the Bios will be imported. 

* `cv_example.tex` -- an example 2-page biographical sketch, in DOE style. This
can either be imported directly as tex (not reccomened) or, using the Makefile
(described below), a stand-alone PDF can be generated which can be imported
into the tex document. Both options are shown in `biosketches.tex`. 

* `cv_example.pdf` -- a stand-alone, PDF, version of the 2-page biographical
sketch described above.

* `stand_alone.tex` -- a tex file used by the Makefile to compile a stand alone
version of a cv.


* `Makefile` -- The makefile. 

Makefile usage:
---------------

By default, running `make` in the bio directory will create a `stand_alone.pdf`
from the `stand_alone.tex` file using `cv_example.tex`. It will then
copy `stand_alone.pdf` to `cv_example.pdf`.

To use another CV tex file, invoke make like:

`make fbio=cv_OTHER.tex`

which will create the same stand alone files, but using `cv_OTHER_.tex` (must be in
the bio directory), and will create a `cv_OTHER.pdf` file. This will also edit
`stand_alone.tex` to specify which CV to use.

Running `make clean` will remove the extra `stand_alone.*` latex files and reset
`stand_alone.tex` back to it's original state. It will _not_, however, remove
`stand_alone.pdf` or any of the cv PDFs.

Running `make distclean` differs from `make clean` only in that it will also
remove the `stand_alone.pdf` file (it still will not remove any of the cv PDFs).


