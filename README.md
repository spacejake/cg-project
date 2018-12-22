# cg-project: Normal From Shading

Normal Estimation from Shading using 2nd-order Spherical Harmonics

# Python3 Dependancies
* numpy
* opencv
* metabochumpy

Inputs needed are Raw RGB Image, albedo, and estimated normals

# Usage
usage: estimateLighting.py [-h] --image FILE --albedo FILE [--normals FILE]

If normals are not given, a flat surface with normals pointing towards camera is assumed.
