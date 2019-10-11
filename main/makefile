all: tracker1 tracker2 peer1 peer2 peer3

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

# tracker1: tracker1/tracker.cpp
# 	g++ -pthread -Wall tracker1/tracker.cpp -o tracker1/tracker1

tracker1: tracker1/tracker1.o tracker1/trackersync1.o
	g++ -pthread -Wall tracker1/tracker1.o tracker1/trackersync1.o -o tracker1/tracker1 -lssl -lcrypto

tracker1/tracker1.o: tracker1/tracker.cpp
	g++ -pthread -Wall -c -o tracker1/tracker1.o tracker1/tracker.cpp -lssl -lcrypto

tracker1/trackersync1.o: tracker1/trackersync.cpp
	g++ -pthread -Wall -c -o tracker1/trackersync1.o tracker1/trackersync.cpp -lssl -lcrypto

tracker2: tracker2/tracker2.o tracker2/trackersync2.o
	g++ -pthread -Wall tracker2/tracker2.o tracker2/trackersync2.o -o tracker2/tracker2 -lssl -lcrypto

tracker2/tracker2.o: tracker2/tracker.cpp
	g++ -pthread -Wall -c -o tracker2/tracker2.o tracker2/tracker.cpp -lssl -lcrypto

tracker2/trackersync2.o: tracker2/trackersync.cpp
	g++ -pthread -Wall -c -o tracker2/trackersync2.o tracker2/trackersync.cpp -lssl -lcrypto

peer1: peer1/peer1.o peer1/peerserver1.o
	g++ -pthread -Wall peer1/peer1.o peer1/peerserver1.o -o peer1/peer1 -lssl -lcrypto

peer1/peer1.o: peer1/peer.cpp
	g++ -pthread -Wall -c -o peer1/peer1.o peer1/peer.cpp -lssl -lcrypto

peer1/peerserver1.o: peer1/peerserver.cpp 
	g++ -pthread -Wall -c -o peer1/peerserver1.o peer1/peerserver.cpp -lssl -lcrypto

peer2: peer2/peer2.o peer2/peerserver2.o
	g++ -pthread -Wall peer2/peer2.o peer2/peerserver2.o -o peer2/peer2 -lssl -lcrypto

peer2/peer2.o: peer2/peer.cpp
	g++ -pthread -Wall -c -o peer2/peer2.o peer2/peer.cpp -lssl -lcrypto

peer2/peerserver2.o: peer2/peerserver.cpp 
	g++ -pthread -Wall -c -o peer2/peerserver2.o peer2/peerserver.cpp -lssl -lcrypto

peer3: peer3/peer3.o peer3/peerserver3.o
	g++ -pthread -Wall peer3/peer3.o peer3/peerserver3.o -o peer3/peer3 -lssl -lcrypto

peer3/peer3.o: peer3/peer.cpp
	g++ -pthread -Wall -c -o peer3/peer3.o peer3/peer.cpp -lssl -lcrypto

peer3/peerserver3.o: peer3/peerserver.cpp
	g++ -pthread -Wall -c -o peer3/peerserver3.o peer3/peerserver.cpp -lssl -lcrypto