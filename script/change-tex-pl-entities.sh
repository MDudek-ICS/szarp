#!/bin/bash

# $Id$

# Ten skrypt zamienia polskie znaczki (podane na standardowym
# wej�ciu) entities TeX-owe. Wynik zwracany jest na standardowe 
# wyj�cie.
# Vooyeck 2002-07

	sed -e s/�/'\\Entity{aogon}'/g -e s/�/'\\Entity{Aogon}'/g  \
	-e s/�/'\\Entity{cacute}'/g -e s/�/'\\Entity{Cacute}'/g \
	-e s/�/'\\Entity{eogon}'/g -e s/�/'\\Entity{Eogon}'/g \
	-e s/�/'\\Entity{lstrok}'/g -e s/�/'\\Entity{Lstrok}'/g \
	-e s/�/'\\Entity{nacute}'/g -e s/�/'\\Entity{Nacute}'/g \
	-e s/�/'\\Entity{sacute}'/g -e s/�/'\\Entity{Sacute}'/g \
	-e s/�/'\\Entity{zacute}'/g -e s/�/'\\Entity{Zacute}'/g \
	-e s/�/'\\Entity{zdot}'/g -e s/�/'\\Entity{Zdot}'/g 

