# this program is used to randomly select files and record the names of the files that have been selected.
# Written by Joe Yang
# 25 July, 2011
import os
import random
import dircache

dir = 'some/directory'
filename = random.choice(dircache.listdir(dir))
path = os.path.join(dir, filename)

