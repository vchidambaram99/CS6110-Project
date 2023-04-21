#!/usr/bin/env python3

import argparse
import os
import subprocess
import sys
import time
import numpy as np

def runSimulations(commands, outputFiles):
	running = [None] * os.cpu_count()
	for i in range(len(outputFiles)):
		started = False
		if (os.path.isfile(outputFiles[i])): continue # skip cases already run
		while (not started):
			for j in range(len(running)):
				if (running[j] == None or running[j].poll() != None):
					print("Running {}".format(commands[i]))
					running[j] = subprocess.Popen(commands[i], shell=True)
					started = True
					break
			if (not started): time.sleep(1)

	for i in range(len(running)):
		if (running[i] != None): running[i].wait()

def main():
	parser = argparse.ArgumentParser(description='Generate simulation data or create visualizations of it')
	parser.add_argument("-r", "--results", required=True, help="Folder to place results in or get results from")
	parser.add_argument("-g", "--graphs", action='append', help="Graphs to run simulations on or create visualizations for")
	parser.add_argument("-p", "--program", help="Path to program to use to generate results, only produce visualizations if not provided")
	args = parser.parse_args(sys.argv[1:])

	rounds = 200
	simulations = 100
	attackEdges = [0, 50, 100, 150, 200, 250]
	nodeTypes = [("tmr", "tmr", 1), ("plurality", "borda", 1), ("borda5", "borda", 5)]

	outputArgs = []
	outputFiles = []
	commands = []
	for s in range(simulations):
		for g in args.graphs:
			for nodeName, nodeType, candidates in nodeTypes:
				for edges in attackEdges:
					outputArgs = [args.results, os.path.basename(g), nodeName, edges, s]
					outputFiles.append("{}/{}_{}_{}_{}.txt".format(args.results, os.path.basename(g), nodeName, edges, s))
					if (args.program != None):
						commands.append("{} -g {} -n {} -c {} -m {} -s {} -r {} > {}".format(args.program, g, nodeType, candidates, edges, s, rounds, outputFiles[-1]))
	if (args.program != None):
		runSimulations(commands, outputFiles)

	# TODO visualizations

if __name__ == "__main__":
	main();
