#!/usr/bin/env python

import os
import itertools
import numpy as np

import cv2
import argparse
from skimage import data, img_as_float, color
from skimage.util import random_noise
import chumpy as ch
from chumpy import Ch
from os.path import join

from argparse import ArgumentParser
import os.path as path

import SHLoss as sh

def is_valid_file(parser, arg):
    if not path.exists(arg):
        parser.error("The file %s does not exist!" % arg)
    else:
        return path.abspath(arg) #open(arg, 'r')  # return an open file handle


def get_arguments():
    """Parse all the arguments provided from the CLI.
    Returns:
      A list of parsed arguments.
    """
    parser = argparse.ArgumentParser(description="Normals From Shading: Environment Light Estimation.")
    parser.add_argument("--image", dest="image", required=True,
                    help="Path to raw RGB Image", metavar="FILE",
                    type=lambda x: is_valid_file(parser, x))
    parser.add_argument("--albedo", dest="albedo", required=True,
                    help="Path to albedo Image", metavar="FILE",
                    type=lambda x: is_valid_file(parser, x))
    parser.add_argument("--normals", dest="normalMap", required=True,
                    help="Path to normal map", metavar="FILE",
                    type=lambda x: is_valid_file(parser, x))
    parser.add_argument("--output", dest="output", default='./output-normals.bmp',
                    help="Path to output file", type=str)
    return parser.parse_args()

def mkdir_s( dir ):
    if not path.exists( dir ):
        os.mkdir( dir )

# -----------------------------------------------------------------------------
def normalsFromShading(image,        # input RGB image
                       albedo,       # albedo image
                       illum,   # Initial SH illumination Parameters
                       normals, # Initial normal map
                       weights,      # weights for the objectives
                       opt_options): # options

    image_ch = ch.array(image)
    albedo_ch = ch.array(albedo)
    illum_ch = ch.array(illum)
    normals_ch = ch.array(normals)

    """ function: estimate Normals from Shading using Spherical Harmonics
    input:
        image: input RGB image
        albedo: albedo image
        illum_init: Initial SH illumination Parameters
        normals_init: Initial normal map
        weights: weights for the objectives
        opt_options:  optimization options
    output:
        illum: estimated SH illumination
        normals: estimated Normals
    """

    # options
    if opt_options is None:
        print("normalsFromShading(): no 'opt_options' provided, use default settings.")
        import scipy.sparse as sp
        opt_options = {}
        opt_options['disp'] = 1
        opt_options['delta_0'] = 0.1
        opt_options['e_3'] = 1e-4
        opt_options['maxiter'] = 100
        sparse_solver = lambda A, x: sp.linalg.cg(A, x, maxiter=opt_options['maxiter'])[0]
        opt_options['sparse_solver'] = sparse_solver

    # weights
    print("normalsFromShading(): use the following weights:")
    for kk in weights.keys():
        print("normalsFromShading(): weights['%s'] = %f" % (kk, weights[kk]))

        # objectives
    # Illumination Error
    illum_err = sh.illum_error(image=image_ch,
                               albedo=albedo_ch,
                               illum=illum_ch,
                               normals=normals_ch,
                               weight=weights['illum'])


    # regularizer
    #illum_reg = weights['illum_reg'] * illum_ch
    normals_reg = Ch( weights['normals_reg']) * normals_ch

    objectives = {}
    objectives.update({'illum': illum_err})#, {'normals_reg': normals_reg})

    # on_step callback
    def on_step(_):
        pass

    # optimize
    # step 1: rigid alignment
    from time import time
    timer_start = time()
    print("\nstep 1: start Illumination Estimation...")
    ch.minimize(fun=illum_err,
                x0=[illum_ch],
                method='dogleg',
                callback=on_step,
                options=opt_options)
    timer_end = time()
    print("step 1: Illumination Estimation done, in %f sec\n" % (timer_end - timer_start))

    # step 2: non-rigid alignment
    timer_start = time()
    print("step 2: start Normal Refinement...")
    ch.minimize(fun=objectives,
                x0=[normals_ch],
                method='dogleg',
                callback=on_step,
                options=opt_options)
    timer_end = time()
    print("step 2: Normal Refinement done, in %f sec\n" % (timer_end - timer_start))

    R = sh.computeSHEnergy(illum_ch, normals_ch)

    result = albedo.reshape(-1) * R

    # return results
    return illum_ch.r, normals_ch.r, result.r.reshape(image_ch.shape)


# -----------------------------------------------------------------------------

def run_fitting(image, albedo, normals_init, outputPath):

    # output
    dir_path = path.dirname(path.realpath(outputPath))
    mkdir_s(dir_path)

    # weights
    weights = {}
    weights['illum'] = 1.0
    weights['normals'] = 1.0
    weights['illum_reg'] = 0.0
    weights['normals_reg'] = 0.001

    # optimization options
    import scipy.sparse as sp
    opt_options = {}
    opt_options['disp'] = 1
    opt_options['delta_0'] = 0.1
    opt_options['e_3'] = 1e-4
    opt_options['maxiter'] = 100
    sparse_solver = lambda A, x: sp.linalg.cg(A, x, maxiter=opt_options['maxiter'])[0]
    opt_options['sparse_solver'] = sparse_solver

    illum_init = np.zeros(9)

    # run fitting
    illum, normals, result = \
        normalsFromShading( image=image,  # input RGB image
                            albedo=albedo,  # albedo image
                            illum=illum_init,  # albedo image
                            normals=normals_init, # Initial normal map
                            weights=weights,  # weights for the objectives
                            opt_options=opt_options)  # options

    # write result
    print("Estimatied Lignting Params:\n{0}".format(illum))
    cv2.imwrite(outputPath, normals*255)
    cv2.imwrite("result.png", result*255)
    cv2.imwrite("albedo.png", albedo*255)


# -----------------------------------------------------------------------------
def main():
    args = get_arguments()

    imagePath = args.image
    albedoPath = args.albedo
    normalsPath = args.normalMap
    outputPath = args.output

    # print(imagePath)
    # print(albedoPath)
    # print(normalsPath)
    print("Output Path: {}".format(outputPath))

    # Read Images into numpy arrays and normalize
    # TODO: compute SH for each channel
    image = cv2.imread(imagePath, cv2.IMREAD_GRAYSCALE)/255
    albedo = cv2.imread(albedoPath, cv2.IMREAD_GRAYSCALE)/255
    normalMap = cv2.imread(normalsPath, cv2.IMREAD_COLOR)/255

    #print(image.shape)
    #print(albedo.shape)
    #print(normalMap.shape)

    run_fitting(image, albedo, normalMap, outputPath)

    # run_fitting()

# -----------------------------------------------------------------------------

if __name__ == '__main__':
    main()