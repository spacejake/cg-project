import numpy as np
import chumpy as ch
from chumpy import Ch
import math


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


def SHFromNormal(normal, c):
    x = normal[0]
    y = normal[1]
    z = normal[2]

    # SH 0-Basis
    y0 = c[0]

    # SH 1-Basis
    y1 = c[1] * x
    y2 = c[1] * y
    y3 = c[1] * z

    # SH 2-Basis
    y4 = c[2] * x * y
    y5 = c[2] * x * z
    y6 = c[2] * y * z
    y7 = (c[2] / Ch(2.)) * (x**2. - y**2.)
    y8 = (c[2] / (Ch(2.) * ch.sqrt(3.))) * (Ch(3.) * z**2. - 1.)


    return (y0,
            y1, y2, y3,
            y4, y5, y6, y7, y8)


def computeSHEnergy(l, a, Y):

    # SH 0-Basis
    r0 = a[0] * l[0] * Y[0]

    # SH 1-Basis
    r1 = a[1] * l[1] * Y[1]
    r2 = a[1] * l[2] * Y[2]
    r3 = a[1] * l[3] * Y[3]

    # SH 1-Basis
    r4 = a[2] * l[4] * Y[4]
    r5 = a[2] * l[5] * Y[5]
    r6 = a[2] * l[6] * Y[6]
    r7 = a[2] * l[7] * Y[7]
    r8 = a[2] * l[8] * Y[8]

    R = r0 + r1 + r2 + r3 + r4 + r5 + r6 + r7 + r8

    return R

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


def sh_illum_error(image, albedo, illum, normals, weight):

    # I(u,v) = A(u,v) * sum_{lm}( a_l * l_lm * Y_lm(n(u,v))
    # A(u,v) : Albedo (surface reflectance color) at image coordiange
    # a_l : Lambertian SH Basis coeff
    # l_lm : lighting parameter of m coeff at SH basis l
    # Y_lm : diffuse surface energy using Normal of m coeff at SH basis l
    # n(u,v) : normal at image coordinate
    # l : SH basis
    # m : m coeff at SH basis l

    R = computeSHEnergy(illum, normals)

    I_est = albedo.reshape(-1) * R

    residuals = weight * (image.reshape(-1) - I_est)

    return residuals
