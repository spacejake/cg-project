import numpy as np
import chumpy as ch
from chumpy import Ch


def getLambertionBasisFactors():
    # Lambertian Basis Factors for SH
    # a_l, where l is the SH basis
    a0 = ch.pi
    a1 = ch.pi * Ch(2.) / ch.sqrt(3.)
    a2 = ch.pi * Ch(2.) / ch.sqrt(8.)
    #a3 = 0 Only need first 3 basis factors for 3-order SH

    return (a0, a1, a2)


def getSHNormalBasis():
    # Surface Normal Basis Factors for SH
    # c_l, where l is the SH basis
    c0 = Ch(1.) / ch.sqrt(ch.pi * ch.Ch(4.))
    c1 = ch.sqrt(3.) / ch.sqrt(ch.pi * ch.Ch(4.))
    c2 = (Ch(3.) * ch.sqrt(5.)) / ch.sqrt(ch.pi * ch.Ch(12.))

    return (c0, c1, c2)

def computeSHEnergy(l, normals):
    a = getLambertionBasisFactors()
    c = getSHNormalBasis()

    nx = normals[:, :, 0].reshape(-1)
    ny = normals[:, :, 1].reshape(-1)
    nz = normals[:, :, 2].reshape(-1)

    R0 = a[0] * l[0] * c[0]

    R1 = a[1] * l[1] * c[1] * nx \
         + a[1] * l[2] * c[1] * ny \
         + a[1] * l[3] * c[1] * nz

    R2 = a[2] * l[4] * c[2] * nx * ny \
         + a[2] * l[5] * c[2] * nx * nz \
         + a[2] * l[6] * c[2] * ny * nz \
         + a[2] * l[7] * (c[2] / Ch(2.)) * (nx ** 2. - ny ** 2.) \
         + a[2] * l[8] * (c[2] / (Ch(2.) * ch.sqrt(3.))) * (Ch(3.) * nz ** 2. - 1.)

    R = R0 + R1 + R2

    return R


def illum_error(image, albedo, illum, normals, weight):

    # I(u,v) = A(u,v) * sum_{lm}( a_l * l_lm * Y_lm(n(u,v))
    # A(u,v) : Albedo (surface reflectance color) at image coordiange
    # a_l : Lambertian SH Basis coeff
    # l_lm : lighting parameter of m coeff at SH basis l
    # Y_lm : diffuse surface energy using Normal of m coeff at SH basis l
    # n(u,v) : normal at image coordinate
    # l : SH basis
    # m : m coeff at SH basis l

    R = computeSHEnergy(illum, normals)

    model = albedo.reshape(-1) * R

    I = image.reshape(-1)

    residuals = weight * (I - model)

    return residuals
