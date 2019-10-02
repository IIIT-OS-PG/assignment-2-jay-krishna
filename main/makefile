all: tracker peer

# tracker: tracker1/tracker.o tracker1/trackerdriver.o
# 	g++ -pthread -Wall tracker1/tracker.o tracker1/trackerdriver.o -o tracker1/tracker

# tracker.o: tracker1/tracker.cpp
# 	g++ -pthread -Wall -c tracker1/tracker.cpp

# trackerdriver.o: tracker1/trackerdriver.cpp
# 	g++ -pthread -Wall -c tracker1/trackerdriver.cpp

# peer: peer1/peer.o peer1/peercredentials.o
# 	g++ -pthread -Wall peer1/peer.o peer1/peercredentials.o -o peer

# peer.o: peer1/peer.cpp
# 	g++ -pthread -Wall -c peer1/peer.cpp

# peercredentials.o: peer1/peercredentials.cpp
# 	g++ -pthread -Wall -c peer1/peercredentials.cpp

tracker: tracker1/tracker.cpp
	g++ -pthread -Wall tracker1/tracker.cpp -o tracker1/tracker

peer: peer1/peer.cpp
	g++ -pthread -Wall peer1/peer.cpp -o peer1/peer