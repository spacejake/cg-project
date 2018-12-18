import numpy as np
import chumpy as ch
from chumpy import Ch
import SHLoss as sh



n = ch.array([[[ 1., 2., 3.],
           [ 1., 2., 3.],
           [ 1., 2., 3.]],

           [[ 1., 2., 3.],
            [ 1., 2., 3.],
            [ 1., 2., 3.]]])

albedo = ch.array(np.ones([2,3]))
image = ch.array(np.ones([2,3]))
l = ch.array(np.ones(9))



a = sh.getLambertionBasisFactors()
c = sh.getSHNormalBasis()

nx = n[:,:,0].reshape(-1)
ny = n[:,:,1].reshape(-1)
nz = n[:,:,2].reshape(-1)

R0 = a[0] * l[0] * c[0]

R1 = a[1] * l[1]* c[1] * nx\
     + a[1] * l[2]* c[1] * ny\
     + a[1] * l[3]* c[1] * nz

R2 = a[2] * l[4]* c[2] * nx * ny\
     + a[2] * l[5]* c[2] * nx * nz\
     + a[2] * l[6]* c[2] * ny * nz\
     + a[2] * l[7]* (c[2] / Ch(2.)) * (nx**2. - ny**2.)\
     + a[2] * l[8]* (c[2] / (Ch(2.) * ch.sqrt(3.))) * (Ch(3.) * nz**2. - 1.)

R = R0 + R1 + R2

I_est = albedo.reshape(-1) * R

res = image.reshape(-1) - I_est
res_np = res.r

print(R)
print(I_est)
print(res_np)

