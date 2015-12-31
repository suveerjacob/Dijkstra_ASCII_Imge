# Dijkstra_ASCII_Imge


Author : Suveer Jacob
	Description : The following program is an implementation of the Dijkstra's Algorithm.
	  It takes input from a graph.dat file and is of the following format: 
	  
	  
	  node# 1st-arc-to-node# weight-for-previous-arc 2nd-arc-to-node# weight-for-previous-arc 3rd-arc-to-node# weight-for-previous-arc ...
	  
				As an example:
				  1 2 10 3 5 4 100
				  2 1 5
				  4 3 50 2 10
				  

				  In the above example, node 1 has an arc with weight 10 to node 2, weight 5 to node 3, and weight 100 				  to node 4. 

				  Node 2 has an arc with weight 5 to node 1.
				  Node 4 has an arc to node 3 with weight 50 and to node 2 with weight 10.
				  Note that in the above example, there is a node 3 despite not being specifically declared on a line.
				  
				  Any node that is either explicitly defined on its own line, or into which an arc goes, must be 					  considered to exist.
				  It displays the shortest path from each node of the graph to every other node.  
				  It also displays an ASCII art image of the graph provided as the input.
				  For convenience all nodes of the graph are diaplayed vertically and are connected to each other as 					  given in the input file.
				  

	**** Note : If the graph is not displayed properly on Console, then expand the CMD promt horizontally.
				### ASCII art image can also be viewed in "graph_output.dat" file. ###
