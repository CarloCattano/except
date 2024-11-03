## Except 

Learning about creating pd externals using C and the pd API.
This is a simple external that filters out a selected list of numbers from the input list.


### Description
filters a selected list of numbers from the input list and returns it out to the outlet.

The right outlet outputs the numbers that are not in the selected list.

The left inlet is used to set the selected list of numbers to be filtered out.


### Support
This external has been only tested on linux at the moment and it still lacks
of other architectures support.
