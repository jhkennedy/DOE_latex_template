#!/usr/bin/perl
use strict;
use warnings;
use Carp;
use English;
use Symbol qw( gensym );
# follow include commands while processing latex files
# 

my $processBib = 0;
my $rootFile = $ARGV[0];
$rootFile =~ s/\.tex$//;

readFile();

sub readFile {
	my $FHANDLE = gensym();
	my $opened = 0;
	if (@_>0) {
		my $filename = shift @_;
#		print "Reading in file $filename\n";
		if (!open($FHANDLE, '<', $filename)) {
#			print "Reading in file $filename.tex\n";
			open $FHANDLE  , '<', $filename . ".tex"
  				or die  "Can't open '$filename'" . $OS_ERROR ;
#  			print "fine\n";
		}
		$opened = 1;	
	}
	
	my $prev_empty = 0;
	my $this_empty = 0;
	while ( my $line = ($opened ? <$FHANDLE> : <> ) ) {
		chomp($line);
		
		my $became_empty = ($line =~ /^\s*$/);
		# remove comments 
		$line =~ s/^(%.*)$//; # remove comment starting a line
		$line =~ s/(.*?[^\\])(%.+)$/$1/; # remove all other comments except for end of line, protect \%
		$became_empty = (!$became_empty && ($line =~ /^\s*$/));
		
		# remove \todo[xxx]{aaa}{bbb}, first and third are optional
		# this only works for single line todo commands
		# it won't work if within the todo argument you have additional
		# curly braces
		$line =~ s/\\todo\s*(\[.*?\])?\s*\{.*?\}\s*(\{.*?\})?//gi;
	
		# check for input statements, just one per line!!
		if ($line =~ /^(.*)\\(input)\s*\{(.*?)\}(.*)$/) {
			print "$1 % $3\n";
			readFile($3);
			print  (defined($4)?$4 ."\n" : "\n");
			$this_empty = 0;
		} 
		elsif ($processBib && ($line =~ /^(.*)\\(bibliography)\s*\{(.*?)\}(.*)$/)) { # bibliography
			print $1;
			readFile($rootFile . ".bbl");
			print  (defined($4)?$4 . "\n": "\n");
			$this_empty = 0;
		}
		else {
			$this_empty = ($line =~ /^\s*$/);
			if ($became_empty || ($prev_empty && $this_empty)) {
				$this_empty = $prev_empty;
			}
			else {
				print $line, "\n";				
			}
			
		}
		$prev_empty = $this_empty;
	}
}
