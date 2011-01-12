OpenSkyscraper Source Organization
==================================

This document describes how the sources for the OpenSkyscraper project are intended to be organized into their various subdirectories.

*A general note on the organization:* The sources are organized into distinct layers which reside in the top level of the source/ directory. The layers are intended to form a one-way dependency chain, which means that higher layers depend on lower ones, but not vice versa. E.g. engine depends on core, which itself depends on base. If a layer requires external stuff to be included, it does this in its `external.h` header. If a layer depends on another layer, it includes the other layer's `headers.h` file in its own `external.h`.


## classic

Contains all sources that are specific to the "Classic" version of OpenSkyscraper, i.e. the SimTower clone.


### ui

Code for drawing the user interface.

- `control` contains the Control window which shows time, rating, funds and population.
- `tools` contains the tool classes that enable manipulation of the tower's structure.
