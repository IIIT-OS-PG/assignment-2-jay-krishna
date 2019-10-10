all: tracker peer1 peer2

# tracker: tracker1/tracker.o tracker1/trackerdriver.o
# 	g++ -pthread -Wall tracker1/tracker.o tracker1/trackerdriver.o -o tracker1/tracker

# tracker.o: tracker1/tracker.cpp
# 	g++ -pthread -Wall -c tracker1/tracker.cpp

# trackerdriver.o: tracker1/trackerdriver.cpp
# 	g++ -pthread -Wall -c tracker1/trackerdriver.cpp

# peer.o: peer1/peer.cpp
# 	g++ -pthread -Wall -c peer1/peer.cpp

# peer: peer1/peer.cpp
# 	g++ -pthread -Wall peer1/peer.cpp -o peer1/peer

tracker: tracker1/tracker.cpp
	g++ -pthread -Wall tracker1/tracker.cpp -o tracker1/tracker

peer1: peer1/peer1.o peer1/peerserver1.o
	g++ -pthread -Wall peer1/peer1.o peer1/peerserver1.o -o peer1/peer1

peer1/peer1.o: peer1/peer.cpp
	g++ -pthread -Wall -c -o peer1/peer1.o peer1/peer.cpp

peer1/peerserver1.o: peer1/peerserver.cpp 
	g++ -pthread -Wall -c -o peer1/peerserver1.o peer1/peerserver.cpp

peer2: peer2/peer2.o peer2/peerserver2.o
	g++ -pthread -Wall peer2/peer2.o peer2/peerserver2.o -o peer2/peer2

peer2/peer2.o: peer2/peer.cpp
	g++ -pthread -Wall -c -o peer2/peer2.o peer2/peer.cpp

peer2/peerserver2.o: peer2/peerserver.cpp 
	g++ -pthread -Wall -c -o peer2/peerserver2.o peer2/peerserver.cpp