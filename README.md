sentperm - type sentences and get them permutated
========

[![Build Status](https://travis-ci.org/wkoszek/sentperm.svg)](https://travis-ci.org/wkoszek/sentperm)

The idea behind `sentperm` is very simple: for program verification I often
have to ask myself whether possible scenario is possible. For example:
whether certain combination of variables is possible. `sentperm` lets you
type the possible scenarios and get them permutated so that you can go
through each permutation one by one in your head and see if it's likely.

# How to build

To build `sentperm`, do:

	make

# How to use

To use `sentperm`, type:

	./sentperm

Followed by your possible cases each followed by ENTER. Once you enter all
cases, press CTRL+D.

# Example

Sample use-case:

	./sentperm
	x is negative
	y is 0
	z is 7
	^D
	!x is negative !y is 0 !z is 7
	 x is negative !y is 0 !z is 7
	!x is negative  y is 0 !z is 7
	 x is negative  y is 0 !z is 7
	!x is negative !y is 0  z is 7
	 x is negative !y is 0  z is 7
	!x is negative  y is 0  z is 7
	 x is negative  y is 0  z is 7
