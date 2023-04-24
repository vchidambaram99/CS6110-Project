#!/usr/bin/env python3

import argparse
import os
import subprocess
import sys
import time
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

def runSimulations(commands, outputFiles):
	running = [None] * os.cpu_count()
	for i in range(len(outputFiles)):
		started = False
		if (os.path.isfile(outputFiles[i])): 
			print("Skipping {}".format(outputFiles[i]))
			continue
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

def readResults(filename):
	with open(filename) as f:
		maliciousNode = int(f.readline());
		stats = []
		for line in f:
			stats.append(np.array([float(s) for s in line.split()[1:]]))
		return maliciousNode, np.array(stats)
	
def agreementRatioGraph(df, rounds, nodeTypes, graphName, filename):
	xarr = np.array(range(rounds))
	for n in nodeTypes:
		data = df.loc[n, 0, :, :][1].unstack(level=0).to_numpy()
		means = data.mean(1)
		plt.plot(xarr, means, label=n)
	plt.xlabel("Rounds")
	plt.ylabel("Agreement Ratio")
	plt.title("Agreement Ratio vs Rounds for {}".format(graphName))
	plt.legend()
	plt.savefig(filename)
	plt.show()
	plt.close()

def sortedAgreementRatio(df, rounds, simulations, nodeTypes, graphName, filename):
	xarr = np.array(range(simulations))
	for n in nodeTypes:
		data = np.sort(df.loc[n, 0, :, rounds - 1][1].to_numpy())
		plt.plot(xarr, data, label=n)
	plt.xlabel("Simulations")
	plt.ylabel("Agreement Ratio")
	plt.title("Sorted Agreement Ratio for {}".format(graphName))
	plt.legend()
	plt.savefig(filename)
	plt.show()
	plt.close()

def leaderChangeGraph(df, rounds, interval, simulations, nodeTypes, graphName, filename):
	intervals = int(rounds / interval)
	xarr = np.array(range(1, intervals+1))*interval
	width = interval / (len(nodeTypes) + 1)
	offset = -width * len(nodeTypes)/2
	for n in nodeTypes:
		data = df.loc[n, 0, :, :][0].unstack(level=0).to_numpy()
		changes = (data[1:,:] != data[:-1,:])
		intervalChanges = np.zeros((intervals, simulations))
		for i in range(intervals):
			intervalChanges[i,:] = changes[i*interval:(i+1)*interval, :].sum(0)
		means = intervalChanges.mean(1)
		plt.bar(xarr+offset, means, label=n, width=width, align='edge')
		offset += width
	plt.xlabel("Rounds")
	plt.ylabel("Leader Changes")
	plt.title("Leader Changes per interval for {}".format(graphName))
	plt.legend()
	plt.savefig(filename)
	plt.show()
	plt.close()

def failureRatioScatter(df, rounds, nodeTypes, attackEdges, maliciousNode, graphName, filename):
	xarr = np.array(attackEdges)
	markers = ['o', 'x', '+', '^']
	for i, n in enumerate(nodeTypes):
		data = np.zeros(len(attackEdges))
		for j in range(len(attackEdges)):
			data[j] = (df.loc[n, attackEdges[j], :, rounds - 1][0].to_numpy() >= maliciousNode).mean()
		plt.scatter(xarr, data, label=n, marker=markers[i],s=100)
	plt.xlabel("Attack Edges")
	plt.ylabel("FailureRatio")
	plt.title("Failure Ratio vs AttackEdges for {}".format(graphName))
	plt.legend()
	plt.savefig(filename)
	plt.show()
	plt.close()

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
	nodeNames = [n[0] for n in nodeTypes]

	outputArgs = []
	outputFiles = []
	commands = []
	for s in range(simulations):
		for g in args.graphs:
			for nodeName, nodeType, candidates in nodeTypes:
				for edges in attackEdges:
					outputArgs.append([os.path.basename(g), nodeName, edges, s])
					outputFiles.append("{}/{}_{}_{}_{}.txt".format(args.results, outputArgs[-1][0], nodeName, edges, s))
					if (args.program != None):
						commands.append("{} -g {} -n {} -c {} -m {} -s {} -r {} > {}".format(args.program, g, nodeType, candidates, edges, s, rounds, outputFiles[-1]))
	if (args.program != None):
		runSimulations(commands, outputFiles)

	results = {}
	midx = pd.MultiIndex.from_product([nodeNames, attackEdges, list(range(simulations)), list(range(rounds))])
	for g in args.graphs:
		results[os.path.basename(g)] = [0, pd.DataFrame(np.zeros((len(midx), 4)), index = midx).sort_index()]
	for i in range(len(outputArgs)):
		maliciousNode, data = readResults(outputFiles[i])
		results[outputArgs[i][0]][0] = maliciousNode # malicious node will end up being the same for every simulation on the same graph
		results[outputArgs[i][0]][1].loc[outputArgs[i][1], outputArgs[i][2], outputArgs[i][3]] = data 
	for g in args.graphs:
		g = os.path.basename(g)
		maliciousNode = results[g][0]
		dataframe = results[g][1]
		agreementRatioGraph(dataframe, rounds, nodeNames, g, "{}/agreementRatio_{}.png".format(args.results, g))
		sortedAgreementRatio(dataframe, rounds, simulations, nodeNames, g, "{}/sortedAgreement_{}.png".format(args.results, g))
		leaderChangeGraph(dataframe, rounds, 25, simulations, nodeNames, g, "{}/leaderChange_{}.png".format(args.results, g))
		failureRatioScatter(dataframe, rounds, nodeNames, attackEdges[1:], maliciousNode, g, "{}/failureRatio_{}.png".format(args.results, g))

		

if __name__ == "__main__":
	main();
