\NeedsTeXFormat{LaTeX2e}
\ProvidesClass{DOEproposal}[2016/05/01 DOE proposal style]
\DeclareOption*{\PassOptionsToClass{\CurrentOption}{article}}
\ProcessOptions
\LoadClass[11pt,letterpaper]{article}

\RequirePackage[in]{fullpage}
\RequirePackage{mathptmx}
\RequirePackage[compact]{titlesec}
    \titlespacing{\section}{0pt}{*2}{*0.5}
    \titlespacing{\subsection}{0pt}{*0.75}{*0.5}
    \titlespacing{\subsubsection}{0pt}{*0.75}{*0.5}
\RequirePackage[sort]{natbib}
\RequirePackage{longtable}
\RequirePackage{color}

\renewcommand*{\title}[1]{\gdef\Title{#1}}
\renewcommand*{\date}[1]{\gdef\Date{#1}}
\newcommand*{\investigator}[1]{\gdef\Investigator{#1}}
\newcommand*{\institute}[1]{\gdef\Institute{#1}}
\newcommand*{\phone}[1]{\gdef\Phone{#1}}
\newcommand*{\email}[1]{\gdef\Email{#1}}

%Command to remove space from itemized lists
\newcommand{\zapspace}{\topsep=1pt\partopsep=1pt\itemsep=1pt\parskip=2pt}
