import sys
import os
import os.path as path
sys.path.append(os.path.abspath('..'))

import argparse
import cv2
from skimage import io
from matplotlib import pyplot as plt
from estimateLighting import run_fitting

import numpy as np

import re


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
    parser = argparse.ArgumentParser(description="Refine Normals From Shading")
    parser.add_argument("--data", dest="data", required=True,
                    help="Path to data", metavar="FILE",
                    type=lambda x: is_valid_file(parser, x))
    parser.add_argument("--albedo-pattern", dest="albedo", default='.png',
                    help="Albedo File Pattern", type=str)
    parser.add_argument("--normal-pattern", dest="normal", default='.n.png',
                    help="Normals File Pattern", type=str)
    parser.add_argument("--image-pattern", dest="target", default='.orig.png',
                    help="Image (Target) File Pattern, match groups must be (unique)(ending)", type=str)
    parser.add_argument("--out-dir", dest="output", default='./output',
                    help="Path to output directory", type=str)
    return parser.parse_args()


def mkdir_s( dir ):
    if not path.exists( dir ):
        os.mkdir( dir )

# -----------------------------------------------------------------------------
def main():
    args = get_arguments()

    print("Output Path: {}".format(args.output))

    mkdir_s(args.output)

    data_dir = args.data
    for filename in os.listdir(data_dir):
        if filename.endswith(".jpg") or filename.endswith(".png"):
            x = re.search(r"(\d+)({})".format(args.albedo), filename)
            if x is not None:
                id, ending = x.groups()
                print('Reading albedo {}'.format(os.path.join(data_dir, filename)))
                albedo = cv2.imread(os.path.join(data_dir, filename), cv2.IMREAD_GRAYSCALE)/255

                normal_fn = "{}{}".format(id,args.normal)
                print('Reading albedo {}'.format(os.path.join(data_dir, normal_fn)))
                normals = cv2.imread(os.path.join(data_dir, normal_fn),cv2.IMREAD_COLOR)
                normalMap = (normals / 128) - 1

                target_fn = "{}{}".format(id,args.target)
                print('Reading albedo {}'.format(os.path.join(data_dir, target_fn)))
                target = cv2.imread(os.path.join(data_dir, target_fn), cv2.IMREAD_GRAYSCALE)

                # Generate mask from normals
                mask = (np.sum(normals,2) > 0).astype(np.uint8)*255

                # io.imshow(mask)
                # io.show()
                # cv2.waitKey(500)

                # Mask Target
                # mask_rgb = cv2.cvtColor(mask, cv2.COLOR_GRAY2BGR)  # change mask to a 3 channel image
                target_masked = cv2.subtract(mask, target)
                target_masked = cv2.subtract(mask, target_masked)

                # io.imshow(target_masked)
                # io.show()
                # cv2.waitKey(500)

                out_illum_fn = "{}.sh-illum.csv".format(id)
                out_normals_fn = "{}.n-refine.png".format(id)

                w, h, c = normals.shape

                edge_map = np.zeros([w,h], dtype=np.float64)
                edge_list = np.zeros([w,h,8], dtype=np.uint64)
                for u in range(1,w-1):
                    for v in range(1,h-1):
                        window = normals[u-1:u+2, v-1:v+2]
                        edge_map[u,v] = (np.min(np.sum(window, 2)) > 0).astype(dtype=np.uint8)

                        edge_idx = 0
                        for i in range(-1,2):
                            for j in range(-1,2):
                                if i is 0 and j is 0: continue
                                edge_list[u,v, edge_idx] = (u+i)*h + (v+j)
                                edge_idx += 1
                edge_map = edge_map.reshape([w*h])
                edge_list = edge_list.reshape([w*h,8])
                mask_reg = np.nonzero(edge_map)
                #
                # laplacian = np.sum(normalMap.reshape([w*h,3])[edge_list], 1)
                # laplacian = 8*normalMap.reshape([w*h,3]) - laplacian

                # laplacian = np.sum(normalMap.reshape([w*h,3])[edge_list], 1)
                # laplacian = 8*normalMap.reshape([w*h,3]) - laplacian

                mask_idx = np.nonzero(mask)

                # illum, normals_refine = run_fitting(target_masked[mask_idx]/255, albedo[mask_idx], normalMap[mask_idx])
                #
                # normalMap_refine = normalMap
                # normalMap_refine[mask_idx] = normals_refine

                illum, normalMap_refine = run_fitting(target_masked / 255, albedo, normalMap, mask_idx, mask_reg, edge_list)

                np.savetxt(path.join(args.output, out_illum_fn), illum, delimiter=",")
                cv2.imwrite(path.join(args.output, out_normals_fn), (normalMap_refine + 1) * 128)

    cv2.destroyAllWindows()

# -----------------------------------------------------------------------------

if __name__ == '__main__':
    main()