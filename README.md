fca-utils
=========

Formal Concept Analysis (FCA) is an algebraic approach to analyzing data ---
which, while can be more general, here is just categorical data given as a
relation between objects and attributes.
These programs are utilities for manipulating input to and output from the program
[colibri-java](https://code.google.com/p/colibri-java/), which does the FCA
computation.
To do anything useful, you'll need that program.

The input to colibri-java is in the form of "con" (context) files, and about half
of the programs here either translate to that format, or manipulate those files.
The rest of the programs operate on the output "lattice" in the form of a `dot` file
(a la [graphviz](http://www.graphviz.org)).
colibri-java will generate these when run with the argument `--output_format dot`.

The programs here either deal with `con` files or `dot` files.
Since this is a refactored set of programs that grew organically,
naming is by some logic lost in the fog of other more interesting things.
Executables that end in "con" do all, magically, manipulate con files, while those
ending in "attr" somehow use the attributes of the concepts of the `dot` files.
Most of these are filters on standard input and produce dot output for visualization,
but some do generate tabular data.

The "dep" programs implement the greedy search used in the conference paper

BJ Keller, F Eichinger, M Kretzler.
“Formal concept analysis of disease similarity”,
In the *Proceedings of the 2012 AMIA Summit on Translational Bioinformatics*,
2012, 42–51.

Because these programs require colibri-java to generate the full lattice output,
what may be practical to compute is limited by the ability of that program to
write the dot file.

#Building

To build you'll need

- [SCons](http://www.scons.org), and
- [TCLAP](http://tclap.sourceforge.net)

You may need to edit the `LIBPATH` in the SConstruct file so that TCLAP is
accessible.

#Refactoring and Testing
I'm in the process of refactoring the programs and moving from ad hoc testing
to using cxxunit.
Until the tests appear, the refactored code is untested.
