#!/bin/bash

######## LICENSE ########
# This is free software. Use at your own risk.
# This script is provided under the terms of the
# GNU GENERAL PUBLIC LICENSE, Version 3, 29 June 2007
# 
# http://www.gnu.org/licenses/gpl-3.0.txt
# 
# Author: Markus Kroetzsch, 2004 (<firstname>@korrekt.org)
# Version: 1.0
########

######## MANUAL ########
# A script for extracting all those entries from a bib-tex file
# that appear in some particular tex document.
# 
# Usage:
# 
# ./bibdigest.sh bibfile.bib file.aux > newbib.bib
#
# file.tex can be used in place of file.aux as well.
# 
# This will not take cross references into account. To get them,
# run
# 
# bibtex -min-crossrefs=1 file
#
# and then run latex (twice?). This should get all used crossrefs
# into your aux file. You may want to delete the aux file after 
# that.
#
# If you find a way to do this in the script below, please send
# me the code so I can update this software.
########

######## SYSTEM REQUIREMENTS ########
# This scripte requires the following suite of industry-strength
# software components to be installed on your system:
# 
#   bash, sed, grep
# 
# Please contact Apple or Microsoft for details on how to obtain
# current versions of these tools for your platform. Or use Linux.
########

if [ $# -ne 2 ]
then
  echo "A script for extracting used entries from a bibtex file."
  echo "Usage: `basename $0` bibfile.bib file.aux > newbib.bib"
  echo "Open this script in a text editor for details."
  exit 1
fi

BIB=$1
TEX=$2

SED=""

IFS=$'\t\n'
delnum=""

for line in `grep -in "@\(Article\|Book\|Booklet\|Conference\|InBook\|InCollection\|InProceedings\|Manual\|MasterThesis\|Misc\|PhdThesis\|Proceedings\|TechReport\|Unpublished\)" $BIB`; do
  num=${line%%:*}

  if [[ $delnum != "" ]]; then
    stopnum=$((num-1))
    SED="$SED-e $delnum,$stopnum""d "
  fi
  
  key=${line#*\{} 
#} < a comment to un-puzzle syntax highlighting in some versions of vi ;-)
  key=${key%,*}
  key=`echo $key|tr -d " "`
  if ( grep -q "\citation{$key}" $TEX ); then
     delnum=""
  else
     delnum=$num
  fi   
done

if [[ $delnum != "" ]]; then
  SED="$SED-e $delnum,`grep -c "$" $BIB`""d "
fi
	    
IFS=$' \t\n'
if [[ $SED != "" ]]; then
sed $SED $BIB
else
 echo "No matches found. Check whether your usage is correct."
 echo "Run this script without parameters for more information." 
fi
