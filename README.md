# Graph_Algorithms
This program takes road data in the form country.osm.graph and country.osm.xyz and answers the following graph questions:
1. Order and size of graph.
2. Largest Degree of graph
3. Avergae Degree of graph
4. Neighbours at edge-distance 1
5. Neighbours at edge-distance k
6. Shortest path between two vertices
7. shortest edge count between two vertices

To run the program: ./streets <filename> <query_number> <expected_parameters> <br />
  Filename is minus .osm.graph and osm.xyz, these are appended later. <br />
  For queries 1, 2, 3 no paramteres are expected. <br />
  For 4 one parameter is expected which is the vertex number. <br />
  For 5 two parameters are expected. the vertex number and the distance k. <br />
  For 6 and 7 two vertices are expected. <br />
