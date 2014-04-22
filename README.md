fca-utils
=========

Formal Concept Analysis (FCA) is an algebraic approach to analyzing data;
data which, while it can be more general, here is categorical data given as a
relation between objects and attributes.
The programs in this repo are utilities for manipulating input to and output from the program
[colibri-java](https://code.google.com/p/colibri-java/), which does the FCA
computation.
To do anything useful, you'll need that program.

The input to colibri-java is in the form of "con" (context) files, and about half
of the programs here either translate to that format, or manipulate those files.
The rest of the programs operate on the output "lattice" in the form of a `dot` file
(a la [graphviz](http://www.graphviz.org)).
colibri-java will generate these when run with the argument `--output_format dot`.
This output is in the form of a directed graph with the concepts as nodes and
coverings as arcs with no annotation (node positions, colorings, etc.).

The programs here either deal with `con` files or `dot` files.
Since this is a refactored set of programs that grew organically,
naming is by some logic lost in the fog of other more interesting things.
I did somehow manage to have executables that end in "con" be those that
manipulate con files, while those ending in "attr" somehow use the attributes of
the concepts of the `dot` files.
The lattice programs are mostly filters on standard input and produce dot output
for visualization, but some do generate tabular data.

The "dep" programs implement the greedy search used in the conference paper

BJ Keller, F Eichinger, M Kretzler.
“Formal concept analysis of disease similarity”,
In the *Proceedings of the 2012 AMIA Summit on Translational Bioinformatics*,
2012, 42–51. [Pubmed: [22779049](http://www.ncbi.nlm.nih.gov/pubmed/22779049)]

#Building

To build you'll need

- [SCons](http://www.scons.org) for building,
- [TCLAP](http://tclap.sourceforge.net) for command line parsing, and
- [cxxtest](http://cxxtest.com)

then edit the `LIBPATH` in the src/SConscript file to indicate where TCLAP is
located.
The following shenanagans should get you to a working state

    mkdir -p site_scons/site_tools
    ln -s /path/to/cxxtest.py site_scons/site_tools/cxxtest.py
    mkdir extern_libs
    ln -s /path/to/cxxtestdir extern_libs/cxxtest

Alternatively, you can install the cxxtest directory in `extern_libs` and change
its name.
Otherwise, you may have to adjust `test/SConscript`.

It should build all of the programs with a simple command line

    scons

For testing, use

    scons check

should build the tests and run them.

#Limitations

Notes on programs manipulating dot files:

- The code to parse the dot output of colibri-java is naive and does not fully
handle the dot grammar.
So, I'm not sure what will happen when the dot goes beyond this format.
If you give dot input with annotations, at best, the annotations will be ignored
 and will just not be in the output.
- Because these programs require colibri-java to generate the full lattice output,
what may be practical to compute is limited by the ability of that program to
write the dot file.
