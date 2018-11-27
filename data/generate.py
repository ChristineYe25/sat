#!/usr/bin/env python
import os

f = open("run_all", "w")
exe = '/home/jiwei/Documents/cs259/minisat/core/minisat_static'

for file in os.listdir("."):
	if ".cnf" in file: 
		f.write(exe + " " + file+"\n")
