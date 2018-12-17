import numpy as np

import chumpy as ch
from chumpy import Ch

import math


def getLambertionBasisFactors():
    a0 = ch.pi
    a1 = ch.pi * Ch(2.) / 3.
    a2 = ch.pi / 4.
    #a3 = 0 Only need first 3 basis factors for 3-order SH

    return (a0, a1, a2)

def getSHBasis():
    # access returned tuple as sh_basis[0][0] ...

    l0 = Ch(1.) / (ch.sqrt(ch.pi) * 2.)
    l1 = ch.sqrt(3.) * l0
    l2 = ch.sqrt(15.) * l0

    l2m0 = ch.sqrt(5.) / (ch.sqrt(ch.pi) * 4.)
    l2m2 = ch.sqrt(15.) / (ch.sqrt(ch.pi) * 4.)


    return ((l0,  Ch(0.), Ch(0.), Ch(0.), Ch(0.)),
            (-l1, l1,    -l1,   Ch(0.), Ch(0.)),
            (l2,  -l2,   l2m0,  -l2,   l2m2))



def sh_illum(illum_params):
    # TODO: implement
    return illum_params

def sh_normals(normals):
    #TODO: implement
    return normals

def sh_illum_error(image, albedo, illum, normals, weight):
    sh_basis = getSHBasis()
    lambert_Basis = getLambertionBasisFactors()

    # I(u,v) = A(u,v) * sum_{lm}( L_lm * H_lm(n(u,v))
    # l = sh basis
    # m = m coeff at basis l
    for i in range(0,image.size):
        for l in range(0,2):
            maxM = l*2+1
            lambert_basis = lambert_Basis[l]
            for m in range(0,maxM):
                illum[i*]
                sh_lm = sh_basis[l][m]
                c =

    model = albedo.dot(sh_illum(illum)).dot(sh_normals(normals))
    residuals = weight * (image - model)

    return residuals
